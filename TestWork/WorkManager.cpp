#include "ThreadPool.h"
#include "WorkManager.h"
#include "ReadFileMngr.h"
#include "WriteFileMngr.h"
#include "Settings.h"

#include <condition_variable>
#include <mutex>

using namespace ThreadPool;

WorkManager::WorkManager(std::shared_ptr<ISettings>&& settings) : m_settings(std::move(settings)) {

	m_ReadFileMngr = std::make_unique<ReadFileMngr>(m_settings);
	m_HashMngr = std::make_unique<HashMngr>(m_settings);
	m_WriteFileMngr = std::make_unique<WriteFileMngr>(m_settings);
}

bool WorkManager::StartWork() noexcept {

	GetMainThreadPool().StartingWork();
	
	size_t s;

	m_ReadFileMngr->InitializeWork(s);
	m_HashMngr->InitializeWork();
	m_WriteFileMngr->InitializeWork((s / m_settings->GetChunkSize() + (s % m_settings->GetChunkSize() ? 1 : 0)) * 16);

	for (unsigned long i = 0;  i < GetMainThreadPool().GetMinThreadCount(); i++) {
		m_Items.emplace_back(std::make_shared<ItemConveyer>());
		WriteCompleteChunck(m_Items[i]);
		m_thread_used++;
	}

	return true;
}

void WorkManager::StopWork() {
	m_ReadFileMngr->StopWork();
	m_WriteFileMngr->StopWork();
}

bool WorkManager::IsWorkDone() {
	return m_thread_used == 0;
}

void WorkManager::ReadCompleteChunck(std::shared_ptr<ItemConveyer>& item) {
	m_HashMngr->Hashing(item->GetItemHashIface());
	m_WriteFileMngr->Writing(item->GetItemWriteIface([item, this]() mutable { return this->WriteCompleteChunck(item);} ));
}

void WorkManager::WriteCompleteChunck(std::shared_ptr<ItemConveyer>& item) {

	if (!m_ReadFileMngr->Reading(item->GetItemReadIface([item, this]() mutable { return this->ReadCompleteChunck(item); }))) {
		m_thread_used--;
	}
}


#include "ThreadPool.h"
#include "WorkManager.h"
#include "ReadFileMngr.h"
#include "WriteFileMngr.h"
#include "Settings.h"

using namespace ThreadPool;

WorkManager::WorkManager(std::shared_ptr<ISettings>&& settings) : m_settings(move(settings)) {

	m_ReadFileMngr = std::make_unique<ReadFileMngr>(m_settings);

	m_HashMngr = std::make_unique<HashMngr>(m_settings);

	m_WriteFileMngr = std::make_unique<WriteFileMngr>(m_settings);
}

bool WorkManager::StartWork() noexcept {

	GetMainThreadPool().StartingWork();
	size_t s;
	m_ReadFileMngr->InitializeWork(s);
	m_HashMngr->InitializeWork();
	m_WriteFileMngr->InitializeWork(s);

	for (unsigned long i = 0; i < GetMainThreadPool().GetMinThreadCount(); i++) {
		m_Items.emplace_back(std::make_shared<ItemConveyer>());
		WriteCompleteChunck(m_Items[i]);
	}

	return true;
}

void WorkManager::StopWork() {

}

void WorkManager::ReadCompleteChunck(std::shared_ptr<ItemConveyer>& item) {

	m_WriteFileMngr->Writing(item->GetItemWriteIface([item, this]() mutable { return this->WriteCompleteChunck(item);} ));
}

void WorkManager::WriteCompleteChunck(std::shared_ptr<ItemConveyer>& item) {

/*	{
		auto p = item->GetItemReadIface([item, this]() mutable { return this->ReadCompleteChunck(item); });
	}*/

	m_ReadFileMngr->Reading(item->GetItemReadIface([item, this]() mutable { return this->ReadCompleteChunck(item); }));
}


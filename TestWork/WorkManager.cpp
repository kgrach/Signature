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

	m_ReadFileMngr->InitializeWork();
	m_HashMngr->InitializeWork();
	m_WriteFileMngr->InitializeWork();

	for (unsigned long i = 0; i < GetMainThreadPool().GetMaxThreadCount(); i++) {
		auto p = std::make_shared<ItemConveyer>();
		WriteCompleteChunck(p);
	}

	return true;
}

void WorkManager::StopWork() {

}

void WorkManager::ReadCompleteChunck(std::shared_ptr<ItemConveyer>& item) {

	m_HashMngr->Hashing(item->GetItemHashIface());

	auto f = [_item = item, this]() mutable {
		return this->WriteCompleteChunck(_item);
	};

	m_WriteFileMngr->Writing(item->GetItemWriteIface(f));
}

void WorkManager::WriteCompleteChunck(std::shared_ptr<ItemConveyer>& item) {
	
	auto f = [_item = item, this]() mutable {
		return this->ReadCompleteChunck(_item);
	};

	m_ReadFileMngr->Reading(item->GetItemReadIface(f));
}


#include "ThreadPool.h"
#include "WorkManager.h"
#include "ReadFileMngr.h"
#include "WriteFileMngr.h"
#include "ISettings.h"

using namespace ThreadPool;

WorkManager::WorkManager(std::shared_ptr<ISettings>&& settings) : m_settings(move(settings)) {

	auto fReadComplete = [this](size_t offset, std::unique_ptr<std::vector<unsigned char>> buff) {
		return this->ReadCompleteChunck(offset, move(buff));
	};

	m_ReadFileMngr = std::make_unique<ReadFileMngr>(m_settings, fReadComplete);


	auto fHashComplete = [this](size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
		return this->HashCompleteChunck(offset, std::move(buff), std::move(hash));
	};

	m_HashMngr = std::make_unique<HashMngr>(m_settings, fHashComplete);

	auto fWriteComplete = [this](std::unique_ptr<std::vector<unsigned char>> buff) {
		return this->WriteCompleteChunck(move(buff));
	};

	m_WriteFileMngr = std::make_unique<WriteFileMngr>(m_settings, fWriteComplete);
}

bool WorkManager::StartWork() noexcept {

	GetMainThreadPool().StartingWork();

	bool start_done;

	do {
		
		auto buff = std::make_unique<std::vector<unsigned char>>(m_settings->GetChunkSize());
		start_done = m_ReadFileMngr->Reading(std::move(buff));

	} while (start_done);
	
	return true;
}


void WorkManager::ReadCompleteChunck(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff) {

	m_HashMngr->Hashing1(offset, std::move(buff));
}

void WorkManager::HashCompleteChunck(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
	m_ReadFileMngr->Reading(std::move(buff));
	m_WriteFileMngr->Writing(offset/hash->size(), std::move(hash));
}

void WorkManager::WriteCompleteChunck(std::unique_ptr<std::vector<unsigned char>> hash) {
	m_HashMngr->Hashing2(std::move(hash));
}


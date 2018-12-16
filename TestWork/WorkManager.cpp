#include "ThreadPool.h"
#include "WorkManager.h"
#include "ReadFileMngr.h"
#include "WriteFileMngr.h"

using namespace std;
using namespace ThreadPool;

WorkManager::WorkManager(shared_ptr<ISettings>&& settings) : m_settings(move(settings)) {

	auto fReadComplete = [this](std::unique_ptr<std::vector<unsigned char>> buff) {
		return this->ReadCompleteChunck(move(buff));
	};

	m_ReadFileMngr = make_unique<ReadFileMngr>(m_settings, fReadComplete);


	auto fWriteComplete = [this](std::unique_ptr<std::vector<unsigned char>> buff) {
		return this->WriteCompleteChunck(move(buff));
	};

	m_WriteFileMngr = make_unique<WriteFileMngr>(m_settings, fWriteComplete);
}

bool WorkManager::StartWork() noexcept {

	GetMainThreadPool().StartingWork();

	return true;
}


void WorkManager::ReadCompleteChunck(std::unique_ptr<std::vector<unsigned char>> buff) {

}

void WorkManager::HashCompleteChunck(std::unique_ptr<std::vector<unsigned char>> buff) {

}

void WorkManager::WriteCompleteChunck(std::unique_ptr<std::vector<unsigned char>> buff) {

}


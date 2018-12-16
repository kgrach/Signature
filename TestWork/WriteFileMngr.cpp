#include "WriteFileMngr.h"
#include "ThreadPool.h"
#include "ISettings.h"

#include <functional>
#include <memory>

using namespace ThreadPool;
using namespace std;

WriteFileMngr::WriteFileMngr(std::shared_ptr<ISettings>& settings, std::function<void(std::unique_ptr<std::vector<unsigned char>>)> fCallback) : m_settings(settings), m_fCallback(fCallback) {

}

bool WriteFileMngr::InitializeWork() {

	m_hfileDest = ::CreateFile(m_settings->GetOutFileName().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

	if (!m_hfileDest)
		return false;

	m_offset = 0ull;

	auto fReadComplete = [this](size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> data) {
		return this->WriteComplete(taskNum, offset, move(data));
	};

	for (auto i = 0ul; i < GetMainThreadPool().GetMaxThreadCount(); i++) {
		m_vecIOTask.emplace_back(move(make_unique<WriteFileIOTask>(i, m_hfileDest, fReadComplete)));
		m_vecIOTaskCompleted.push(m_vecIOTask[i]);
		Writing(move(make_unique<vector<unsigned char>>(m_settings->GetChunkSize())));
	}

	return true;
}

bool WriteFileMngr::Writing(unique_ptr<vector<unsigned char>>&& buff) {

	std::shared_ptr<WriteFileIOTask> io;

	if (m_vecIOTaskCompleted.try_pop(io)) {

		size_t offset = m_offset.fetch_add(m_settings->GetChunkSize());

		io->StartWriteFileIOTask(offset, move(buff));

		return true;
	}

	return false;
}

void WriteFileMngr::WriteComplete(size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> data) {

	m_vecIOTaskCompleted.push(m_vecIOTask[taskNum]);

	m_fCallback(move(data));
	///process data
};

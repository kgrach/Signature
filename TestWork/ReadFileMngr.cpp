#include "ReadFileMngr.h"
#include "ThreadPool.h"
#include "ISettings.h"

#include <functional>
#include <memory>

using namespace ThreadPool;
using namespace std;

ReadFileMngr::ReadFileMngr(std::shared_ptr<ISettings>& settings, fReadChunckCallback fCallback) : m_settings(settings), m_fCallback(fCallback) {

}

bool ReadFileMngr::InitializeWork() {

	m_hfileSrc = ::CreateFile(m_settings->GetInFileName().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (!m_hfileSrc)
		return false;

	if (!::GetFileSizeEx(m_hfileSrc, &m_SrcFileSize)) {
		return false;
	}

	m_offset = 0ull;

	auto fReadComplete = [this](size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> data) {
		return this->ReadComplete(taskNum, offset, move(data));
	};

	for (auto i = 0ul; i < GetMainThreadPool().GetMinThreadCount(); i++) {
		m_vecIOTask.emplace_back(move(make_unique<ReadFileIO>(i, m_hfileSrc, fReadComplete)));
		m_vecIOTaskCompleted.push(m_vecIOTask[i]);
	}

	return true;
}

bool ReadFileMngr::Reading(unique_ptr<vector<unsigned char>> buff) {

	std::shared_ptr<ReadFileIO> io;

	if (m_vecIOTaskCompleted.try_pop(io)) {
		
		size_t offset = m_offset.fetch_add(m_settings->GetChunkSize());

		io->StartReadFileIOTask(offset, move(buff));
		
		return true;
	}
	
	return false;
}

void ReadFileMngr::ReadComplete(size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> data) {

	m_vecIOTaskCompleted.push(m_vecIOTask[taskNum]);

	m_fCallback(offset, move(data));
};

#include "ReadFileIO.h"

using namespace ThreadPool;

ReadFileIOTask::ReadFileIOTask(size_t taskNum, HANDLE hFile, fReadComplete fCallback) : ThreadPoolIO(hFile, m_fCallbackCompleted), m_taskNum(taskNum), m_fCallbackClient(fCallback) {
	memset(&m_ov, 0, sizeof(m_ov));
}

bool ReadFileIOTask::IoPending() {
	return ::ReadFile(m_hFile, m_buff->data(), m_buff->size(), NULL, &m_ov) ? true : false;
}

void ReadFileIOTask::SetOffset(size_t offset) {

	LARGE_INTEGER x;

	x.QuadPart = offset;

	m_ov.Offset = x.LowPart;
	m_ov.OffsetHigh = x.HighPart;
}

size_t ReadFileIOTask::GetOffset() const {

	LARGE_INTEGER x;

	x.LowPart = m_ov.Offset;
	x.HighPart = m_ov.OffsetHigh;

	return (size_t)x.QuadPart;
}

void ReadFileIOTask::StartReadFileIOTask(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff) {

	m_buff = std::move(buff);
	SetOffset(offset);

	StartIo();
}



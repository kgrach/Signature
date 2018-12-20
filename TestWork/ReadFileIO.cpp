#include "ReadFileIO.h"

using namespace ThreadPool;

ReadFileIO::ReadFileIO(size_t taskNum, HANDLE hFile, fReadComplete fCallback) : ThreadPoolIO(hFile), m_hFile(hFile), m_taskNum(taskNum), m_fCallbackClient(fCallback) {
	memset(&m_ov, 0, sizeof(m_ov));
}

bool ReadFileIO::IoPending() {
	return ::ReadFile(m_hFile, m_buff->data(), m_buff->size(), NULL, &m_ov) ? true : false;
}

void ReadFileIO::IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred) {
	
	m_buff->resize((*NumberOfBytesTransferred).QuadPart);
	return m_fCallbackClient(m_taskNum, GetOffset(), std::move(m_buff));
}

void ReadFileIO::SetOffset(size_t offset) {

	LARGE_INTEGER x;

	x.QuadPart = offset;

	m_ov.Offset = x.LowPart;
	m_ov.OffsetHigh = x.HighPart;
}

size_t ReadFileIO::GetOffset() const {

	LARGE_INTEGER x;

	x.LowPart = m_ov.Offset;
	x.HighPart = m_ov.OffsetHigh;

	return x.QuadPart;
}

void ReadFileIO::StartReadFileIOTask(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff) {

	m_buff = std::move(buff);
	SetOffset(offset);

	StartIO();
}



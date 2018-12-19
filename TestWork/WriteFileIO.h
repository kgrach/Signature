#pragma once

#include "ThreadPoolIo.h"

#include <memory>
#include <vector>
#include <functional>

using fReadComplete = std::function<void(size_t, size_t, std::unique_ptr<std::vector<unsigned char>>)>;

class WriteFileIO : public ThreadPool::ThreadPoolIO {

	HANDLE							m_hFile;
	size_t							m_taskNum;
	OVERLAPPED						m_ov;

	std::unique_ptr<std::vector<unsigned char>>	m_buff;

	fReadComplete m_fCallbackClient;

	virtual void IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred);
	virtual bool IoPending();

	void SetOffset(size_t offset);
	size_t GetOffset() const;

public:

	WriteFileIO(size_t taskNum, HANDLE hFile, fReadComplete fCallback);
	void StartWriteFileIOTask(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff);
};
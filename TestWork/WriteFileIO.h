#pragma once

#include "ThreadPoolIo.h"

#include <memory>
#include <vector>

using fReadComplete = std::function<void(size_t, size_t, std::unique_ptr<std::vector<unsigned char>>)>;

class WriteFileIOTask : public ThreadPool::ThreadPoolIO {

	HANDLE							m_hFile;
	size_t							m_taskNum;
	OVERLAPPED						m_ov;

	std::unique_ptr<std::vector<unsigned char>>	m_buff;

	fReadComplete m_fCallbackClient;

	ThreadPool::fIOComplete m_fCallbackCompleted = [this](ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred) {
		m_buff->resize((size_t)NumberOfBytesTransferred);
		return m_fCallbackClient(m_taskNum, GetOffset(), std::move(m_buff));
	};

	virtual bool IoPending();

	void SetOffset(size_t offset);
	size_t GetOffset() const;

public:

	WriteFileIOTask(size_t taskNum, HANDLE hFile, fReadComplete fCallback);
	void StartWriteFileIOTask(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff);
};
#pragma once

#include "ThreadPoolIo.h"

#include <memory>
#include <vector>

using fReadComplete = std::function<void(size_t, size_t, std::unique_ptr<std::vector<unsigned char>>)>;

class ReadFileIOTask : public ThreadPool::ThreadPoolIO {

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

	ReadFileIOTask(size_t taskNum, HANDLE hFile, fReadComplete fCallback);
	void StartReadFileIOTask(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff);
};
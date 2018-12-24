#pragma once

#include "ThreadPoolIo.h"
#include "ItemConveyer.h"


template<>
bool ThreadPool::ThreadPoolIO<IItemRead>::IoPending(std::shared_ptr<IOOverlapped<IItemRead>>& ov_item) {
	OVERLAPPED* ov = static_cast<OVERLAPPED*>(ov_item.get());

	ov_item->SetOffset(ov_item->m_item->GetOffset());

	return ::ReadFile(m_hFile, ov_item->m_item->GetBuff().data(), ov_item->m_item->GetBuff().size(), NULL, ov) ? true : false;
}

template <>
void ThreadPool::ThreadPoolIO<IItemRead>::IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred) {

	IOOverlapped<IItemRead> *p = static_cast<IOOverlapped<IItemRead>*>(Overlapped);

	m_ov_complete.push(m_ov[p->m_num]);

	p->m_item->GetBuff().resize((size_t)NumberOfBytesTransferred);

	p->m_item->Callback();
}


using ReadFileIO = ThreadPool::ThreadPoolIO<IItemRead>;

/*
using fReadComplete = std::function<void(size_t, size_t, std::unique_ptr<std::vector<unsigned char>>)>;

class ItemConveyer;

class ReadFileIO : public ThreadPool::ThreadPoolIO {

	HANDLE							m_hFile;
	OVERLAPPED						m_ov;

	fReadComplete m_fCallbackClient;
	
	void IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred);
	bool IoPending();

	void SetOffset(size_t offset);
	size_t GetOffset() const;

public:

	ReadFileIO(HANDLE hFile, fReadComplete fCallback);
	void StartReadFileIOTask(std::shared_ptr<ItemConveyer>&);
};*/
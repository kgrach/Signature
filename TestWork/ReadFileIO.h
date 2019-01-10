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
	std::shared_ptr<IItemRead> item = std::move(p->m_item);

	m_ov_complete.push(m_ov[p->m_num]);

	item->GetBuff().resize((size_t)NumberOfBytesTransferred);
	item->CallbackRead();
}


using ReadFileIO = ThreadPool::ThreadPoolIO<IItemRead>;
#pragma once

#include <memory>
#include <vector>
#include <concurrent_queue.h>

#include "ThreadPool.h"

namespace ThreadPool {

	template <typename T>
	class IOOverlapped  : public OVERLAPPED{

	public:
		size_t m_num;
		std::shared_ptr<T> m_item = nullptr;

		IOOverlapped(size_t num) : m_num(num) {
			memset((OVERLAPPED*)this, 0, sizeof(OVERLAPPED));
		}

		void SetOffset(size_t offset) {

			LARGE_INTEGER x;

			x.QuadPart = offset;

			Offset = x.LowPart;
			OffsetHigh = x.HighPart;
		}
	};

	template <typename T>
	class ThreadPoolIO {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PVOID Overlapped, ULONG IoResult, ULONG_PTR NumberOfBytesTransferred, PTP_IO Io) {
			ThreadPoolIO<T> *io = static_cast<ThreadPoolIO<T>*>(Context);
			io->IoCompletion((OVERLAPPED*)Overlapped, IoResult, (PLARGE_INTEGER)NumberOfBytesTransferred);
		}

		HANDLE m_hFile;
		PTP_IO m_item;

		std::vector<std::shared_ptr<IOOverlapped<T>>> m_ov;
		concurrency::concurrent_queue<std::shared_ptr<IOOverlapped<T>>> m_ov_complete;


		ThreadPoolIO(const ThreadPoolIO&) = delete;
		ThreadPoolIO& operator=(const ThreadPoolIO&) = delete;

		bool IoPending(std::shared_ptr<IOOverlapped<T>>& );
		void IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred);

	public:

		ThreadPoolIO(HANDLE h, ThreadPool &tp = GetMainThreadPool()) : m_hFile(h) {
			m_item = CreateThreadpoolIo(m_hFile, callback, this, tp);

			//std::cout << typeid(*this).name() << "=" << std::to_string((unsigned __int64)this) << std::endl;

			for (auto i = 0; i < GetMainThreadPool().GetMinThreadCount(); i++) {
				m_ov.emplace_back(std::make_shared<IOOverlapped<T>>(i));
				m_ov_complete.push(m_ov[i]);
			}
		}

		~ThreadPoolIO() {
			::CloseThreadpoolIo(m_item);
		}

		bool StartIO(std::shared_ptr<T>& item) {

			std::shared_ptr<IOOverlapped<T>> ov_item = nullptr;

			m_ov_complete.try_pop(ov_item);
			ov_item->m_item = item;

			StartThreadpoolIo(m_item);
			
			DWORD err = ERROR_SUCCESS;
			bool ret = IoPending(ov_item);

			if (!(ret == false && (err = GetLastError()) == ERROR_IO_PENDING)) {
				CancelThreadpoolIo(m_item);
				//this->IoCompletion(0, 0, nullptr);
				return false;
			}
			
			return true;
		};
	};
}



#pragma once

#include "ThreadPool.h"

namespace ThreadPool {

	class ThreadPoolIO {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PVOID Overlapped, ULONG IoResult, ULONG_PTR NumberOfBytesTransferred, PTP_IO Io) {
			ThreadPoolIO *item = static_cast<ThreadPoolIO*>(Context);
			LARGE_INTEGER x;

			x.QuadPart = NumberOfBytesTransferred;

			item->IoCompletion((OVERLAPPED*)Overlapped, IoResult, &x);
		}

		PTP_IO m_item;

		ThreadPoolIO(const ThreadPoolIO&) = delete;
		ThreadPoolIO& operator=(const ThreadPoolIO&) = delete;

	protected:
		
		virtual void IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred) = 0;
		virtual bool IoPending() = 0;

		ThreadPoolIO(HANDLE h, ThreadPool &tp = GetMainThreadPool()) {
			m_item = CreateThreadpoolIo(h, callback, this, tp);
		}

	public:

		virtual ~ThreadPoolIO() {
			CloseThreadpoolIo(m_item);
		}

		void StartIO() {

			StartThreadpoolIo(m_item);

			DWORD err = ERROR_SUCCESS;
			bool ret = IoPending();

			if (ret == false && (err = GetLastError()) != ERROR_IO_PENDING) {
				CancelThreadpoolIo(m_item);
				this->IoCompletion(0, 0, nullptr);
			}
		};
	};
}

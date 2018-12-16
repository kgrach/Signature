#pragma once

#include <functional>
#include "ThreadPool.h"

namespace ThreadPool {

	using fIOComplete = std::function<void(ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred)>;

	class ThreadPoolIO {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PVOID Overlapped, ULONG IoResult, ULONG_PTR NumberOfBytesTransferred, PTP_IO Io) {
			ThreadPoolIO *item = static_cast<ThreadPoolIO*>(Context);
			LARGE_INTEGER x;

			x.QuadPart = NumberOfBytesTransferred;

			item->IoCompletion((OVERLAPPED*)Overlapped, IoResult, &x);
		}
		
		virtual void IoCompletion(OVERLAPPED* Overlapped, ULONG IoResult, PLARGE_INTEGER NumberOfBytesTransferred) {
			return m_fIoComplete(IoResult, NumberOfBytesTransferred);
		}

		PTP_IO m_item;
		fIOComplete	m_fIoComplete;

		ThreadPoolIO(const ThreadPoolIO&) = delete;
		ThreadPoolIO& operator=(const ThreadPoolIO&) = delete;

	protected:
		
		virtual bool IoPending() = 0;

		ThreadPoolIO(HANDLE h, fIOComplete f, ThreadPool &tp = GetMainThreadPool()) : m_fIoComplete(f) {
			m_item = CreateThreadpoolIo(h, callback, this, tp);
		}

	public:

		virtual ~ThreadPoolIO() {
			CloseThreadpoolIo(m_item);
		}

		void StartIo() {

			StartThreadpoolIo(m_item);

			DWORD err = ERROR_SUCCESS;
			bool ret = IoPending();

			if (ret == false && (err = GetLastError()) != ERROR_IO_PENDING) {
				CancelThreadpoolIo(m_item);
				m_fIoComplete(0, nullptr);
			}
		};
	};
}

#pragma once
#include "ThreadPool.h"
#include <memory>

namespace ThreadPool {

	class ThreadPoolWait {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult) {
			ThreadPoolWait *item = static_cast<ThreadPoolWait*>(context);
			item->WaitComplete();
		}

		virtual void WaitComplete() = 0;

		PTP_WAIT m_item;

	protected:

		ThreadPoolWait(ThreadPool &tp = GetMainThreadPool()) {
			m_item = CreateThreadpoolWait(callback, this, tp);
		}

	public:

		virtual ~ThreadPoolWait() {
			CloseThreadpoolWait(m_item);
		}

		void StartWait(HANDLE hWait, FILETIME* timeout) {
			SetThreadpoolWait(m_item, hWait, timeout);
		}
	};
};

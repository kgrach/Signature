#pragma once

#include <shared_mutex>

#include "ThreadPool.h"


namespace ThreadPool {

	class ThreadPoolWork {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK) {
			ThreadPoolWork *item = static_cast<ThreadPoolWork*>(context);
			item->RunWork();
		}

		virtual void RunWork() = 0;

		HANDLE m_hStopEvnt;
		std::shared_mutex m_StartingLock;
		PTP_WORK m_item;

	protected:

		ThreadPoolWork(ThreadPool &tp = GetMainThreadPool()) {
			m_item = CreateThreadpoolWork(callback, this, tp);
		}

	public:

		virtual ~ThreadPoolWork() {
			CloseThreadpoolWork(m_item);
		}

		void StartWork() {

			if (::WaitForSingleObject(m_hStopEvnt, 0) == WAIT_OBJECT_0) {
				return;
			}

			std::shared_lock<std::shared_mutex> lock(m_StartingLock, std::try_to_lock_t());

			if (!lock)
				return;

			SubmitThreadpoolWork(m_item);
		}

		void StopWork() {
			SetEvent(m_hStopEvnt);

			std::unique_lock<std::shared_mutex> lock(m_StartingLock);
			::WaitForThreadpoolWorkCallbacks(m_item, TRUE);
		}
	};
};

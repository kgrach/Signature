#pragma once

#include "ThreadPool.h"

namespace ThreadPool {

	class ThreadPoolWork {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK) {
			ThreadPoolWork *item = static_cast<ThreadPoolWork*>(context);
			item->RunWork();
		}

		virtual void RunWork() = 0;

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
			SubmitThreadpoolWork(m_item);
		}
	};
};

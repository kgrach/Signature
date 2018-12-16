#pragma once

#include <concurrent_queue.h>
#include <functional>

#include "ThreadPool.h"


namespace ThreadPool {

	class WorkItem {

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK) {
			WorkItem *item = static_cast<WorkItem*>(context);
			item->EndWork();
		}

		PTP_WORK m_item;

	protected:

		WorkItem(PVOID context, PTP_CALLBACK_ENVIRON env) {
			m_item = CreateThreadpoolWork(callback, context, env);
		}


	public:

		~WorkItem() {
			CloseThreadpoolWork(m_item);
		}

		void StartWork() {
			SubmitThreadpoolWork(m_item);
		}

		virtual void EndWork() = 0;
	};



	template<typename T, typename F_USER_CALLCBACK = std::function<void(const typename T&)>>
	class SequentialTasks : public WorkItem {

		Concurrency::concurrent_queue<T> m_queue_task;
		F_USER_CALLCBACK m_func;
		unsigned long m_queue_processing;

		void EndWork() {

			T data;

			if (m_queue_task.try_pop(data)) {
				m_func(data);
			}

			if (m_queue_task.empty() == true)
				InterlockedExchange(&m_queue_processing, 0);
			else
				StartWork();
		}

	public:

		SequentialTasks(F_USER_CALLCBACK callback, ThreadPool& tp = GetMainThreadPool()) : WorkItem(this, tp), m_func(callback), m_queue_processing(0) {
		}

		void StartTask(const T& data) {

			m_queue_task.push(data);

			if (InterlockedCompareExchange(&m_queue_processing, 1, 0) == 0)
				StartWork();
		}
	};

	template<typename T, typename F_USER_CALLCBACK>
	class ParallelTasks : public WorkItem {

		Concurrency::concurrent_queue<T> m_queue_processing;
		F_USER_CALLCBACK m_func;

		void EndWork() {

			T data;

			if (m_queue_processing.try_pop(data)) {
				m_func(data);
			}
		}

	public:

		ParallelTasks(F_USER_CALLCBACK callback, ThreadPool& tp = GetMainThreadPool()) : WorkItem(this, tp), m_func(callback) {
		}

		void StartTask(const T& data) {

			m_queue_processing.push(data);
			StartWork();
		}
	};


};

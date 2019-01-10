#pragma once

#include <Windows.h>
#include <atomic>
#include <functional>


namespace ThreadPool {

	class ThreadPool {
		
		PTP_POOL m_tp;
		TP_CALLBACK_ENVIRON m_env;
		PTP_CLEANUP_GROUP m_clean_group;
		unsigned long m_maxThreadCnt, m_minThreadCnt;

		std::atomic<bool> m_is_starting;

	public:
		
		ThreadPool();
		~ThreadPool();

		void StartingWork();
		void StopWork(bool cancel_pending_request = true);


		operator PTP_CALLBACK_ENVIRON();

		unsigned long GetMaxThreadCount();
		unsigned long GetMinThreadCount();

		bool IsStarted();
	};

	ThreadPool& GetMainThreadPool();
}
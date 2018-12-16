#pragma once

#include <Windows.h>

namespace ThreadPool {

	class ThreadPool {
		PTP_POOL m_tp;
		TP_CALLBACK_ENVIRON m_env;
		unsigned long m_maxThreadCnt, m_minThreadCnt;

		bool m_is_starting;
	public:
		
		ThreadPool() : m_is_starting(false) {
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);

			m_maxThreadCnt = 2 * sysinfo.dwNumberOfProcessors;
			m_minThreadCnt = sysinfo.dwNumberOfProcessors;
		}

		void StartingWork() {
			m_tp = CreateThreadpool(nullptr);
			InitializeThreadpoolEnvironment(&m_env);

			SetThreadpoolCallbackPriority(&m_env, TP_CALLBACK_PRIORITY_NORMAL);
			SetThreadpoolCallbackPool(&m_env, m_tp);

			SetThreadpoolThreadMaximum(m_tp, m_maxThreadCnt);
			SetThreadpoolThreadMinimum(m_tp, m_minThreadCnt);

			m_is_starting = true;
		}

		unsigned long GetMaxThreadCount() {
			return m_maxThreadCnt;
		}

		unsigned long GetMinThreadCount() {
			return m_minThreadCnt;
		}

		bool IsStarted() {
			return m_is_starting;
		}

		~ThreadPool() {

			if (m_is_starting) {
				DestroyThreadpoolEnvironment(&m_env);
				CloseThreadpool(m_tp);
			}
		}

		operator PTP_CALLBACK_ENVIRON() {
			return &m_env;
		}
	};

	ThreadPool& GetMainThreadPool();
}
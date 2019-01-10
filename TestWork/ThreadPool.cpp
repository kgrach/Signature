#include "ThreadPool.h"

#include <iostream>
#include <string>

namespace ThreadPool {

	ThreadPool& GetMainThreadPool() {
		static ThreadPool pool;
		return pool;
	}

	ThreadPool::ThreadPool() : m_is_starting(false) {

		SYSTEM_INFO sysinfo;
		::GetSystemInfo(&sysinfo);

		m_maxThreadCnt = 2 * sysinfo.dwNumberOfProcessors;
		m_minThreadCnt = sysinfo.dwNumberOfProcessors;
	}

	void ThreadPool::StartingWork() {

		m_tp = ::CreateThreadpool(nullptr);
		::InitializeThreadpoolEnvironment(&m_env);

		::SetThreadpoolCallbackPriority(&m_env, TP_CALLBACK_PRIORITY_NORMAL);
		::SetThreadpoolCallbackPool(&m_env, m_tp);

		::SetThreadpoolThreadMaximum(m_tp, m_maxThreadCnt);
		::SetThreadpoolThreadMinimum(m_tp, m_minThreadCnt);

		m_clean_group = ::CreateThreadpoolCleanupGroup();

		::SetThreadpoolCallbackCleanupGroup(&m_env, m_clean_group, nullptr);
		m_is_starting = true;
	}

	void ThreadPool::StopWork(bool cancel_pending_request) {
		::CloseThreadpoolCleanupGroupMembers(m_clean_group, cancel_pending_request, nullptr);
	}

	unsigned long ThreadPool::GetMaxThreadCount() {
		return m_maxThreadCnt;
	}

	unsigned long ThreadPool::GetMinThreadCount() {
		return m_minThreadCnt;
	}

	bool ThreadPool::IsStarted() {
		return m_is_starting;
	}

	ThreadPool::~ThreadPool() {

		if (m_is_starting) {
			::CloseThreadpoolCleanupGroup(m_clean_group);
			::DestroyThreadpoolEnvironment(&m_env);
			::CloseThreadpool(m_tp);
		}
	}

	ThreadPool::operator PTP_CALLBACK_ENVIRON() {
		return &m_env;
	}



}
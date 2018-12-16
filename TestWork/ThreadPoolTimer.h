#pragma once

#include "ThreadPool.h"

#define TIMEOUT_100_MS 100

namespace ThreadPool {

	class TimerItem {

		union FILETIME64 {
			INT64 quad;
			FILETIME ft;
		};

		FILETIME relative_time(DWORD milliseconds) {
			FILETIME64 ft = { -static_cast<INT64>(milliseconds) * 10000 };
			return ft.ft;
		}

		static void CALLBACK callback(PTP_CALLBACK_INSTANCE Instance, PVOID context, PTP_TIMER Timer) {
			TimerItem *item = static_cast<TimerItem*>(context);
			item->Timeout();
		}

	protected:

		PTP_TIMER m_item;

		TimerItem(long time_period_ms, void* context, PTP_CALLBACK_ENVIRON env) {
			m_item = CreateThreadpoolTimer(callback, context, env);
			SetThreadpoolTimer(m_item, &relative_time(time_period_ms), time_period_ms, 0);
		}

	public:

		~TimerItem() {
			CloseThreadpoolTimer(m_item);
		}

		virtual void Timeout() = 0;
	};



	template<typename F_USER_CALLCBACK>
	class Timer : public TimerItem {

		F_USER_CALLCBACK m_func;

		void Timeout(void) {
			m_func();
		}

	public:

		Timer(F_USER_CALLCBACK callback, long time_period_ms, ThreadPool& tp = GetMainThreadPool()) : TimerItem(time_period_ms, this, tp), m_func(callback) {
		}
	};

};


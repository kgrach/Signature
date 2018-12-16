#include "ThreadPool.h"

namespace ThreadPool {

	ThreadPool& GetMainThreadPool() {
		static ThreadPool pool;
		return pool;
	}

}
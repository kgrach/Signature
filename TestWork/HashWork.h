#pragma once

#include "ThreadPoolWork.h"

#include <memory>
#include <vector>
#include <functional>

using fHashCallback = std::function<void(size_t, size_t, std::unique_ptr<std::vector<unsigned char>>, std::unique_ptr<std::vector<unsigned char>>)>;

class HashWork : public ThreadPool::ThreadPoolWork {
	
	size_t m_num;
	std::shared_ptr<BCRYPT_ALG_HANDLE>	m_hAlg;
	std::shared_ptr<BCRYPT_HASH_HANDLE> m_hHash;

	size_t m_offset;
	std::unique_ptr<std::vector<unsigned char>>	m_buff;
	std::unique_ptr<std::vector<unsigned char>>	m_hash;

	std::unique_ptr<std::vector<unsigned char>>	m_localhash;

	fHashCallback m_fCallbackClient;

	DWORD                   cbData = 0, cbHash = 0, cbHashObject = 0;
	PBYTE                   pbHashObject = NULL;
	PBYTE                   pbHash = NULL;

	void RunWork();

public:

	HashWork(size_t num, std::shared_ptr<BCRYPT_ALG_HANDLE>& h_Alg, fHashCallback fCallback);
	bool InitializeWork();

	void StartHashWork(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash);
};



/*
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
*/
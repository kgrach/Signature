#include "HashMngr.h"
#include "ISettings.h"
#include "ThreadPoolWork.h"
#include <winternl.h>

using namespace ThreadPool;

static auto fFreeAlg = [](BCRYPT_ALG_HANDLE* p) {
	::BCryptCloseAlgorithmProvider(*p, 0);
};

HashMngr::HashMngr(std::shared_ptr<ISettings>& settings, fHashClientCallback fCallback) : m_settings(settings), m_fClientCallback(fCallback) {

	m_hAlg = std::shared_ptr<BCRYPT_ALG_HANDLE>(new BCRYPT_ALG_HANDLE(NULL), fFreeAlg);
}

bool HashMngr::InitializeWork() {

	NTSTATUS status; 

	if (!(::BCryptOpenAlgorithmProvider(m_hAlg.get(), BCRYPT_MD5_ALGORITHM, NULL, 0) >=0))
		return false;

	auto f = [this](size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
		return HashComplete(taskNum, offset, std::move(buff), std::move(hash));
	};

	for (auto i = 0ul; i < GetMainThreadPool().GetMinThreadCount(); i++) {
		m_vecHashTask.emplace_back(std::move(std::make_unique<HashWork>(i, m_hAlg, f)));
		m_vecHashTaskComplete.push(i);
		m_vecHashTask[i]->InitializeWork();

		m_part2.push(HashPart2(std::move(std::make_unique<std::vector<unsigned char>>())));
	}
	//new memory



	return true;
}

void HashMngr::HashComplete(size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
	
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vecHashTaskComplete.push(taskNum);
	}
	
	m_fClientCallback(offset, std::move(buff), std::move(hash));
}

bool HashMngr::StartingHash(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
	
	size_t num = m_vecHashTaskComplete.top();

	m_vecHashTask[num]->StartHashWork(offset, std::move(buff), std::move(hash));

	m_vecHashTaskComplete.pop();

	return true;
}

bool HashMngr::Hashing1(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff) {

	std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_part2.empty()) {

		size_t taskNum = m_vecHashTaskComplete.top();
		HashPart2 &part2 = m_part2.top();

		StartingHash(offset, std::move(buff), std::move(part2.hash));

		m_part2.pop();

		return true;
	}

	m_part1.push(HashPart1(offset, std::move(buff)));

	return true;
}

bool HashMngr::Hashing2(std::unique_ptr<std::vector<unsigned char>> hash) {
	
	std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_part1.empty()) {

		size_t taskNum = m_vecHashTaskComplete.top();
		HashPart1 &part1 = m_part1.top();

		StartingHash(part1.offset, std::move(part1.buff), std::move(hash));

		m_part1.pop();

		return true;
	}

	m_part2.push(HashPart2(std::move(hash)));
	
	return true;
}

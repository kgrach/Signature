#include "HashMngr.h"
#include "ISettings.h"
#include "ThreadPoolWork.h"

using namespace ThreadPool;

static auto fFreeAlg = [](BCRYPT_ALG_HANDLE* p) {
	::BCryptCloseAlgorithmProvider(*p, 0);
};

HashMngr::HashMngr(std::shared_ptr<ISettings>& settings, fHashCallback fCallback) : m_settings(settings), m_fClientCallback(fCallback) {

	m_hAlg = std::shared_ptr<BCRYPT_ALG_HANDLE>(new BCRYPT_ALG_HANDLE(NULL), fFreeAlg);
}

bool HashMngr::InitializeWork() {

	if (!::BCryptOpenAlgorithmProvider(m_hAlg.get(), BCRYPT_MD5_ALGORITHM, NULL, 0))
		return false;

	for (auto i = 0ul; i < GetMainThreadPool().GetMinThreadCount(); i++) {
		m_vecHashTask.emplace_back(std::move(std::make_unique<HashWork>(m_hAlg, m_fClientCallback)));
		m_vecHashTask[i]->InitializeWork();

	}
	//new memory



	return true;
}

bool HashMngr::Hashing1(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff) {
	return true;
}

bool HashMngr::Hashing2(std::unique_ptr<std::vector<unsigned char>> hash) {
	return true;
}

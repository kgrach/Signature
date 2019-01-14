#include "HashMngr.h"
#include "ISettings.h"
#include "ItemConveyer.h"
#include <winternl.h>

using namespace ThreadPool;


HashMngr::HashMngr(std::shared_ptr<ISettings>& settings) : m_settings(settings){
}

bool HashMngr::InitializeWork() {

	NTSTATUS status; 

	m_hAlg = std::shared_ptr<BCRYPT_ALG_HANDLE>(new BCRYPT_ALG_HANDLE(NULL), [](BCRYPT_ALG_HANDLE* p) { ::BCryptCloseAlgorithmProvider(*p, 0); delete p; });

	if (!(::BCryptOpenAlgorithmProvider(m_hAlg.get(), BCRYPT_MD5_ALGORITHM, NULL, 0) >=0))
		return false;

	return true;
}



bool HashMngr::Hashing(std::shared_ptr<IItemHash>& item) {

	std::unique_ptr<std::vector<unsigned char>>	m_localhash;

	DWORD                   cbData = 0, cbHash = 0, cbHashObject = 0;
	PBYTE                   pbHashObject = NULL;
	PBYTE                   pbHash = NULL;

	std::shared_ptr<BCRYPT_HASH_HANDLE> m_hHash = std::shared_ptr<BCRYPT_HASH_HANDLE>(new BCRYPT_HASH_HANDLE(NULL), [](BCRYPT_ALG_HANDLE* p) { ::BCryptDestroyHash(*p); delete p; });

	NTSTATUS status;
	status = ::BCryptGetProperty(*m_hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);

	if (!(status >= 0))
		return false;

	m_localhash = std::move(std::make_unique<std::vector<unsigned char>>(cbHashObject));

	status = ::BCryptGetProperty(*m_hAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);
	if (!(status >= 0))
		return false;

	item->GetHash().resize(cbHash);

	if (!(::BCryptCreateHash(*m_hAlg, m_hHash.get(), m_localhash->data(), m_localhash->size(), NULL, 0, 0) >= 0))
		return false;

	if (!(::BCryptHashData(*m_hHash, item->GetBuff().data(), item->GetBuff().size(), 0) >= 0))
		return false;

	if (!(::BCryptFinishHash(*m_hHash, item->GetHash().data(), item->GetHash().size(), 0) >= 0))
		return false;

	return true;
}

#include "HashWork.h"

static auto fDelHash = [](BCRYPT_ALG_HANDLE* p) {
	::BCryptDestroyHash(*p);
};

HashWork::HashWork(std::shared_ptr<BCRYPT_ALG_HANDLE>& hAlg, fHashCallback fCallback) : m_hAlg(hAlg), m_fCallbackClient(fCallback) {

	m_hHash = std::shared_ptr<BCRYPT_HASH_HANDLE>(new BCRYPT_HASH_HANDLE(NULL), fDelHash);
}

bool HashWork::InitializeWork() {

	if (!::BCryptGetProperty(m_hAlg.get(), BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0))
		return false;

	if (!::BCryptGetProperty(m_hAlg.get(), BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0))
		return false;

	return true;
}


void HashWork::RunWork() {

	/*if (!::BCryptCreateHash(m_hAlg.get(), m_hHash.get(), pbHashObject, bHashObject, NULL, 0, 0))
		return false;	

	if (!::BCryptHashData(m_hHash.get(), (PBYTE)rgbMsg, sizeof(rgbMsg), 0))
		return false;

	if (!::BCryptFinishHash(m_hHash.get(), pbHash, cbHash, 0))
		return false;

	m_fCallbackClient();*/
}

void HashWork::StartHashWork(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
	
	m_offset = offset;
	m_buff = std::move(buff);
	m_hash = std::move(hash);

	StartWork();
}
#include "HashWork.h"

static auto fDelHash = [](BCRYPT_ALG_HANDLE* p) {
	::BCryptDestroyHash(*p);
};

HashWork::HashWork(size_t num, std::shared_ptr<BCRYPT_ALG_HANDLE>& hAlg, fHashCallback fCallback) : m_num(num), m_hAlg(hAlg), m_fCallbackClient(fCallback) {

	m_hHash = std::shared_ptr<BCRYPT_HASH_HANDLE>(new BCRYPT_HASH_HANDLE(NULL), fDelHash);
}

bool HashWork::InitializeWork() {
	NTSTATUS status;
	status = ::BCryptGetProperty(*m_hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);

	if (!(status >= 0 ))
		return false;

	m_localhash =std::move(std::make_unique<std::vector<unsigned char>>(cbHashObject));

	status = ::BCryptGetProperty(*m_hAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);
	if (!(status >=0 ))
		return false;

	return true;
}


void HashWork::RunWork() {

	if (!(::BCryptCreateHash(*m_hAlg, m_hHash.get(), m_localhash->data(), m_localhash->size(), NULL, 0, 0) >= 0))
		return;	

	if (!(::BCryptHashData(*m_hHash, m_buff->data(), m_buff->size(), 0) >= 0))
		return;

	if (!(::BCryptFinishHash(*m_hHash, m_hash->data(), m_hash->size(), 0) >= 0))
		return;

	m_fCallbackClient(m_num, m_offset, std::move(m_buff), std::move(m_hash));
}

void HashWork::StartHashWork(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash) {
	
	m_offset = offset;
	m_buff = std::move(buff);
	m_hash = std::move(hash);

	m_hash->resize(cbHash);

	StartWork();
}


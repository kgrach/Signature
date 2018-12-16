#include "HashMngr.h"
#include "ScopeAlgorithmHandle.h"

#include <bcrypt.h>

HashMngr::HashMngr() {

}

bool HashMngr::InitializeWork() {
	
	


	::BCryptOpenAlgorithmProvider(m_hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0);
	return true;
}

bool HashMngr::Hashing(std::unique_ptr<std::vector<unsigned char>>) {
	return true;
}

void HashMngr::HashComplete(size_t offset, std::unique_ptr<std::vector<unsigned char>> data) {

}

#pragma once

#include <memory>
#include <vector>

#include "ScopeAlgorithmHandle.h"

class HashMngr{

	ScopeAlgorithmHandle m_hAlg;

	void HashComplete(size_t offset, std::unique_ptr<std::vector<unsigned char>> data);

public:
	HashMngr();

	bool InitializeWork();
	bool Hashing(std::unique_ptr<std::vector<unsigned char>>);

};
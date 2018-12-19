#pragma once

#include <memory>
#include <vector>
#include <windows.h>
#include <bcrypt.h>

#include "HashWork.h"

class ISettings;

//using fHashCallback = std::function<void(size_t, std::unique_ptr<std::vector<unsigned char>>, std::unique_ptr<std::vector<unsigned char>>)>;

class HashMngr{
	
	std::shared_ptr<ISettings>				m_settings;
	std::shared_ptr<BCRYPT_ALG_HANDLE>		m_hAlg;
	std::vector<std::unique_ptr<HashWork>>	m_vecHashTask;

	//void HashComplete(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash);

	fHashCallback m_fClientCallback;

public:
	HashMngr(std::shared_ptr<ISettings>& settings, fHashCallback fCallback);

	bool InitializeWork();
	bool Hashing1(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff);
	bool Hashing2(std::unique_ptr<std::vector<unsigned char>> hash);
};
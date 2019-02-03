#pragma once

#include <windows.h>
#include <bcrypt.h>
#include <memory>

//#include "HashWork.h"

class ISettings;
class IItemHash;

class HashMngr{
	
	std::shared_ptr<ISettings>				m_settings;
	std::shared_ptr<BCRYPT_ALG_HANDLE>		m_hAlg;
	
public:
	HashMngr(std::shared_ptr<ISettings>& settings);

	bool InitializeWork();
	bool Hashing(std::shared_ptr<IItemHash>&);
};


#pragma once

#include "ScopeResource.h"
#include <windows.h>
#include <bcrypt.h>

struct BCRYPT_ALG_HANDLE_Type {
};

template<>
void TScopeResource<BCRYPT_ALG_HANDLE, BCRYPT_ALG_HANDLE_Type>::FreeResource() {
	if (m_Res != default_value)
		::BCryptCloseAlgorithmProvider(m_Res, 0);
}

template <>
BCRYPT_ALG_HANDLE TScopeResource<BCRYPT_ALG_HANDLE, BCRYPT_ALG_HANDLE_Type>::default_value = NULL;

template <typename T, typename U>
class TScopeResourceEx : public TScopeResource<T, U> {
public:
	operator T*() {
		return &m_Res;
	}
};

using ScopeAlgorithmHandle = TScopeResourceEx<BCRYPT_ALG_HANDLE, BCRYPT_ALG_HANDLE_Type>;
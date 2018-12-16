#pragma once

#include "ScopeResource.h"
#include <windows.h>

struct HANDLE_Type {
};

template<>
void TScopeResource<HANDLE, HANDLE_Type>::FreeResource() {
	if (m_Res != default_value)
		::CloseHandle(m_Res);
}

template <>
HANDLE TScopeResource<HANDLE, HANDLE_Type>::default_value = INVALID_HANDLE_VALUE;

using ScopeHandle = TScopeResource<HANDLE, HANDLE_Type>;
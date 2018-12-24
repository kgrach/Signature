#pragma once

#include <memory>
#include <windows.h>


template <typename T, typename Type>
class TSharedResource : public std::enable_shared_from_this<SharedResource<T>> {
	
	T			m_hRes;
	static T	default_value;


public:

	TSharedResource(T&& rhs) {
		//m_hRes = rhs.
	}
	TSharedResource& operator=(T&& rhs);
	
	operator bool() const {
		return m_Res != default_value ? true : false;
	}

	operator T() {
		return m_hRes;
	}

};
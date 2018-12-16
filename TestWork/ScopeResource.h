#pragma once

template <class T, class U>
class TScopeResource : private U {

	static T default_value;

	void FreeResource() {
	}

	TScopeResource(const TScopeResource& rhs) = delete;
	TScopeResource& operator=(const TScopeResource& rhs) = delete;

protected:

	T m_Res;

public:

	TScopeResource() : m_Res(default_value) {
	}

	TScopeResource(const T& res) : m_Res(res) {
	}

	operator T() const{
		return m_Res;
	}

	operator bool() const {
		return m_Res != default_value ? true : false;
	}

	TScopeResource& operator=(const T res) {
		
		FreeResource();
		m_Res = res;
		
		return *this;
	}

	~TScopeResource() {
		FreeResource();
	}
};
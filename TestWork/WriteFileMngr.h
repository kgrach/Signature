#pragma once

#include <memory>
#include "ScopeHandle.h"

class ISettings;
class IItemWrite;

namespace ThreadPool {
	template <typename T>
	class ThreadPoolIO;
}

class WriteFileMngr {

	std::shared_ptr<ISettings> m_settings;

	ScopeHandle	m_hfileDest;

	std::shared_ptr<ThreadPool::ThreadPoolIO<IItemWrite>>		m_io;

public:
	WriteFileMngr(std::shared_ptr<ISettings>& settings);

	bool Writing(std::shared_ptr<IItemWrite>& item);
	bool InitializeWork(size_t size);
};
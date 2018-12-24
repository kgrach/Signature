#pragma once

#include <memory>
#include <atomic>
#include "ScopeHandle.h"

class ISettings;
class IItemRead;

namespace ThreadPool {
	template <typename T>
	class ThreadPoolIO;
}


class ReadFileMngr {

	std::shared_ptr<ISettings> m_settings;

	ScopeHandle m_hfileSrc;
	LARGE_INTEGER m_SrcFileSize;

	std::atomic<unsigned __int64>	m_offset;
	std::shared_ptr<ThreadPool::ThreadPoolIO<IItemRead>>		m_io;

public:
	ReadFileMngr(std::shared_ptr<ISettings>& settings);

	bool Reading(std::shared_ptr<IItemRead>& item);
	bool InitializeWork();
};
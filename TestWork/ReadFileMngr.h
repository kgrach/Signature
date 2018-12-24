#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <functional>

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
	//std::shared_ptr<HANDLE> m_hfileSrc;

	LARGE_INTEGER m_SrcFileSize;

	std::atomic<unsigned __int64>	m_offset;
	std::shared_ptr<ThreadPool::ThreadPoolIO<IItemRead>>		m_io;

	//std::function<void(size_t, std::unique_ptr<std::vector<unsigned char>>)> m_fCallback;
	
	//void ReadComplete(size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> data);

public:
	ReadFileMngr(std::shared_ptr<ISettings>& settings);

	bool Reading(std::shared_ptr<IItemRead>& item);
	bool InitializeWork();
};
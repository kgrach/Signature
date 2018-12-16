#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <concurrent_queue.h>

#include "ScopeHandle.h"
#include "WriteFileIO.h"

class ISettings;

class WriteFileMngr {

	std::shared_ptr<ISettings> m_settings;

	ScopeHandle	m_hfileDest;

	std::atomic<unsigned __int64> m_offset;

	std::vector<std::shared_ptr<WriteFileIOTask>> m_vecIOTask;
	concurrency::concurrent_queue<std::shared_ptr<WriteFileIOTask>> m_vecIOTaskCompleted;


	std::function<void(std::unique_ptr<std::vector<unsigned char>>)> m_fCallback;


	void WriteComplete(size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> data);

public:
	WriteFileMngr(std::shared_ptr<ISettings>& settings, std::function<void(std::unique_ptr<std::vector<unsigned char>>)>);

	bool Writing(std::unique_ptr<std::vector<unsigned char>>&& buff);
	bool InitializeWork();
};
#pragma once

#include <memory>
#include <vector>
#include <concurrent_queue.h>

#include "ReadFileMngr.h"
#include "WriteFileMngr.h"
#include "HashMngr.h"
#include "ItemConveyer.h"

class ISettings;

class WorkManager {
	std::shared_ptr<ISettings> m_settings;
	
	std::unique_ptr<ReadFileMngr>	m_ReadFileMngr;
	std::unique_ptr<HashMngr>		m_HashMngr;
	std::unique_ptr<WriteFileMngr>	m_WriteFileMngr;

	std::atomic_long m_thread_used = {0};

	std::vector<std::shared_ptr<ItemConveyer>> m_Items;
	concurrency::concurrent_queue<std::shared_ptr<ItemConveyer>> m_ItemsComplete;
	
	void ReadCompleteChunck(std::shared_ptr<ItemConveyer>& item);
	void WriteCompleteChunck(std::shared_ptr<ItemConveyer>& item);

public:
	WorkManager(std::shared_ptr<ISettings>&& settings);
	bool StartWork() noexcept;
	void StopWork();
	bool IsWorkDone();
};

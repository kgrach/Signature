#pragma once

#include <memory>
#include <vector>

#include "ReadFileMngr.h"
#include "WriteFileMngr.h"
#include "HashMngr.h"

class ISettings;

class WorkManager {
	std::shared_ptr<ISettings> m_settings;
	
	std::unique_ptr<ReadFileMngr>	m_ReadFileMngr;
	std::unique_ptr<WriteFileMngr>	m_WriteFileMngr;
	std::unique_ptr<HashMngr>		m_HashMngr;
	
	void ReadCompleteChunck(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff);
	void HashCompleteChunck(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash);
	void WriteCompleteChunck(std::unique_ptr<std::vector<unsigned char>> hash);

public:
	WorkManager(std::shared_ptr<ISettings>&& settings);
	bool StartWork() noexcept;
	void StopWork();
};

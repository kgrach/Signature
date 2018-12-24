#pragma once

#include <experimental\filesystem>

#include "ISettings.h"


namespace fs = std::experimental::filesystem;

class Settings : public ISettings {

	fs::path m_InFile, m_OutFile;
	size_t m_ChunckSize;
	bool m_arg_parsed;

	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;

	Settings(Settings&&) = delete;
	Settings& operator=(Settings&&) = delete;


	std::string GetInFileName() const;
	std::string GetOutFileName() const;
	size_t GetChunkSize() const;

public:
	Settings() = default;

	~Settings() {

	}

	std::shared_ptr<ISettings> GetInterface();
	bool ParseConsoleArgs(const int argc, const char * const* const argv) noexcept;
};

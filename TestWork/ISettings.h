#pragma once

#include <memory>
#include <string>

class ISettings : public std::enable_shared_from_this<ISettings> {

public:

	virtual std::string GetInFileName() const = 0;
	virtual std::string GetOutFileName() const = 0;
	virtual size_t GetChunkSize() const = 0;

	virtual ~ISettings() {}
};

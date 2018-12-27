#pragma once


#include <memory>
#include <vector>
#include <functional>

#include <windows.h>

class IItemRead {

public:

	virtual void SetOffset(size_t) = 0;
	virtual size_t GetOffset() const  = 0;
	virtual std::vector<unsigned char>& GetBuff() = 0;

	virtual void CallbackRead(void) = 0;
};


class IItemHash {

public:

	virtual std::vector<unsigned char>& GetBuff() = 0;
	virtual std::vector<unsigned char>& GetHash() = 0;

};

class IItemWrite {

public:
	 
	virtual void SetOffset(size_t) = 0;
	virtual size_t GetOffset() const = 0;
	virtual std::vector<unsigned char>& GetHash() = 0;
	virtual std::vector<unsigned char>& GetBuff() = 0;
	virtual void CallbackWrite(void) = 0;
};

class ItemConveyer : public IItemRead, public IItemHash, public IItemWrite, public std::enable_shared_from_this<ItemConveyer> {

	size_t						m_offset;
	std::vector<unsigned char>	m_buff;
	std::vector<unsigned char>	m_hash;

	std::function<void(void)> m_fCallbackRead;
	std::function<void(void)> m_fCallbackWrite;

	void SetOffset(size_t);
	size_t GetOffset() const;
	std::vector<unsigned char>& GetBuff();
	std::vector<unsigned char>& GetHash();
	void CallbackRead(void);
	void CallbackWrite(void);

public:

	~ItemConveyer(){

	}

	std::shared_ptr<IItemRead> GetItemReadIface(std::function<void(void)>&& f);
	std::shared_ptr<IItemHash> GetItemHashIface();
	std::shared_ptr<IItemWrite> GetItemWriteIface(std::function<void(void)>&& f);
};



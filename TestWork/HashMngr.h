#pragma once

#include <memory>
#include <vector>
#include <windows.h>
#include <bcrypt.h>
#include <stack>
#include <mutex>

#include "HashWork.h"

class ISettings;

using fHashClientCallback = std::function<void(size_t, std::unique_ptr<std::vector<unsigned char>>, std::unique_ptr<std::vector<unsigned char>>)>;

class HashMngr{
	
	struct HashPart1 {

		size_t offset; 
		std::unique_ptr<std::vector<unsigned char>> buff;

		HashPart1(size_t o, std::unique_ptr<std::vector<unsigned char>> b) : offset(0), buff(std::move(b)) {
		}

		HashPart1(HashPart1&& rhs) {
			this->operator=(std::move(rhs));
		}

		HashPart1& operator=(HashPart1&& rhs) {
			offset = rhs.offset;
			buff = std::move(rhs.buff);
			return *this;
		}
	};

	struct HashPart2 {
		std::unique_ptr<std::vector<unsigned char>> hash;

		HashPart2(std::unique_ptr<std::vector<unsigned char>> h) : hash(std::move(h)) {
		}

		HashPart2(HashPart2&& rhs) {
			this->operator=(std::move(rhs));
		}

		HashPart2& operator=(HashPart2&& rhs) {
			hash = std::move(rhs.hash);
			return *this;
		}
	};

	
	std::shared_ptr<ISettings>				m_settings;
	std::shared_ptr<BCRYPT_ALG_HANDLE>		m_hAlg;
	
	std::vector<std::unique_ptr<HashWork>>	m_vecHashTask;
	std::stack<size_t>						m_vecHashTaskComplete;


	std::stack<HashPart1> m_part1;
	std::stack<HashPart2> m_part2;

	std::mutex m_mutex;

	fHashClientCallback m_fClientCallback;

	void HashComplete(size_t taskNum, size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash);
	bool StartingHash(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff, std::unique_ptr<std::vector<unsigned char>> hash);

public:
	HashMngr(std::shared_ptr<ISettings>& settings, fHashClientCallback fCallback);

	bool InitializeWork();
	bool Hashing1(size_t offset, std::unique_ptr<std::vector<unsigned char>> buff);
	bool Hashing2(std::unique_ptr<std::vector<unsigned char>> hash);
};
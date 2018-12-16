#include "Settings.h"

#include <iostream>
#include <exception>

using namespace std;

shared_ptr<ISettings> Settings::GetInterface() {
	return shared_from_this();
}

string Settings::GetInFileName() const {
	return m_InFile.string();
}

string Settings::GetOutFileName() const {
	return m_OutFile.string();
}

size_t Settings::GetChunkSize() const {
	return m_ChunckSize;
}

bool Settings::ParseConsoleArgs(const int argc, const char * const* const argv) noexcept {

	bool ret_val = false;

	try {

		if (argc > 1) {
			m_InFile = argv[1];

			if (!fs::exists(m_InFile))
				throw exception("Invalid input file");
		}
		else {
			throw exception("Invalid argument 1");
		}

		if (argc > 2) {
			m_OutFile = argv[2];
		}
		else {
			throw exception("Invalid argument 2");
		}

		if (argc > 3) {

			m_ChunckSize = stoull(argv[3]);
		}
		else {
			m_ChunckSize = 1024 * 1024;
		}

		ret_val = true;
	}
	catch (exception& e) {
		cout << "Error parse console arguments: " << e.what() << endl;
	}

	return ret_val;
}
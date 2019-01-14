#include "WriteFileMngr.h"
#include "ISettings.h"
#include "ItemConveyer.h"
#include "WriteFileIO.h"

WriteFileMngr::WriteFileMngr(std::shared_ptr<ISettings>& settings) : m_settings(settings) {

}

bool WriteFileMngr::InitializeWork(size_t size) {

	m_hfileDest = std::shared_ptr<HANDLE>(new HANDLE(INVALID_HANDLE_VALUE), [](HANDLE* p) { ::CloseHandle(*p); delete p; });

	*m_hfileDest = ::CreateFile(m_settings->GetOutFileName().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

	if (*m_hfileDest == INVALID_HANDLE_VALUE)
		return false;

	LARGE_INTEGER _size;
	_size.QuadPart = size;

	SetFilePointerEx(*m_hfileDest, _size, NULL, FILE_BEGIN);
	SetEndOfFile(*m_hfileDest);

	m_io = std::move(std::make_shared<ThreadPool::ThreadPoolIO<IItemWrite>>(m_hfileDest));

	return true;
}

bool WriteFileMngr::Writing(std::shared_ptr<IItemWrite>& item) {

	item->SetOffset(item->GetOffset() / m_settings->GetChunkSize() * item->GetHash().size());

	return m_io->StartIO(item);
}
#include "WriteFileMngr.h"
#include "ISettings.h"
#include "ItemConveyer.h"
#include "WriteFileIO.h"

WriteFileMngr::WriteFileMngr(std::shared_ptr<ISettings>& settings) : m_settings(settings) {

}

bool WriteFileMngr::InitializeWork() {

	m_hfileDest = ::CreateFile(m_settings->GetOutFileName().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

	if (!m_hfileDest)
		return false;

	m_io = std::move(std::make_shared<ThreadPool::ThreadPoolIO<IItemWrite>>(m_hfileDest));

	return true;
}

bool WriteFileMngr::Writing(std::shared_ptr<IItemWrite>& item) {


	item->SetOffset(item->GetOffset()/ item->GetHash().size());

	return m_io->StartIO(item);
}
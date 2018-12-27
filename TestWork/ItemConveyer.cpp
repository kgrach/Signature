#include "ItemConveyer.h"


std::shared_ptr<IItemRead> ItemConveyer::GetItemReadIface(std::function<void(void)>&& f) {
	m_fCallbackRead = std::move(f);
	return shared_from_this();
}

std::shared_ptr<IItemHash> ItemConveyer::GetItemHashIface() {
	return shared_from_this();
}

std::shared_ptr<IItemWrite> ItemConveyer::GetItemWriteIface(std::function<void(void)>&& f) {
	m_fCallbackWrite = std::move(f);
	return shared_from_this();
}

std::vector<unsigned char>& ItemConveyer::GetBuff() {
	return m_buff;
}

std::vector<unsigned char>& ItemConveyer::GetHash() {
	return m_hash;
}

void ItemConveyer::SetOffset(size_t offset) {
	m_offset = offset;
}

size_t ItemConveyer::GetOffset() const {
	return m_offset;
}

void ItemConveyer::CallbackRead(void) {
	return m_fCallbackRead();
}
void ItemConveyer::CallbackWrite(void) {
	return m_fCallbackWrite();
}
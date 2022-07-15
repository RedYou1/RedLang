#include "Return.h"
#include "MemoryObject.h"

DTO::Return::Return(std::wstring name) : m_name(name) { }

DTO::CommandReturn* DTO::Return::exec(MemoryObject& mem) {
	return new CommandReturn(mem.containKey(m_name) ? mem.get(m_name) : new NullObject(), true, false);
}

DTO::Command* DTO::Return::clone()
{
	return new Return(m_name);
}

#include "Return.h"
#include "MemoryObject.h"

DTO::Return::Return(std::wstring name, Instanciable* returnType) : m_name(name), m_returnType(returnType) { }

DTO::CommandReturn* DTO::Return::exec(MemoryObject& mem) {
	return new CommandReturn(mem.get(m_name), true, false);
}

DTO::Command* DTO::Return::clone()
{
	return new Return(m_name, m_returnType);
}

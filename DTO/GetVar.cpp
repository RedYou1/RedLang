#include "GetVar.h"

DTO::GetVar::GetVar(Object* o, std::wstring name)
	: m_o(o), m_name(name)
{
	m_o->addRef();
}

DTO::GetVar::~GetVar()
{
	m_o->removeRef();
}

DTO::CommandReturn* DTO::GetVar::exec(MemoryObject& mem)
{
	IObject* cc{ m_o->get(m_name) };
	return new CommandReturn(cc, true, false);
}

DTO::Command* DTO::GetVar::clone()
{
	return new GetVar((Object*)m_o->clone(), m_name);
}

#include "GetStaticVar.h"
#include "MemoryStatVar.h"
#include "StatVar.h"

DTO::GetStaticVar::GetStaticVar(Class* c, std::wstring name)
	:m_c(c), m_name(name)
{
}

DTO::CommandReturn* DTO::GetStaticVar::exec(MemoryObject& mem)
{
	return new CommandReturn(m_c->getStatVars()->get(m_name)->GetValue(), false, false);
}

DTO::Command* DTO::GetStaticVar::clone()
{
	return new GetStaticVar(m_c, m_name);
}

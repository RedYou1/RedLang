#include "ReturnObj.h"

DTO::ReturnObj::ReturnObj(IObject* cmd)
	:m_cmd(cmd)
{
	if (m_cmd == nullptr)
		throw "??";
	m_cmd->addRef();
}

DTO::ReturnObj::~ReturnObj()
{
	m_cmd->removeRef();
}

DTO::CommandReturn* DTO::ReturnObj::exec(MemoryObject& mem)
{
	return new CommandReturn(m_cmd, true, false);
}

DTO::Command* DTO::ReturnObj::clone()
{
	return new ReturnObj(m_cmd->clone());
}
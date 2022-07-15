#include "ReturnCom.h"

DTO::ReturnCom::ReturnCom(Command* cmd)
	:m_cmd(cmd)
{
	if (m_cmd == nullptr)
		throw "??";
}

DTO::ReturnCom::~ReturnCom()
{
	delete m_cmd;
}

DTO::CommandReturn* DTO::ReturnCom::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_cmd->exec(mem) };
	r->setReturn(true);
	return r;
}

DTO::Command* DTO::ReturnCom::clone()
{
	return new ReturnCom(m_cmd->clone());
}
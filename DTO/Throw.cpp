#include "Throw.h"

DTO::Throw::Throw(Command* cmd)
	:m_cmd(cmd)
{
}

DTO::Throw::~Throw()
{
	delete m_cmd;
}

DTO::CommandReturn* DTO::Throw::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_cmd->exec(mem) };
	r->setThrow(true);
	return r;
}

DTO::Command* DTO::Throw::clone()
{
	return new Throw(m_cmd->clone());
}
#include "ReturnObj.h"
#include "GarbageCollector.h"

DTO::ReturnObj::ReturnObj(IObject* cmd)
	:m_cmd(cmd)
{
	if (m_cmd == nullptr)
		throw "??";
	GarbageCollector::Add(m_cmd);
}

DTO::ReturnObj::~ReturnObj()
{
	GarbageCollector::Remove(m_cmd);
}

DTO::CommandReturn* DTO::ReturnObj::exec(MemoryObject& mem)
{
	return new CommandReturn(m_cmd, true, false);
}

DTO::Command* DTO::ReturnObj::clone()
{
	return new ReturnObj(m_cmd->clone());
}
#include "ReplaceVar.h"
#include "CastException.h"

DTO::ReplaceVar::ReplaceVar(Interface* type, std::string name, Command* cmd)
	:m_type(type), m_name(name), m_cmd(cmd) {}

DTO::CommandReturn* DTO::ReplaceVar::exec(MemoryObject& mem) {
	CommandReturn* obj{ m_cmd->exec(mem) };
	if (dynamic_cast<Object*>(obj->getObject()) != nullptr &&
		!obj->getObject()->getClass()->instanceOf(m_type))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "ReplaceVar", obj, m_type), false, true);
	mem.set(m_name, obj->getObject());
	obj->setReturn(false);
	return obj;
}

DTO::Command* DTO::ReplaceVar::clone()
{
	return new ReplaceVar(m_type, m_name, m_cmd->clone());
}

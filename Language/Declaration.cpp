#include "Declaration.h"
#include "CastException.h"

Declaration::Declaration(Interface* type, std::string name, Command* cmd)
	:m_type(type), m_name(name), m_cmd(cmd) {}

CommandReturn* Declaration::exec(MemoryObject& mem) {
	CommandReturn* obj{ m_cmd->exec(mem) };
	if (!obj->getObject()->getClass()->instanceOf(m_type))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "Declaration", obj, m_type), false, true);
	mem.add(m_name, obj->getObject(), m_type);
	obj->setReturn(false);
	return obj;
}

Command* Declaration::clone()
{
	return new Declaration(m_type, m_name, m_cmd->clone());
}

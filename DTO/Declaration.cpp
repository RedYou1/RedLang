#include "Declaration.h"
#include "CastException.h"

DTO::Declaration::Declaration(Instanciable* type, std::wstring name, Command* cmd)
	:m_type(type), m_name(name), m_cmd(cmd) {}

DTO::CommandReturn* DTO::Declaration::exec(MemoryObject& mem) {
	CommandReturn* obj{ m_cmd->exec(mem) };
	if (obj->isThrow())
		return obj;
	if (dynamic_cast<Object*>(obj->getObject()) != nullptr &&
		!obj->getObject()->getClass()->instanceOf(m_type))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"Declaration", obj, m_type), false, true);
	mem.add(m_name, obj->getObject(), m_type);
	obj->setReturn(false);
	return obj;
}

DTO::Command* DTO::Declaration::clone()
{
	return new Declaration(m_type, m_name, m_cmd->clone());
}

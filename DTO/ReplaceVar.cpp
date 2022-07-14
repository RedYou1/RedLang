#include "ReplaceVar.h"
#include "CastException.h"
#include "NullException.h"

DTO::ReplaceVar::ReplaceVar(Instanciable* type, std::wstring name, Command* cmd)
	:m_type(type), m_name(name), m_cmd(cmd) {}

DTO::CommandReturn* DTO::ReplaceVar::exec(MemoryObject& mem) {
	CommandReturn* obj{ m_cmd->exec(mem) };
	if (dynamic_cast<NullObject*>(obj->getObject()) != nullptr) {
		if (!mem.isNullable(m_name))
			return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at"), true, true);
	}
	else if (!obj->getObject()->getClass()->instanceOf(m_type))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"ReplaceVar", obj, m_type), false, true);
	mem.set(m_name, obj->getObject());
	obj->setReturn(false);
	return obj;
}

DTO::Command* DTO::ReplaceVar::clone()
{
	return new ReplaceVar(m_type, m_name, m_cmd->clone());
}

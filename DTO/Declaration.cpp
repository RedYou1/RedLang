#include "Declaration.h"
#include "CastException.h"
#include "NullException.h"

DTO::Declaration::Declaration(Instanciable* type, std::wstring name, Command* cmd, bool nullable)
	:m_type(type), m_name(name), m_cmd(cmd), m_nullable{ nullable } {}

DTO::CommandReturn* DTO::Declaration::exec(MemoryObject& mem) {
	CommandReturn* obj{ m_cmd->exec(mem) };
	if (obj->isThrow())
		return obj;
	if (m_nullable && dynamic_cast<NullObject*>(obj->getObject()) != nullptr)
		return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument"), true, true);
	else if (!obj->getObject()->getClass()->instanceOf(m_type))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"Declaration", obj, m_type), false, true);
	mem.add(m_name, obj->getObject(), m_type, m_nullable);
	obj->setReturn(false);
	return obj;
}

DTO::Command* DTO::Declaration::clone()
{
	return new Declaration(m_type, m_name, m_cmd->clone(), m_nullable);
}

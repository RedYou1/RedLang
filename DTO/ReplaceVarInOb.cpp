#include "ReplaceVarInOb.h"
#include "CastException.h"
#include "MemoryVar.h"
#include "Var.h"

DTO::ReplaceVarInOb::ReplaceVarInOb(Command* ob, bool _delete, std::wstring name, Command* value)
	:m_ob(ob), m_delete(_delete), m_name(name), m_value(value) {}

DTO::CommandReturn* DTO::ReplaceVarInOb::exec(MemoryObject& mem) {
	CommandReturn* obj{ m_ob->exec(mem) };
	if (dynamic_cast<NullObject*>(obj->getObject()) != nullptr) {
		delete obj;
		return new CommandReturn(new ExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"ReplaceVarInOb"), false, true);
	}
	Var* var{ obj->getObject()->getClass()->getVars()->get(m_name) };
	CommandReturn* val{ m_value->exec(mem) };
	if (dynamic_cast<Object*>(val->getObject()) != nullptr &&
		!val->getObject()->getClass()->instanceOf(var->m_type))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"ReplaceVarInOb", val, var->m_type), false, true);
	((Object*)obj->getObject())->set(var->m_index, val->getObject());
	delete val;
	obj->setReturn(false);
	return obj;
}

DTO::Command* DTO::ReplaceVarInOb::clone()
{
	return new ReplaceVarInOb(m_ob->clone(), m_delete, m_name, m_value->clone());
}

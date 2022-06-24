#include "ReplaceStatVar.h"
#include "CastException.h"
#include "StatVar.h"
#include "MemoryStatVar.h"

DTO::ReplaceStatVar::ReplaceStatVar(Class* _class, std::string name, Command* value)
	:m_class(_class), m_name(name), m_value(value) {}

DTO::CommandReturn* DTO::ReplaceStatVar::exec(MemoryObject& mem) {
	CommandReturn* r{ m_value->exec(mem) };
	StatVar* var{ m_class->getStatVars()->get(m_name) };
	if (dynamic_cast<Object*>(r->getObject()) != nullptr &&
		!r->getObject()->getClass()->instanceOf(var->GetType()))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "ReplaceStatVar", r, var->GetType()), false, true);
	m_class->getStatVars()->set(m_name, r->getObject());
	return r;
}

DTO::Command* DTO::ReplaceStatVar::clone()
{
	return new ReplaceStatVar(m_class, m_name, m_value->clone());
}

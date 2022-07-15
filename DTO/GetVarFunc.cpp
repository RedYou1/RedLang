#include "GetVarFunc.h"
#include "NullException.h"

DTO::GetVarFunc::GetVarFunc(Command* o, std::wstring name)
	:m_o(o), m_name(name)
{
}

DTO::GetVarFunc::~GetVarFunc()
{
	delete m_o;
}

DTO::CommandReturn* DTO::GetVarFunc::exec(MemoryObject& mem)
{
	CommandReturn* a{ m_o->exec(mem) };
	if (dynamic_cast<NullObject*>(a->getObject()) != nullptr) {
		delete a;
		return new CommandReturn(new ExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"GetVarFunc"), false, true);
	}
	IObject* cc{ ((Object*)a->getObject())->get(m_name) };
	CommandReturn* r{ new CommandReturn(cc,true,false) };
	delete a;
	return r;
}

DTO::Command* DTO::GetVarFunc::clone()
{
	return new GetVarFunc(m_o->clone(), m_name);
}

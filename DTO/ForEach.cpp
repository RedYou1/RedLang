#include "ForEach.h"
#include "FunctionClass.h"

DTO::ForEach::ForEach(Type type, std::wstring name, Command* iterable, Function* func)
	:m_type(type), m_name(name), m_iterable(iterable), m_func(func)
{
}

DTO::ForEach::~ForEach()
{
	delete m_iterable;
	delete m_func;
}

DTO::CommandReturn* DTO::ForEach::exec(MemoryObject& mem)
{
	CommandReturn* q{ m_iterable->exec(mem) };

	FunctionO* func{ new FunctionO(GLOBAL::getClasses()->getClass(Paths::Function),m_func) };

	IObject** args{ new IObject * [2]{q->getObject(),func} };
	delete q->getObject()->exec(L"forEach", args, 2);

	delete[] args;
	delete q;
	return new CommandReturn(new NullObject(), false, false);
}

DTO::Command* DTO::ForEach::clone()
{
	return new ForEach(m_type, m_name, m_iterable->clone(), (Function*)m_func->clone());
}

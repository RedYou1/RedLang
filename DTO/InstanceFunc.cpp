#include "InstanceFunc.h"
#include "MemoryFunction.h"
#include "myString.h"

DTO::InstanceFunc::InstanceFunc(Class* _class, Command** args, size_t argsLen)
	:m_class(_class), m_args(args), m_argsLen(argsLen) {
}

DTO::InstanceFunc::~InstanceFunc() {
	for (size_t c(0); c < m_argsLen - 1; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::InstanceFunc::exec(MemoryObject& pre_mem) {
	IObject** argsType{ new IObject * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	args[0] = new CommandReturn(new Object(m_class), false, false);
	argsType[0] = args[0]->getObject();

	for (size_t c{ 1 }; c < m_argsLen; c++) {
		args[c] = m_args[c - 1]->exec(pre_mem);
		argsType[c] = args[c]->getObject();
	}
	std::wstring name{ std::wstring(m_class->getName()) };
	CommandReturn* r{ m_class->getFuncs()->get(myString{&name}.extract2(), argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	//TODO can return an exception
	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] args;
	delete[] argsType;
	r->setReturn(false);
	return r;
}

DTO::Command* DTO::InstanceFunc::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++)
		cmds[c] = m_args[c - 1]->clone();
	return new InstanceFunc(m_class, cmds, m_argsLen);
}
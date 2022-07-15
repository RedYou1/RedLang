#include "ReturnFunc.h"
#include "MemoryFunction.h"

DTO::ReturnFunc::ReturnFunc(Class* _class, std::wstring name, Command** args, size_t argsLen)
	:m_class(_class), m_name(name), m_args(args), m_argsLen(argsLen) {
}
DTO::ReturnFunc::~ReturnFunc() {
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::ReturnFunc::exec(MemoryObject& mem) {
	IObject** argsType{ new IObject * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(mem);
		argsType[c] = args[c]->getObject();
	}

	CommandReturn* r{ m_class->getFuncs()->get(m_name, argsType, m_argsLen)->exec(mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;
	return r;
}

DTO::Command* DTO::ReturnFunc::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		if (m_args[c] != nullptr)
			cmds[c] = m_args[c]->clone();
		else
			cmds[c] = nullptr;
	return new ReturnFunc(m_class, m_name, cmds, m_argsLen);
}
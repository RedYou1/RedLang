#include "FunctionCom.h"
#include "MemoryFunction.h"

DTO::FunctionCom::FunctionCom(Class* _class, std::wstring name, Command** args, size_t argsLen)
	:m_class(_class), m_name(name), m_args(args), m_argsLen(argsLen) {}

DTO::FunctionCom::~FunctionCom()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionCom::exec(MemoryObject& mem) {

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

	r->setReturn(false);
	return r;
}

DTO::Command* DTO::FunctionCom::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new FunctionCom(m_class, m_name, cmds, m_argsLen);
}
#include "FunctionKnownCom.h"

DTO::FunctionKnownCom::FunctionKnownCom(Function* func, Command** args, size_t argsLen)
	:m_func(func), m_args(args), m_argsLen(argsLen)
{
}

DTO::FunctionKnownCom::~FunctionKnownCom()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionKnownCom::exec(MemoryObject& pre_mem)
{
	CommandReturn* r{ m_func->exec(pre_mem,m_args,m_argsLen) };
	r->setReturn(false);
	return r;
}

DTO::Command* DTO::FunctionKnownCom::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new FunctionKnownCom(m_func, cmds, m_argsLen);
}
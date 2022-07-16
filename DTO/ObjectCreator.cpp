#include "ObjectCreator.h"

DTO::ObjectCreator::ObjectCreator(Function* func, IObject** args, size_t argsLen)
	:m_func(func), m_args(args), m_argsLen(argsLen)
{
	for (size_t c(1); c < m_argsLen; c++)
		m_args[c]->addRef();
}

DTO::ObjectCreator::~ObjectCreator()
{
	for (size_t c(1); c < m_argsLen; c++)
		m_args[c]->removeRef();
	delete[] m_args;
}

DTO::CommandReturn* DTO::ObjectCreator::exec(MemoryObject& pre_mem)
{
	IObject** cmds{ new IObject * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++) {
		cmds[c] = m_args[c]->clone();
		cmds[c]->addRef();
	}
	CommandReturn* r{ m_func->exec(pre_mem, cmds, m_argsLen) };
	for (size_t c(1); c < m_argsLen; c++) {
		cmds[c]->removeRef();
	}
	delete[] cmds;
	return r;
}

DTO::Command* DTO::ObjectCreator::clone()
{
	IObject** cmds{ new IObject * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new ObjectCreator(m_func, cmds, m_argsLen);
}
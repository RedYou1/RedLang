#include "FunctionDynCom.h"
#include "Class.h"
#include "Object.h"
#include "MemoryFunction.h"

DTO::FunctionDynCom::FunctionDynCom(Command* ob, std::wstring name, Command** args, size_t argsLen)
	:m_ob(ob), m_name(name), m_args(args), m_argsLen(argsLen)
{
}

DTO::FunctionDynCom::~FunctionDynCom()
{
	if (m_argsLen > 0 && m_args[0] != m_ob)
		delete m_ob;
	for (size_t c(0); c < m_argsLen; c++) {
		delete m_args[c];
	}
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionDynCom::exec(MemoryObject& pre_mem)
{
	CommandReturn* cm(m_ob->exec(pre_mem));
	Class* cl{ cm->getObject()->getClass() };
	delete cm;
	if (cl == nullptr)
		throw "??";

	IObject** argsType{ new IObject * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(pre_mem);
		argsType[c] = args[c]->getObject();
	}

	CommandReturn* r{ cl->getFuncs()->get(m_name, argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;

	r->setReturn(false);
	return r;
}

DTO::Command* DTO::FunctionDynCom::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new FunctionDynCom(m_ob->clone(), m_name, cmds, m_argsLen);
}
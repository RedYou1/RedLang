#include "ObFunc.h"
#include "Object.h"
#include "MemoryFunction.h"

DTO::ObFunc::ObFunc(std::wstring name, Command** args, size_t argsLen)
	:m_name(name), m_args(args), m_argsLen(argsLen)
{
}

DTO::ObFunc::~ObFunc()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::ObFunc::exec(MemoryObject& pre_mem)
{

	IObject** argsType{ new IObject * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(pre_mem);
		argsType[c] = args[c]->getObject();
	}

	CommandReturn* r{ args[0]->getObject()->getClass()->getFuncs()->get(m_name, argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;
	return r;
}

DTO::Command* DTO::ObFunc::clone()
{
	throw "??";
}
#include "Try.h"

DTO::Try::Try(FunctionBlock* t_try, Instanciable** catchKeys, std::string* names, FunctionBlock** catchs, size_t catchLen)
	:m_try(t_try), m_catchKeys(catchKeys), m_names(names), m_catchs(catchs), m_catchLen(catchLen)
{}

DTO::Try::~Try()
{
	delete m_try;
	for (size_t c(0); c < m_catchLen; c++) {
		delete m_catchs[c];
	}
	delete[] m_names;
	delete[] m_catchKeys;
	delete[] m_catchs;
}

DTO::CommandReturn* DTO::Try::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_try->exec(mem) };
	if (r->isThrow()) {
		for (size_t c(0); c < m_catchLen; c++) {
			if (r->getObject()->getClass()->instanceOf(m_catchKeys[c])) {
				std::string* name{ new std::string[1]{m_names[c]} };
				IObject** coms{ new IObject * [1]{r->getObject()} };
				CommandReturn* r2{ m_catchs[c]->exec2(mem,name,coms,1) };
				delete[] coms;
				delete[] name;
				delete r;
				return r2;
			}
		}
	}
	return r;
}

DTO::Command* DTO::Try::clone()
{
	Instanciable** catchKeys{ new Instanciable * [m_catchLen] };
	for (size_t c(0); c < m_catchLen; c++)
		catchKeys[c] = m_catchKeys[c];
	std::string* names{ new std::string[m_catchLen] };
	for (size_t c(0); c < m_catchLen; c++)
		names[c] = m_names[c];
	FunctionBlock** catchs{ new FunctionBlock * [m_catchLen] };
	for (size_t c(0); c < m_catchLen; c++)
		catchs[c] = (FunctionBlock*)m_catchs[c]->clone();
	return new Try((FunctionBlock*)m_try->clone(), catchKeys, names, catchs, m_catchLen);
}

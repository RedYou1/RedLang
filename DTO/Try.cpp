#include "Try.h"

DTO::Try::Try(FunctionBlock* t_try, Instanciable** catchKeys, std::wstring* names, FunctionBlock** catchs, size_t catchLen, FunctionBlock* finally)
	:m_try(t_try), m_catchKeys(catchKeys), m_names(names), m_catchs(catchs), m_catchLen(catchLen), m_finally(finally)
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
	delete m_finally;
}

DTO::CommandReturn* DTO::Try::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_try->exec(mem) };
	if (r->isThrow()) {
		for (size_t c(0); c < m_catchLen; c++) {
			if (r->getObject()->getClass()->instanceOf(m_catchKeys[c])) {
				std::wstring* name{ new std::wstring[1]{m_names[c]} };
				IObject** coms{ new IObject * [1]{r->getObject()} };
				CommandReturn* r2{ m_catchs[c]->exec2(mem,name,coms,1) };
				delete[] coms;
				delete[] name;
				delete r;
				r = r2;
				break;
			}
		}
	}
	if (m_finally != nullptr) {
		CommandReturn* r2{ m_finally->exec(mem) };
		if (r2->exitFunction()) {
			delete r;
			r = r2;
			r2 = nullptr;
		}
		delete r2;
	}
	return r;
}

DTO::Command* DTO::Try::clone()
{
	Instanciable** catchKeys{ new Instanciable * [m_catchLen] };
	for (size_t c(0); c < m_catchLen; c++)
		catchKeys[c] = m_catchKeys[c];
	std::wstring* names{ new std::wstring[m_catchLen] };
	for (size_t c(0); c < m_catchLen; c++)
		names[c] = m_names[c];
	FunctionBlock** catchs{ new FunctionBlock * [m_catchLen] };
	for (size_t c(0); c < m_catchLen; c++)
		catchs[c] = (FunctionBlock*)m_catchs[c]->clone();
	return new Try((FunctionBlock*)m_try->clone(), catchKeys, names, catchs, m_catchLen, m_finally == nullptr ? nullptr : (FunctionBlock*)m_finally->clone());
}

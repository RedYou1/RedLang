#include "MemoryFunction.h"

DTO::MemoryFunction::~MemoryFunction() {
	for (typename std::map<std::wstring, std::list<Function*>>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		for (typename std::list<Function*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			delete* it2;
		}
	}
}

void DTO::MemoryFunction::add(std::wstring name, Function* o) {
	if (m_vars.find(name) == m_vars.end())
		m_vars.insert(std::pair<std::wstring, std::list<Function*>>(name, std::list<Function*>()));
	typename std::map<std::wstring, std::list<Function*>>::iterator it(m_vars.find(name));
	it->second.push_back(o);
}

std::list<DTO::Function*> DTO::MemoryFunction::get(std::wstring name) {
	typename std::map<std::wstring, std::list<Function*>>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent)
			return m_parent->get(name);
		throw "not found";
	}
	return it->second;
}

DTO::Function* DTO::MemoryFunction::get(std::wstring name, Instanciable** argsType, size_t argsLen) {
	std::list<Function*> li(get(name));
	for (std::list<Function*>::iterator it = li.begin(); it != li.end(); ++it) {
		if ((*it)->getSignature()->similarI(argsType, argsLen))
			return (*it);
	}
	if (m_parent)
		return m_parent->get(name, argsType, argsLen);
	throw "not found";
}

bool DTO::MemoryFunction::containsI(std::wstring name, Instanciable** argsType, size_t argsLen) {
	std::list<Function*> li(get(name));
	for (std::list<Function*>::iterator it = li.begin(); it != li.end(); ++it) {
		if ((*it)->getSignature()->equalsI(argsType, argsLen))
			return true;
	}
	if (m_parent)
		return m_parent->containsI(name, argsType, argsLen);
	return false;
}

bool DTO::MemoryFunction::containsI(std::wstring name, Arg* args, size_t argsLen) {
	std::list<Function*> li(get(name));
	for (std::list<Function*>::iterator it = li.begin(); it != li.end(); ++it) {
		if ((*it)->getSignature()->equalsI(args, argsLen))
			return true;
	}
	if (m_parent)
		return m_parent->containsI(name, args, argsLen);
	return false;
}
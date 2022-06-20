#include "MemoryFunction.h"

MemoryFunction::~MemoryFunction() {
	for (typename std::map<std::string, std::list<Function*>>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		for (typename std::list<Function*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			delete* it2;
		}
	}
}

void MemoryFunction::add(std::string name, Function* o) {
	if (m_vars.find(name) == m_vars.end())
		m_vars.insert(std::pair<std::string, std::list<Function*>>(name, std::list<Function*>()));
	typename std::map<std::string, std::list<Function*>>::iterator it(m_vars.find(name));
	it->second.push_back(o);
}

std::list<Function*> MemoryFunction::get(std::string name) {
	typename std::map<std::string, std::list<Function*>>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent)
			return m_parent->get(name);
		throw "not found";
	}
	return it->second;
}

Function* MemoryFunction::get(std::string name, Interface** argsType, size_t argsLen) {
	std::list<Function*> li(get(name));
	for (std::list<Function*>::iterator it = li.begin(); it != li.end(); ++it) {
		if ((*it)->getSignature()->similarI(argsType, argsLen))
			return (*it);
	}
	if (m_parent)
		return m_parent->get(name, argsType, argsLen);
	throw "not found";
}

bool MemoryFunction::containsI(std::string name, Interface** argsType, size_t argsLen) {
	std::list<Function*> li(get(name));
	for (std::list<Function*>::iterator it = li.begin(); it != li.end(); ++it) {
		if ((*it)->getSignature()->equalsI(argsType, argsLen))
			return true;
	}
	if (m_parent)
		return m_parent->containsI(name, argsType, argsLen);
	return false;
}
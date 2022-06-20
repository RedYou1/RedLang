#include "MemoryVariable.h"
#include "Command.h"
#include "Class.h"

MemoryVariable::MemoryVariable(MemoryVariable* parent)
	:m_parent(parent), m_vars(std::map<std::string, Interface*>())
{
}

void MemoryVariable::add(std::string name, Interface* o) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::string, Interface*>(name, o));
}

void MemoryVariable::set(std::string name, Interface* o) {
	typename std::map<std::string, Interface*>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent != nullptr)
			return m_parent->set(name, o);
		else
			throw "not found";
	}
	if (it->second == o)
		return;
	delete it->second;
	it->second = o;
}

bool MemoryVariable::containKey(std::string name) {
	return m_vars.find(name) != m_vars.end() || (m_parent != nullptr && m_parent->containKey(name));
}

Interface* MemoryVariable::get(std::string name) {
	typename std::map<std::string, Interface*>::iterator it(m_vars.find(name));
	if (it == m_vars.end())
		if (m_parent != nullptr)
			return m_parent->get(name);
		else
			throw "not found";
	return it->second;
}

size_t MemoryVariable::size() {
	return m_vars.size();
}
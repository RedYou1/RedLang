#include "MemoryVariable.h"
#include "Command.h"
#include "Class.h"

DTO::MemoryVariable::MemoryVariable(MemoryVariable* parent)
	:m_parent(parent), m_vars(std::map<std::wstring, Type>())
{
}

void DTO::MemoryVariable::add(std::wstring name, Type o) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::wstring, Type>(name, o));
}

bool DTO::MemoryVariable::containKey(std::wstring name) {
	return m_vars.find(name) != m_vars.end() || (m_parent != nullptr && m_parent->containKey(name));
}

DTO::Type DTO::MemoryVariable::get(std::wstring name) {
	typename std::map<std::wstring, Type>::iterator it(m_vars.find(name));
	if (it == m_vars.end())
		if (m_parent != nullptr)
			return m_parent->get(name);
		else
			throw "not found";
	return it->second;
}

size_t DTO::MemoryVariable::size() {
	return m_vars.size();
}
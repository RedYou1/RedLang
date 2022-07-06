#include "Class.h"
#include "MemoryObject.h"
#include "MemorySourceFile.h"
#include "StatVar.h"
#include "MemoryVar.h"
#include "MemoryStatVar.h"
#include "MemoryFunction.h"

DTO::Class::Class(std::string name, std::string path, Class* parent)
	: Class(name, path, parent, new Interface* [0], 0) {}

DTO::Class::Class(std::string name, std::string path, Class* parent, Interface** implements, size_t implementsLen)
	: Interface(name, path, new MemorySourceFile{ false }, implements, implementsLen),
	m_funcs(new MemoryFunction(parent ? parent->m_funcs : nullptr)),
	m_vars(parent ? new MemoryVar(parent->m_vars) : new MemoryVar()),
	m_statVars(new MemoryStatVar()),
	m_parent(parent) {}

DTO::Class::Class(std::string name, std::string path, Class* parent, MemorySourceFile* genTypes)
	: Class(name, path, parent, new Interface* [0], 0, genTypes) {}

DTO::Class::Class(std::string name, std::string path, Class* parent, Interface** implements, size_t implementsLen, MemorySourceFile* genTypes)
	: Interface(name, path, genTypes, implements, implementsLen),
	m_funcs(new MemoryFunction(parent ? parent->m_funcs : nullptr)),
	m_vars(parent ? new MemoryVar(parent->m_vars) : new MemoryVar()),
	m_statVars(new MemoryStatVar()),
	m_parent(parent) {}

DTO::Class::~Class() {
	delete m_funcs;
	delete m_vars;
	delete m_statVars;
}

DTO::Class* DTO::Class::extends() { return m_parent; }

bool DTO::Class::instanceOf(Interface* other)
{
	if (Interface::instanceOf(other))
		return true;
	if (m_parent)
		return m_parent->instanceOf(other);
	return false;
}

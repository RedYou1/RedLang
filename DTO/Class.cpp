#include "Class.h"
#include "MemoryObject.h"
#include "MemorySourceFile.h"
#include "StatVar.h"
#include "MemoryVar.h"
#include "MemoryStatVar.h"
#include "MemoryFunction.h"
#include "Command.h"
#include "Function.h"
#include "Signature.h"
#include "Generic.h"

DTO::Class::Class(std::wstring name, std::filesystem::path path, Class* parent)
	: Class(name, path, parent, new Interface* [0], 0) {}

DTO::Class::Class(std::wstring name, std::filesystem::path path, Class* parent, Interface** implements, size_t implementsLen)
	: Interface(name, path, new MemorySourceFile{ false }, implements, implementsLen),
	m_funcs(new MemoryFunction(parent ? parent->m_funcs : nullptr)),
	m_vars(parent ? new MemoryVar(parent->m_vars) : new MemoryVar()),
	m_statVars(new MemoryStatVar()),
	m_parent(parent) {}

DTO::Class::Class(std::wstring name, std::filesystem::path path, Class* parent, MemorySourceFile* genTypes)
	: Class(name, path, parent, new Interface* [0], 0, genTypes) {}

DTO::Class::Class(std::wstring name, std::filesystem::path path, Class* parent, Interface** implements, size_t implementsLen, MemorySourceFile* genTypes)
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

bool DTO::Class::instanceOf(Instanciable* other)
{
	if (Interface::instanceOf(other))
		return true;
	if (m_parent)
		return m_parent->instanceOf(other);
	return false;
}

DTO::MemoryFunction* DTO::Class::getFuncs()
{
	return m_funcs;
}

DTO::MemoryVar* DTO::Class::getVars()
{
	return m_vars;
}

DTO::MemoryStatVar* DTO::Class::getStatVars()
{
	return m_statVars;
}

void DTO::Class::addFunc(std::wstring name, Type returnType, Arg* args, size_t argsLen, Command* cmd, bool infinity)
{
	m_funcs->add(name, new Function(new Signature(L"", returnType, args, argsLen, infinity), new Command * [1]{ cmd }, 1));
}

void DTO::Class::addFunc(std::wstring name, Type returnType, Arg* args, size_t argsLen, Command** cmds, size_t cmdsLen, bool infinity)
{
	m_funcs->add(name, new Function(new Signature(L"", returnType, args, argsLen, infinity), cmds, cmdsLen));
}

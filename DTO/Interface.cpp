#include "Interface.h"
#include "Signature.h"
#include "Class.h"
#include "MemoryFunction.h"
#include "MemorySourceFile.h"
#include "Generic.h"

DTO::Interface::Interface(std::wstring name, std::filesystem::path path)
	:Instanciable(name, path), m_genTypes(new MemorySourceFile{ false }), m_interfaces(new Interface* [0]), m_interfaceLen(0), m_signs()
{
}

DTO::Interface::Interface(std::wstring name, std::filesystem::path path, MemorySourceFile* genTypes)
	: Instanciable(name, path), m_genTypes(genTypes), m_interfaces(new Interface* [0]), m_interfaceLen(0), m_signs()
{
}

DTO::Interface::Interface(std::wstring name, std::filesystem::path path, Interface** implements, size_t implementsLen)
	: Instanciable(name, path), m_genTypes(new MemorySourceFile{ false }), m_interfaces(implements), m_interfaceLen(implementsLen), m_signs()
{
}

DTO::Interface::Interface(std::wstring name, std::filesystem::path path, MemorySourceFile* genTypes, Interface** implements, size_t implementsLen)
	: Instanciable(name, path), m_genTypes(genTypes), m_interfaces(implements), m_interfaceLen(implementsLen), m_signs()
{
}

DTO::Interface::~Interface()
{
	delete[] m_interfaces;
	for (typename std::map<std::wstring, std::list<Signature*>>::iterator it = m_signs.begin(); it != m_signs.end(); ++it) {
		for (typename std::list<Signature*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			delete* it2;
		}
	}
	delete m_genTypes;
}

void DTO::Interface::add(std::wstring name, Signature* o)
{
	if (m_signs.find(name) == m_signs.end())
		m_signs.insert(std::pair<std::wstring, std::list<Signature*>>(name, std::list<Signature*>()));
	typename std::map<std::wstring, std::list<Signature*>>::iterator it(m_signs.find(name));
	it->second.push_back(o);
}

std::list<DTO::Signature*> DTO::Interface::get(std::wstring name)
{
	typename std::map<std::wstring, std::list<Signature*>>::iterator it(m_signs.find(name));
	if (it == m_signs.end()) {
		for (size_t c(0); c < m_interfaceLen; c++)
		{
			try {
				return m_interfaces[c]->get(name);
			}
			catch (...) {}
		}
		throw "not found";
	}
	return it->second;
}

DTO::Signature* DTO::Interface::get(std::wstring name, Instanciable** argsType, size_t argsLen)
{
	std::list<Signature*> li(get(name));
	for (std::list<Signature*>::iterator it = li.begin(); it != li.end(); ++it) {
		if ((*it)->similar(argsType, argsLen))
			return (*it);
	}
	for (size_t c(0); c < m_interfaceLen; c++)
	{
		try {
			return m_interfaces[c]->get(name, argsType, argsLen);
		}
		catch (...) {}
	}
	throw "not found";
}

bool DTO::Interface::containsAll(Class* _class)
{
	for (std::map<std::wstring, std::list<Signature*>>::iterator it(m_signs.begin()); it != m_signs.end(); ++it) {
		for (std::list<Signature*>::iterator it2(it->second.begin()); it2 != it->second.end(); ++it2) {
			if (!_class->getFuncs()->containsI(it->first, (*it2)->getArgs(), (*it2)->getArgsLen()))
				return false;
		}
	}

	for (size_t c(0); c < m_interfaceLen; c++)
		if (!m_interfaces[c]->containsAll(_class))
			return false;
	return true;
}

bool DTO::Interface::instanceOf(Instanciable* other)
{
	if (other == nullptr)
		return true;
	if (this == nullptr)
		return true;
	if (this == other)
		return true;
	if (GenericI * o{ dynamic_cast<GenericI*>(other) })
		return o->instanceOf(this);
	for (size_t c(0); c < m_interfaceLen; c++)
		if (m_interfaces[c]->instanceOf(other))
			return true;
	return false;
}

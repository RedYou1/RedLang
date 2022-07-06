#include "Interface.h"
#include "Signature.h"
#include "Class.h"
#include "MemoryFunction.h"
#include "MemorySourceFile.h"

DTO::Interface::Interface(std::string name, std::string path)
	:SourceFile(name, path), m_genTypes(new MemorySourceFile{ false }), m_interfaces(new Interface* [0]), m_interfaceLen(0), m_signs()
{
}

DTO::Interface::Interface(std::string name, std::string path, MemorySourceFile* genTypes)
	: SourceFile(name, path), m_genTypes(genTypes), m_interfaces(new Interface* [0]), m_interfaceLen(0), m_signs()
{
}

DTO::Interface::Interface(std::string name, std::string path, Interface** implements, size_t implementsLen)
	: SourceFile(name, path), m_genTypes(new MemorySourceFile{ false }), m_interfaces(implements), m_interfaceLen(implementsLen), m_signs()
{
}

DTO::Interface::Interface(std::string name, std::string path, MemorySourceFile* genTypes, Interface** implements, size_t implementsLen)
	: SourceFile(name, path), m_genTypes(genTypes), m_interfaces(implements), m_interfaceLen(implementsLen), m_signs()
{
}

DTO::Interface::~Interface()
{
	delete[] m_interfaces;
	for (typename std::map<std::string, std::list<Signature*>>::iterator it = m_signs.begin(); it != m_signs.end(); ++it) {
		for (typename std::list<Signature*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			delete* it2;
		}
	}
	delete m_genTypes;
}

void DTO::Interface::add(std::string name, Signature* o)
{
	if (m_signs.find(name) == m_signs.end())
		m_signs.insert(std::pair<std::string, std::list<Signature*>>(name, std::list<Signature*>()));
	typename std::map<std::string, std::list<Signature*>>::iterator it(m_signs.find(name));
	it->second.push_back(o);
}

std::list<DTO::Signature*> DTO::Interface::get(std::string name)
{
	typename std::map<std::string, std::list<Signature*>>::iterator it(m_signs.find(name));
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

DTO::Signature* DTO::Interface::get(std::string name, Interface** argsType, size_t argsLen)
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
	for (std::map<std::string, std::list<Signature*>>::iterator it(m_signs.begin()); it != m_signs.end(); ++it) {
		for (std::list<Signature*>::iterator it2(it->second.begin()); it2 != it->second.end(); ++it2) {
			if (!_class->getFuncs()->containsI(it->first, (*it2)->getArgsType(), (*it2)->getArgsLen()))
				return false;
		}
	}

	for (size_t c(0); c < m_interfaceLen; c++)
		if (!m_interfaces[c]->containsAll(_class))
			return false;
	return true;
}

bool DTO::Interface::instanceOf(Interface* other)
{
	if (other == nullptr)
		return true;
	if (this == nullptr)
		return true;
	if (this == other)
		return true;
	for (size_t c(0); c < m_interfaceLen; c++)
		if (m_interfaces[c]->instanceOf(other))
			return true;
	return false;
}

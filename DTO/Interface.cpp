#include "Interface.h"
#include "Signature.h"
#include "myString.h"
#include "Class.h"
#include "MemoryFunction.h"
#include "MemorySourceFile.h"
#include "Global.h"

DTO::Interface::Interface(std::string name, std::string path)
	:SourceFile(name, path), m_genTypes(new MemorySourceFile{ false }), m_interfaces(new Interface* [0]), m_interfaceLen(0), m_signs()
{
}

DTO::Interface::Interface(std::string name, std::string path, MemorySourceFile* genTypes)
	: SourceFile(name, path), m_genTypes(genTypes), m_interfaces(new Interface* [0]), m_interfaceLen(0), m_signs()
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

DTO::Interface* DTO::Interface::parse(std::string path, std::string str, MemorySourceFile& genTypes)
{
	myString s{ &str };

	s.removeUseless();

	std::string name{ s.extractName() };
	s.removeUseless();

	Interface** implements;
	size_t implementsLen;
	{
		std::queue<Interface*> interfaces{};
		if (s.startWith("extends")) {
			s.extract(7);
			s.removeUseless();
			do {
				interfaces.push(GLOBAL::getClasses()->getInterface(s.extractName()));
				s.removeUseless();
			} while (str.at(0) != '{');
		}

		implementsLen = interfaces.size();
		implements = new Interface * [implementsLen];
		size_t c{ 0 };
		while (!interfaces.empty() && c < implementsLen) {
			implements[c] = interfaces.front();
			interfaces.pop();
			c++;
		}
		if (c > implementsLen)
			throw "??";
	}

	str = s.extractFunc();
	s = myString{ &str };
	s.removeUseless();

	Interface* _interface{ new Interface(name,path,&genTypes,implements,implementsLen) };
	GLOBAL::getClasses()->set(name, _interface);

	while (!str.empty()) {
		s.removeUseless();
		std::string typeName{ s.extractName() };
		Interface* type{ nullptr };
		if (typeName != "void") {
			if (genTypes.containKey(&typeName))
				type = genTypes.getType(typeName);
			else if (genTypes.containKey(&typeName))
				type = GLOBAL::getClasses()->getType(typeName);
			else
				throw "??";
			if (type == nullptr)
				throw "??";
		}
		s.removeUseless();
		std::string name{ s.extractName() };
		s.removeUseless();

		std::queue<std::string> args{ myString(&s.extractFunc2()).split2(',') };

		size_t argsLen{ args.size() + 1 };
		Interface** classes{ new Interface * [argsLen] };
		std::string* argsName{ new std::string[argsLen] };
		size_t argI{ 1 };

		classes[0] = _interface;
		argsName[0] = std::string{ "this" };

		while (!args.empty() && argI < argsLen) {
			std::string current = args.front();
			myString m{ &current };

			std::string argTypeName{ m.extractName() };
			if (genTypes.containKey(&argTypeName))
				classes[argI] = genTypes.getType(argTypeName);
			else if (genTypes.containKey(&argTypeName))
				classes[argI] = GLOBAL::getClasses()->getType(argTypeName);
			else
				throw "??";

			m.removeUseless();
			argsName[argI] = current;

			args.pop();
			argI++;
		}

		if (!args.empty() || argI < argsLen)
			throw "?";

		_interface->add(name, new Signature(path, type, classes, argsName, argsLen));
		s.extract(1);
	}


	return _interface;
}

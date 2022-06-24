#include "Class.h"
#include "myString.h"
#include "Return.h"

#include "MemoryObject.h"
#include "MemorySourceFile.h"
#include "Command.h"
#include "Var.h"
#include "StatVar.h"
#include "MemoryVar.h"
#include "MemoryStatVar.h"
#include "MemoryFunction.h"
#include "Global.h"

Class::Class(std::string name, std::string path, Class* parent)
	: Class(name, path, parent, new Interface* [0], 0) {}

Class::Class(std::string name, std::string path, Class* parent, Interface** implements, size_t implementsLen)
	: Interface(name, path, new MemorySourceFile{ false }, implements, implementsLen),
	m_funcs(new MemoryFunction(parent ? parent->m_funcs : nullptr)),
	m_statFuncs(new MemoryFunction(nullptr)),
	m_vars(parent ? new MemoryVar(parent->m_vars) : new MemoryVar()),
	m_statVars(new MemoryStatVar()),
	m_parent(parent) {}

Class::Class(std::string name, std::string path, Class* parent, MemorySourceFile* genTypes)
	: Class(name, path, parent, new Interface* [0], 0, genTypes) {}

Class::Class(std::string name, std::string path, Class* parent, Interface** implements, size_t implementsLen, MemorySourceFile* genTypes)
	: Interface(name, path, genTypes, implements, implementsLen),
	m_funcs(new MemoryFunction(parent ? parent->m_funcs : nullptr)),
	m_statFuncs(new MemoryFunction(nullptr)),
	m_vars(parent ? new MemoryVar(parent->m_vars) : new MemoryVar()),
	m_statVars(new MemoryStatVar()),
	m_parent(parent) {}

Class::~Class() {
	delete m_funcs;
	delete m_statFuncs;
	delete m_vars;
	delete m_statVars;
}

Class* Class::extends() { return m_parent; }

bool Class::instanceOf(Interface* other)
{
	if (Interface::instanceOf(other))
		return true;
	if (m_parent)
		return m_parent->instanceOf(other);
	return false;
}

Class* Class::parse(std::string path, std::string str, MemorySourceFile& genTypes) {
	myString s{ &str };

	s.removeUseless();

	std::string name{ s.extractName() };
	s.removeUseless();

	Class* parent{ GLOBAL::getClasses()->getClass(Paths::Object) };

	if (s.startWith("extends")) {
		s.extract(7);
		s.removeUseless();
		std::string tname{ s.extractName() };
		parent = GLOBAL::getClasses()->getClass(tname);
		if (parent == nullptr)
			throw "??";
		s.removeUseless();
	}

	Interface** implements;
	size_t implementsLen;
	{
		std::queue<Interface*> interfaces{};
		if (s.startWith("implements")) {
			s.extract(10);
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
		if (!interfaces.empty())
			throw "??";
	}

	str = s.extractFunc();
	myString s2{ &str };
	s2.removeUseless();

	Class* _class{ new Class(name,path,parent,implements,implementsLen,&genTypes) };
	GLOBAL::getClasses()->set(name, _class);

	while (!str.empty()) {
		s2.removeUseless();
		bool _static{ false };

		std::string word{ s.extractName() };
		s2.removeUseless();
		if (word == "static") {
			_static = true;
			word = s.extractName();
			s2.removeUseless();
		}

		Interface* type{ nullptr };


		if (word == "function") {
			std::string signature{};
			Function* func{ Function::parseDef(!_static,_class,&signature, &str,genTypes) };
			if (_static) {
				_class->m_statFuncs->add(signature, func);
			}
			else {
				_class->m_funcs->add(signature, func);
			}
			continue;
		}

		if (genTypes.containKey(&word))
			type = genTypes.getInterface(word);
		else if (GLOBAL::getClasses()->containKey(&word, &genTypes))
			type = GLOBAL::getClasses()->getInterface(word);
		else
			throw "??";
		if (type == nullptr)
			throw "no type in var";

		std::string name{ s.extractName() };

		s.removeUseless();

		if (_static) {
			StatVar* _default{ new StatVar{type,nullptr} };
			if (str.at(0) == '=') {
				s.extract(1);
				s.removeUseless();
				MemoryVariable variables{ };
				MemoryObject a{ };
				Command* r{ Return::parse(variables, str,genTypes) };
				CommandReturn* re{ r->exec(a) };

				_default->SetValue(re->getObject());

				delete re;
				delete r;
				if (!_default->GetValue()->getClass()->instanceOf(type))
					throw "CastException";
			}
			_class->m_statVars->add(name, _default);
		}
		else {
			Command* _default{ nullptr };
			if (str.at(0) == '=') {
				s.extract(1);
				s.removeUseless();
				MemoryVariable variables{ };
				_default = Return::parse(variables, str, genTypes);
			}
			_class->m_vars->add(name, new Var(_class->m_vars->size(), type, _default));
		}
		s.extract(1);
	}

	for (size_t c(0); c < _class->m_interfaceLen; c++)
		if (!_class->m_interfaces[c]->containsAll(_class))
			throw "??";
	return _class;
}

#include "Array.h"

DTO::Array::ArrayC::ArrayC(std::wstring name, Instanciable* type)
	:Class(name, Paths::Array, GLOBAL::getClasses()->getClass(Paths::Object),
		new Interface* [1]{ GLOBAL::getClasses()->checkGetInterface(std::wstring(Paths::Iterable) + L"<" + type->getName() + L">") }, 1), m_type(type)
{
	Number* number{ (Number*)GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	LongC* Long{ (LongC*)GLOBAL::getClasses()->getClass(Paths::Long) };
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	addFunc(L"Array", this, new Arg[2]{ {this,L"this",nullptr,true}, {number,L"c"} }, 2, new ArrayConstruct(this));
	addFunc(L"equals", Bool, new Arg[2]{ {this,L"this"}, {this,L"c"} }, 2, new Equals(Bool));
	addFunc(L"get", m_type, new Arg[2]{ {this,L"this"}, {number,L"c"} }, 2, new Get(this));
	addFunc(L"set", nullptr, new Arg[3]{ {this,L"this"}, {m_type,L"o"}, {number,L"c"} }, 3, new Set(this));
	addFunc(L"size", Long, new Arg[1]{ {this,L"this"} }, 1, new Size(Long));
	addFunc(L"resize", nullptr, new Arg[2]{ {this,L"this"}, {number,L"c"} }, 2, new Resize());

	addFunc(L"forEach", nullptr, new Arg[2]{ {this,L"this"}, {function,L"func"} }, 2, new forEach());
}

DTO::SourceFile* DTO::Array::create(std::wstring newName, Instanciable** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	ArrayC* g{ new ArrayC(newName, gens[0]) };
	add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}

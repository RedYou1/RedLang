#include "Array.h"

DTO::Array::ArrayC::ArrayC(std::wstring name, Instanciable* type)
	:Class(name, Paths::Array, GLOBAL::getClasses()->getClass(Paths::Object),
		new Interface* [1]{ GLOBAL::getClasses()->checkGetInterface(std::wstring(Paths::Iterable) + L"<" + type->getName() + L">") }, 1), m_type(type)
{
	Number* number{ (Number*)GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	LongC* Long{ (LongC*)GLOBAL::getClasses()->getClass(Paths::Long) };
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	addFunc(L"Array", { this ,false }, new Arg[2]{ {this,true,L"this"}, {number,false,L"c"} }, 2, new ArrayConstruct(this));
	addFunc(L"equals", { Bool,false }, new Arg[2]{ {this,false,L"this"}, {this,false,L"c"} }, 2, new Equals(Bool));
	addFunc(L"get", { m_type,true }, new Arg[2]{ {this,false,L"this"}, {number,false,L"c"} }, 2, new Get(this));
	addFunc(L"set", { nullptr, true }, new Arg[3]{ {this,false,L"this"}, {m_type,true,L"o"}, {number,false,L"c"} }, 3, new Set(this));
	addFunc(L"size", { Long,false }, new Arg[1]{ {this,false,L"this"} }, 1, new Size(Long));
	addFunc(L"resize", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {number,false,L"c"} }, 2, new Resize());

	addFunc(L"forEach", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {function,false,L"func"} }, 2, new forEach());
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

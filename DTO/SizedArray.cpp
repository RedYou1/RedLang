#include "SizedArray.h"

DTO::SizedArray::SizedArrayC::SizedArrayC(std::string name, Instanciable* type)
	:Class(name, Paths::SizedArray, GLOBAL::getClasses()->getClass(Paths::Object),
		new Interface* [1]{ GLOBAL::getClasses()->getInterface(std::string(Paths::Iterable) + "<" + type->getName() + ">") }, 1), m_type(type)
{
	Number* number{ (Number*)GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	LongC* Long{ (LongC*)GLOBAL::getClasses()->getClass(Paths::Long) };
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	addFunc("SizedArray", this, new Arg[2]{ this,"this", number,"c" }, 2, new ArrayConstruct(this));
	addFunc("equals", Bool, new Arg[2]{ this,"this", this,"c" }, 2, new Equals(Bool));
	addFunc("get", m_type, new Arg[2]{ this,"this", number,"c" }, 2, new Get(this));
	addFunc("set", nullptr, new Arg[3]{ this,"this", m_type,"o", number,"c" }, 3, new Set(this));
	addFunc("size", Long, new Arg[1]{ this,"this" }, 1, new Size(Long));
	addFunc("resize", nullptr, new Arg[2]{ this,"this", number,"c" }, 2, new Resize());

	addFunc("forEach", nullptr, new Arg[2]{ this,"this", function,"func" }, 2, new forEach());
}

DTO::SourceFile* DTO::SizedArray::create(std::string newName, Instanciable** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	SizedArrayC* g{ new SizedArrayC(newName, gens[0]) };
	add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}

#include "Array.h"

DTO::Array::ArrayC::ArrayC(std::string name, Instanciable* type)
	:Class(name, Paths::Array, GLOBAL::getClasses()->getClass(Paths::Object)), m_type(type)
{
	Number* number{ (Number*)GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	addFunc("Array", this, new Arg[2]{ this,"this",number,"c" }, 2, new ArrayConstruct(this));
	addFunc("equals", Bool, new Arg[2]{ this,"this",this,"c" }, 2, new Equals(Bool));
	addFunc("get", m_type, new Arg[2]{ this,"this", number,"c" }, 2, new Get(this));
	addFunc("set", nullptr, new Arg[3]{ this,"this", m_type,"o",number,"c" }, 3, new Set(this));
}

DTO::SourceFile* DTO::Array::create(std::string newName, Instanciable** gens, size_t genSize)
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

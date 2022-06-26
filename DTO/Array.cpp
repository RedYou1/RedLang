#include "Array.h"

DTO::Array::ArrayC::ArrayC(std::string name, Interface* type)
	:Class(name, Paths::Array, GLOBAL::getClasses()->getClass(Paths::Object)), m_type(type)
{
	Number* number{ (Number*)GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	getStatFuncs()->add("Array", new Function(new Signature("", this, new Interface * [2]{ this,number }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ArrayConstruct(this) }, 1));
	getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new Equals(Bool) }, 1));
	getFuncs()->add("Get", new Function(new Signature("", m_type, new Interface * [2]{ this, number }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new Get(this) }, 1));
	getFuncs()->add("Set", new Function(new Signature("", nullptr, new Interface * [3]{ this, m_type,number }, new std::string[3]{ "this","o","c" }, 3), new Command * [1]{ new Set(this) }, 1));
}

DTO::SourceFile* DTO::Array::create(std::string newName, SourceFile** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	if (dynamic_cast<Interface*>(gens[0]) == nullptr) {
		throw "not an interface or class.";
	}
	ArrayC* g{ new ArrayC(newName, (Interface*)gens[0]) };
	GLOBAL::getClasses()->set(newName, g);
	return g;
}

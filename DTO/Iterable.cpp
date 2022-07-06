#include "Iterable.h"

#include "FunctionClass.h"

DTO::Iterable::IterableI::IterableI(std::string name, Interface* type)
	:Interface(name, Paths::Iterable), m_type(type)
{
	FunctionClass* function{ (FunctionClass*)GLOBAL::getClasses()->getInterface(Paths::Function) };
	add("forEach", new Signature("", nullptr, new Interface * [2]{ this,function }, new std::string[2]{ "this","func" }, 2));
}

DTO::SourceFile* DTO::Iterable::create(std::string newName, SourceFile** gens, size_t genSize)
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
	IterableI* g{ new IterableI(newName, (Interface*)gens[0]) };
	GLOBAL::getClasses()->set(newName, g);
	return g;
}
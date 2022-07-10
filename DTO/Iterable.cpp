#include "Iterable.h"

#include "FunctionClass.h"

DTO::Iterable::IterableI::IterableI(std::string name, Interface* type)
	:Interface(name, Paths::Iterable), m_type(type)
{
	FunctionClass* function{ (FunctionClass*)GLOBAL::getClasses()->getInterface(Paths::Function) };
	add("forEach", new Signature("", nullptr, new Arg[2]{ this,"this", function,"func" }, 2));
}

DTO::SourceFile* DTO::Iterable::create(std::string newName, Interface** gens, size_t genSize)
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
	IterableI* g{ new IterableI(newName, gens[0]) };
	add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}
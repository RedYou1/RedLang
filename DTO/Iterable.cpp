#include "Iterable.h"

#include "FunctionClass.h"

DTO::Iterable::IterableI::IterableI(std::wstring name, Instanciable* type)
	:Interface(name, Paths::Iterable), m_type(type)
{
	FunctionClass* function{ (FunctionClass*)GLOBAL::getClasses()->getInterface(Paths::Function) };
	add(L"forEach", new Signature(L"", nullptr, new Arg[2]{ this,L"this", function,L"func" }, 2));
}

DTO::SourceFile* DTO::Iterable::create(std::wstring newName, Instanciable** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	IterableI* g{ new IterableI(newName, gens[0]) };
	add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}
#include "List.h"

#include "FunctionClass.h"

DTO::List::ListI::ListI(std::string name, Instanciable* type)
	:Interface(name, Paths::List,
		new Interface* [1]{ GLOBAL::getClasses()->getInterface(std::string(Paths::Collection) + "<" + type->getName() + ">") }, 1), m_type(type)
{
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	Interface* Number{ GLOBAL::getClasses()->getInterface(Paths::Number) };
	Interface* Array{ GLOBAL::getClasses()->getInterface(Paths::Array) };
	Interface* Class{ GLOBAL::getClasses()->getInterface(Paths::Class) };
	add("add", new Signature("", nullptr, new Arg[3]{ this,"this", Number,"i", type,"c" }, 3));
	add("addAll", new Signature("", nullptr, new Arg[3]{ this,"this", Number,"i", this,"c" }, 3));
	add("get", new Signature("", type, new Arg[2]{ this,"this", Number,"i" }, 2));
	add("indexOf", new Signature("", Number, new Arg[2]{ this,"this", type,"c" }, 2));
	add("lastIndexOf", new Signature("", Number, new Arg[2]{ this,"this", type,"c" }, 2));
	add("remove", new Signature("", type, new Arg[2]{ this,"this", Number,"i" }, 2));
	add("set", new Signature("", nullptr, new Arg[3]{ this,"this", Number,"i", type,"c" }, 3));
	add("sort", new Signature("", nullptr, new Arg[2]{ this,"this", function,"c" }, 2));
	add("subList", new Signature("", this, new Arg[3]{ this,"this", Number,"a", Number,"b" }, 3));
	add("toArray", new Signature("", Array, new Arg[2]{ this,"this", Class,"c" }, 2));
}

DTO::SourceFile* DTO::List::create(std::string newName, Instanciable** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	ListI* g{ new ListI(newName, gens[0]) };
	add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}
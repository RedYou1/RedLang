#include "Collection.h"

#include "FunctionClass.h"

DTO::Collection::CollectionI::CollectionI(std::string name, Interface* type)
	:Interface(name, Paths::Collection,
		new Interface* [1]{ GLOBAL::getClasses()->getInterface(std::string(Paths::Iterable) + "<" + type->getName() + ">") }, 1), m_type(type)
{
	Interface* Bool{ GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	Interface* Integer{ GLOBAL::getClasses()->getInterface(Paths::Integer) };
	add("add", new Signature("", nullptr, new Arg[2]{ this,"this", type,"c" }, 2));
	add("addAll", new Signature("", nullptr, new Arg[2]{ this,"this", this,"c" }, 2));
	add("clear", new Signature("", nullptr, new Arg[1]{ this,"this" }, 1));
	add("contains", new Signature("", Bool, new Arg[2]{ this,"this", type,"c" }, 2));
	add("containsAll", new Signature("", Bool, new Arg[2]{ this,"this", this,"c" }, 2));
	add("isEmpty", new Signature("", Bool, new Arg[1]{ this,"this" }, 1));
	add("remove", new Signature("", nullptr, new Arg[2]{ this,"this", type,"c" }, 2));
	add("removeAll", new Signature("", nullptr, new Arg[2]{ this,"this", this,"c" }, 2));
	add("size", new Signature("", Integer, new Arg[1]{ this,"this" }, 1));
}

DTO::SourceFile* DTO::Collection::create(std::string newName, SourceFile** gens, size_t genSize)
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
	CollectionI* g{ new CollectionI(newName, (Interface*)gens[0]) };
	GLOBAL::getClasses()->set(newName, g);
	return g;
}
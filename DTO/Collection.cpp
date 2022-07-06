#include "Collection.h"

#include "FunctionClass.h"

DTO::Collection::CollectionI::CollectionI(std::string name, Interface* type)
	:Interface(name, Paths::Collection, new Interface* [1]{ GLOBAL::getClasses()->getInterface(Paths::Iterable) }, 1), m_type(type)
{
	Interface* Bool{ GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	Interface* Integer{ GLOBAL::getClasses()->getInterface(Paths::Integer) };
	add("add", new Signature("", Bool, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2));
	add("addAll", new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2));
	add("clear", new Signature("", nullptr, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1));
	add("contains", new Signature("", Bool, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2));
	add("containsAll", new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2));
	add("isEmpty", new Signature("", Bool, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1));
	add("remove", new Signature("", Bool, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2));
	add("removeAll", new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2));
	add("size", new Signature("", Integer, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1));
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
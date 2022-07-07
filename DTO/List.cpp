#include "List.h"

#include "FunctionClass.h"

DTO::List::ListI::ListI(std::string name, Interface* type)
	:Interface(name, Paths::Collection,
		new Interface* [1]{ GLOBAL::getClasses()->getInterface(std::string(Paths::Collection) + "<" + type->getName() + ">") }, 1), m_type(type)
{
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	Interface* Number{ GLOBAL::getClasses()->getInterface(Paths::Number) };
	Interface* Array{ GLOBAL::getClasses()->getInterface(Paths::Array) };
	Interface* Class{ GLOBAL::getClasses()->getInterface(Paths::Class) };
	add("add", new Signature("", nullptr, new Interface * [3]{ this,Number,type }, new std::string[3]{ "this","i","c" }, 3));
	add("addAll", new Signature("", nullptr, new Interface * [3]{ this,Number,this }, new std::string[3]{ "this","i","c" }, 3));
	add("get", new Signature("", type, new Interface * [2]{ this,Number }, new std::string[2]{ "this","i" }, 2));
	add("indexOf", new Signature("", Number, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2));
	add("lastIndexOf", new Signature("", Number, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2));
	add("remove", new Signature("", type, new Interface * [2]{ this,Number }, new std::string[2]{ "this","i" }, 2));
	add("set", new Signature("", nullptr, new Interface * [3]{ this,Number,type }, new std::string[3]{ "this","i","c" }, 3));
	add("sort", new Signature("", nullptr, new Interface * [2]{ this,function }, new std::string[2]{ "this","c" }, 2));
	add("subList", new Signature("", this, new Interface * [3]{ this,Number,Number }, new std::string[3]{ "this","a","b" }, 3));
	add("toArray", new Signature("", Array, new Interface * [2]{ this,Class }, new std::string[2]{ "this","c" }, 2));
}

DTO::SourceFile* DTO::List::create(std::string newName, SourceFile** gens, size_t genSize)
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
	ListI* g{ new ListI(newName, (Interface*)gens[0]) };
	GLOBAL::getClasses()->set(newName, g);
	return g;
}
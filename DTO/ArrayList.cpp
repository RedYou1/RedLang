#include "ArrayList.h"

DTO::ArrayList::ArrayListC::ArrayListC(std::string name, Interface* type)
	:Class(name, Paths::SizedArray, GLOBAL::getClasses()->getClass(Paths::Object)
		, new Interface* [1]{ GLOBAL::getClasses()->getInterface(std::string(Paths::List) + "<" + type->getName() + ">") }, 1), m_type(type)
{
	Interface* Number{ GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	Interface* Long{ GLOBAL::getClasses()->getClass(Paths::Long) };
	Interface* collection{ GLOBAL::getClasses()->getInterface(Paths::Collection) };
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	Interface* Array{ GLOBAL::getClasses()->getInterface(Paths::Array) };
	Interface* Class{ GLOBAL::getClasses()->getInterface(Paths::Class) };
	getFuncs()->add("ArrayList", new Function(new Signature("", this, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ArrayConstructEmpty(this) }, 1));
	getFuncs()->add("ArrayList", new Function(new Signature("", this, new Interface * [2]{ this,Number }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ArrayConstructSize(this) }, 1));
	getFuncs()->add("ArrayList", new Function(new Signature("", this, new Interface * [2]{ this,collection }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ArrayConstructCopy(this) }, 1));
	getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new Equals(Bool) }, 1));

	getFuncs()->add("forEach", new Function(new Signature("", nullptr, new Interface * [2]{ this,function }, new std::string[2]{ "this","func" }, 2), new Command * [1]{ new forEach() }, 1));
	getFuncs()->add("add", new Function(new Signature("", nullptr, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ArrayList::add() }, 1));
	getFuncs()->add("add", new Function(new Signature("", nullptr, new Interface * [3]{ this,Number,type }, new std::string[3]{ "this","i","c" }, 3), new Command * [1]{ new addI() }, 1));
	getFuncs()->add("addAll", new Function(new Signature("", nullptr, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new addAll(this) }, 1));
	getFuncs()->add("addAll", new Function(new Signature("", nullptr, new Interface * [3]{ this,Number,this }, new std::string[3]{ "this","i","c" }, 3), new Command * [1]{ new addAllI(this) }, 1));
	getFuncs()->add("get", new Function(new Signature("", type, new Interface * [2]{ this,Number }, new std::string[2]{ "this","i" }, 2), new Command * [1]{ new ArrayList::get() }, 1));
	getFuncs()->add("indexOf", new Function(new Signature("", Number, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new indexOf() }, 1));
	getFuncs()->add("lastIndexOf", new Function(new Signature("", Number, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new lastIndexOf() }, 1));
	getFuncs()->add("clear", new Function(new Signature("", nullptr, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new clear() }, 1));
	getFuncs()->add("contains", new Function(new Signature("", Bool, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new contains() }, 1));
	getFuncs()->add("containsAll", new Function(new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new containsAll() }, 1));
	getFuncs()->add("isEmpty", new Function(new Signature("", Bool, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new isEmpty() }, 1));
	getFuncs()->add("remove", new Function(new Signature("", nullptr, new Interface * [2]{ this,type }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new remove() }, 1));
	getFuncs()->add("remove", new Function(new Signature("", type, new Interface * [2]{ this,Number }, new std::string[2]{ "this","i" }, 2), new Command * [1]{ new removeI() }, 1));
	getFuncs()->add("removeAll", new Function(new Signature("", nullptr, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new removeAll() }, 1));
	getFuncs()->add("size", new Function(new Signature("", Number, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new size() }, 1));
	getFuncs()->add("set", new Function(new Signature("", nullptr, new Interface * [3]{ this,Number,type }, new std::string[3]{ "this","i","c" }, 3), new Command * [1]{ new set() }, 1));
	getFuncs()->add("sort", new Function(new Signature("", nullptr, new Interface * [2]{ this,function }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new sort() }, 1));
	getFuncs()->add("subList", new Function(new Signature("", this, new Interface * [3]{ this,Number,Number }, new std::string[3]{ "this","a","b" }, 3), new Command * [1]{ new subList() }, 1));
	getFuncs()->add("toArray", new Function(new Signature("", Array, new Interface * [2]{ this,Class }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new toArray() }, 1));
}

DTO::SourceFile* DTO::ArrayList::create(std::string newName, SourceFile** gens, size_t genSize)
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
	ArrayListC* g{ new ArrayListC(newName, (Interface*)gens[0]) };
	GLOBAL::getClasses()->set(newName, g);
	return g;
}

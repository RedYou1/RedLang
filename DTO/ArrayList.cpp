#include "ArrayList.h"

DTO::ArrayList::ArrayListC::ArrayListC(std::string name, Interface* type)
	:Class(name, Paths::SizedArray, GLOBAL::getClasses()->getClass(Paths::Object)), m_type(type)
{
	Number* number{ (Number*)GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	LongC* Long{ (LongC*)GLOBAL::getClasses()->getClass(Paths::Long) };
	getStatFuncs()->add("ArrayList", new Function(new Signature("", this, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ArrayConstructEmpty(this) }, 1));
	getStatFuncs()->add("ArrayList", new Function(new Signature("", this, new Interface * [2]{ this,number }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ArrayConstructSize(this) }, 1));
	getStatFuncs()->add("ArrayList", new Function(new Signature("", this, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ArrayConstructCopy(this) }, 1));
	getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ this,this }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new Equals(Bool) }, 1));
	getFuncs()->add("Get", new Function(new Signature("", m_type, new Interface * [2]{ this, number }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new Get(this) }, 1));
	getFuncs()->add("Set", new Function(new Signature("", nullptr, new Interface * [3]{ this, m_type,number }, new std::string[3]{ "this","o","c" }, 3), new Command * [1]{ new Set(this) }, 1));
	getFuncs()->add("Push_Back", new Function(new Signature("", nullptr, new Interface * [2]{ this, m_type }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new PushBack() }, 1));
	getFuncs()->add("Pop_Back", new Function(new Signature("", nullptr, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new PopBack() }, 1));
	getFuncs()->add("Size", new Function(new Signature("", Long, new Interface * [1]{ this }, new std::string[1]{ "this" }, 1), new Command * [1]{ new Size(Long) }, 1));
	getFuncs()->add("Resize", new Function(new Signature("", nullptr, new Interface * [2]{ this,number }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new Resize() }, 1));
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

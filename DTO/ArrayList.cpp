#include "ArrayList.h"

DTO::ArrayList::ArrayListC::ArrayListC(std::string name, Instanciable* type)
	:Class(name, Paths::ArrayList, GLOBAL::getClasses()->getClass(Paths::Object),
		new Interface* [1]{ GLOBAL::getClasses()->checkGetInterface(std::string(Paths::List) + "<" + type->getName() + ">") }, 1), m_type(type)
{
	Interface* Number{ GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	Interface* Long{ GLOBAL::getClasses()->getClass(Paths::Long) };
	Interface* collection{ GLOBAL::getClasses()->checkGetInterface(std::string(Paths::Collection) + '<' + type->getName() + '>') };
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	Interface* Array{ GLOBAL::getClasses()->checkGetClass(std::string(Paths::Array) + '<' + type->getName() + '>') };
	Interface* Class{ GLOBAL::getClasses()->getInterface(Paths::Class) };
	addFunc("ArrayList", this, new Arg[1]{ this,"this" }, 1, new ArrayConstructEmpty(this));
	addFunc("ArrayList", this, new Arg[2]{ this,"this", Number,"c" }, 2, new ArrayConstructSize(this));
	addFunc("ArrayList", this, new Arg[2]{ this,"this", collection,"c" }, 2, new ArrayConstructCopy(this));
	addFunc("equals", Bool, new Arg[2]{ this,"this", this,"c" }, 2, new Equals(Bool));

	addFunc("forEach", nullptr, new Arg[2]{ this,"this", function,"func" }, 2, new forEach());
	addFunc("add", nullptr, new Arg[2]{ this,"this", type,"c" }, 2, new ArrayList::add());
	addFunc("add", nullptr, new Arg[3]{ this,"this", Number,"i", type,"c" }, 3, new addI());
	addFunc("addAll", nullptr, new Arg[2]{ this,"this", this,"c" }, 2, new addAll(this));
	addFunc("addAll", nullptr, new Arg[3]{ this,"this", Number,"i", this,"c" }, 3, new addAllI(this));
	addFunc("get", type, new Arg[2]{ this,"this", Number,"i" }, 2, new ArrayList::get());
	addFunc("indexOf", Long, new Arg[2]{ this,"this", type,"c" }, 2, new indexOf());
	addFunc("lastIndexOf", Long, new Arg[2]{ this,"this", type,"c" }, 2, new lastIndexOf());
	addFunc("clear", nullptr, new Arg[1]{ this,"this" }, 1, new clear());
	addFunc("contains", Bool, new Arg[2]{ this,"this", type,"c" }, 2, new contains());
	addFunc("containsAll", Bool, new Arg[2]{ this,"this", this,"c" }, 2, new ArrayList::containsAll());
	addFunc("isEmpty", Bool, new Arg[1]{ this,"this" }, 1, new isEmpty());
	addFunc("remove", nullptr, new Arg[2]{ this,"this", type,"c" }, 2, new remove());
	addFunc("remove", type, new Arg[2]{ this,"this", Number,"i" }, 2, new removeI());
	addFunc("removeAll", nullptr, new Arg[2]{ this,"this", this,"c" }, 2, new removeAll());
	addFunc("size", Long, new Arg[1]{ this,"this" }, 1, new size());
	addFunc("capacity", Long, new Arg[1]{ this,"this" }, 1, new capacity());
	addFunc("set", nullptr, new Arg[3]{ this,"this", Number,"i", type,"c" }, 3, new set());
	addFunc("sort", nullptr, new Arg[2]{ this,"this", function,"c" }, 2, new sort());
	addFunc("subList", this, new Arg[3]{ this,"this", Number,"a", Number,"b" }, 3, new subList(this));
	addFunc("toArray", Array, new Arg[2]{ this,"this", Class,"c" }, 2, new toArray(m_type));
	addFunc("ensureCapacity", nullptr, new Arg[2]{ this,"this",Number,"c" }, 2, new ensureCapacity());
	addFunc("trimToSize", nullptr, new Arg[1]{ this,"this" }, 1, new trimToSize());
}

DTO::SourceFile* DTO::ArrayList::create(std::string newName, Instanciable** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	ArrayListC* g{ new ArrayListC(newName, gens[0]) };
	GenericStatic::add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}

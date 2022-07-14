#include "ArrayList.h"

DTO::ArrayList::ArrayListC::ArrayListC(std::wstring name, Instanciable* type)
	:Class(name, Paths::ArrayList, GLOBAL::getClasses()->getClass(Paths::Object),
		new Interface* [1]{ GLOBAL::getClasses()->checkGetInterface(std::wstring(Paths::List) + L"<" + type->getName() + L">") }, 1), m_type(type)
{
	Interface* Number{ GLOBAL::getClasses()->getInterface(Paths::Number) };
	BooleanC* Bool{ (BooleanC*)GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	Interface* Long{ GLOBAL::getClasses()->getClass(Paths::Long) };
	Interface* collection{ GLOBAL::getClasses()->checkGetInterface(std::wstring(Paths::Collection) + L'<' + type->getName() + L'>') };
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	Interface* Array{ GLOBAL::getClasses()->checkGetClass(std::wstring(Paths::Array) + L'<' + type->getName() + L'>') };
	Interface* Class{ GLOBAL::getClasses()->getInterface(Paths::Class) };
	addFunc(L"ArrayList", this, new Arg[1]{ {this,L"this",nullptr,true} }, 1, new ArrayConstructEmpty(this));
	addFunc(L"ArrayList", this, new Arg[2]{ {this,L"this",nullptr,true}, {Number,L"c"} }, 2, new ArrayConstructSize(this));
	addFunc(L"ArrayList", this, new Arg[2]{ {this,L"this",nullptr,true}, {collection,L"c"} }, 2, new ArrayConstructCopy(this));
	addFunc(L"equals", Bool, new Arg[2]{ {this,L"this"}, {this,L"c"} }, 2, new Equals(Bool));

	addFunc(L"forEach", nullptr, new Arg[2]{ {this,L"this"}, { function,L"func" } }, 2, new forEach());
	addFunc(L"add", nullptr, new Arg[2]{ {this,L"this"}, {type,L"c"} }, 2, new ArrayList::add());
	addFunc(L"add", nullptr, new Arg[3]{ {this,L"this"}, {Number,L"i"}, {type,L"c"} }, 3, new addI());
	addFunc(L"addAll", nullptr, new Arg[2]{ {this,L"this"}, {this,L"c"} }, 2, new addAll(this));
	addFunc(L"addAll", nullptr, new Arg[3]{ {this,L"this"}, {Number,L"i"}, {this,L"c"} }, 3, new addAllI(this));
	addFunc(L"get", type, new Arg[2]{ {this,L"this"}, {Number,L"i"} }, 2, new ArrayList::get());
	addFunc(L"indexOf", Long, new Arg[2]{ {this,L"this"}, {type,L"c"} }, 2, new indexOf());
	addFunc(L"lastIndexOf", Long, new Arg[2]{ {this,L"this"}, {type,L"c"} }, 2, new lastIndexOf());
	addFunc(L"clear", nullptr, new Arg[1]{ {this,L"this"} }, 1, new clear());
	addFunc(L"contains", Bool, new Arg[2]{ {this,L"this"}, {type,L"c"} }, 2, new contains());
	addFunc(L"containsAll", Bool, new Arg[2]{ {this,L"this"}, {this,L"c" } }, 2, new ArrayList::containsAll());
	addFunc(L"isEmpty", Bool, new Arg[1]{ {this,L"this"} }, 1, new isEmpty());
	addFunc(L"remove", nullptr, new Arg[2]{ {this,L"this"}, {type,L"c"} }, 2, new remove());
	addFunc(L"remove", type, new Arg[2]{ {this,L"this"},{ Number,L"i"} }, 2, new removeI());
	addFunc(L"removeAll", nullptr, new Arg[2]{ {this,L"this"}, this,L"c" }, 2, new removeAll());
	addFunc(L"size", Long, new Arg[1]{ {this,L"this"} }, 1, new size());
	addFunc(L"capacity", Long, new Arg[1]{ {this,L"this"} }, 1, new capacity());
	addFunc(L"set", nullptr, new Arg[3]{ {this,L"this"}, {Number,L"i"}, {type,L"c" } }, 3, new set());
	addFunc(L"sort", nullptr, new Arg[2]{ {this,L"this"},{ function,L"c"} }, 2, new sort());
	addFunc(L"subList", this, new Arg[3]{ {this,L"this"}, {Number,L"a"}, {Number,L"b"} }, 3, new subList(this));
	addFunc(L"toArray", Array, new Arg[2]{ {this,L"this"}, {Class,L"c"} }, 2, new toArray(m_type));
	addFunc(L"ensureCapacity", nullptr, new Arg[2]{ {this,L"this"},{Number,L"c"} }, 2, new ensureCapacity());
	addFunc(L"trimToSize", nullptr, new Arg[1]{ {this,L"this"} }, 1, new trimToSize());
}

DTO::SourceFile* DTO::ArrayList::create(std::wstring newName, Instanciable** gens, size_t genSize)
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

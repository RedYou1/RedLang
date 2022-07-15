#include "Collection.h"

#include "FunctionClass.h"

DTO::Collection::CollectionI::CollectionI(std::wstring name, Instanciable* type)
	:Interface(name, Paths::Collection,
		new Interface* [1]{ GLOBAL::getClasses()->checkGetInterface(std::wstring(Paths::Iterable) + L"<" + type->getName() + L">") }, 1), m_type(type)
{
	Interface* Bool{ GLOBAL::getClasses()->getInterface(Paths::Boolean) };
	Interface* Integer{ GLOBAL::getClasses()->getInterface(Paths::Integer) };
	add(L"add", new Signature(L"", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {type,false,L"c"} }, 2));
	add(L"addAll", new Signature(L"", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {this,false,L"c"} }, 2));
	add(L"clear", new Signature(L"", { nullptr, true }, new Arg[1]{ {this,false,L"this"} }, 1));
	add(L"contains", new Signature(L"", { Bool,false }, new Arg[2]{ {this,false,L"this"}, {type,false,L"c"} }, 2));
	add(L"containsAll", new Signature(L"", { Bool,false }, new Arg[2]{ {this,false,L"this"}, {this,false,L"c"} }, 2));
	add(L"isEmpty", new Signature(L"", { Bool,false }, new Arg[1]{ {this,false,L"this"} }, 1));
	add(L"remove", new Signature(L"", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {type,false,L"c"} }, 2));
	add(L"removeAll", new Signature(L"", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {this,false,L"c"} }, 2));
	add(L"size", new Signature(L"", { Integer,false }, new Arg[1]{ {this,false,L"this"} }, 1));
}

DTO::SourceFile* DTO::Collection::create(std::wstring newName, Instanciable** gens, size_t genSize)
{
	if (genSize != 1) {
		throw "not the right number of generic type.";
	}
	if (gens[0] == nullptr) {
		throw "Null Class";
	}
	CollectionI* g{ new CollectionI(newName, gens[0]) };
	add(new GenPossibility[]{ gens[0] }, 1, g);
	return g;
}
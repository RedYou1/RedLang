#include "List.h"

#include "FunctionClass.h"

DTO::List::ListI::ListI(std::wstring name, Instanciable* type)
	:Interface(name, Paths::List,
		new Interface* [1]{ GLOBAL::getClasses()->checkGetInterface(std::wstring(Paths::Collection) + L"<" + type->getName() + L">") }, 1), m_type(type)
{
	Interface* function{ GLOBAL::getClasses()->getInterface(Paths::Function) };
	Interface* Number{ GLOBAL::getClasses()->getInterface(Paths::Number) };
	Interface* Array{ GLOBAL::getClasses()->checkGetClass(std::wstring(Paths::Array) + L'<' + type->getName() + L'>') };
	Interface* Class{ GLOBAL::getClasses()->getInterface(Paths::Class) };
	add(L"add", new Signature(L"", { nullptr, true }, new Arg[3]{ {this,false,L"this"},{ Number,false,L"i"}, {type,false,L"c"} }, 3));
	add(L"addAll", new Signature(L"", { nullptr, true }, new Arg[3]{ {this,false,L"this"}, {Number,false,L"i"}, {this,false,L"c"} }, 3));
	add(L"get", new Signature(L"", { type,true }, new Arg[2]{ {this,false,L"this"}, {Number,false,L"i"} }, 2));
	add(L"indexOf", new Signature(L"", { Number,false }, new Arg[2]{ {this,false,L"this"}, {type,false,L"c" } }, 2));
	add(L"lastIndexOf", new Signature(L"", { Number,false }, new Arg[2]{ {this,false,L"this"},{ type,false,L"c" } }, 2));
	add(L"remove", new Signature(L"", { nullptr,true }, new Arg[2]{ {this,false,L"this"}, {Number,false,L"i"} }, 2));
	add(L"set", new Signature(L"", { nullptr, true }, new Arg[3]{ {this,false,L"this"}, {Number,false,L"i"}, {type,false,L"c" } }, 3));
	add(L"sort", new Signature(L"", { nullptr, true }, new Arg[2]{ {this,false,L"this"}, {function,false,L"c" } }, 2));
	add(L"subList", new Signature(L"", { this,false }, new Arg[3]{ {this,false,L"this"}, {Number,false,L"a"},{ Number,false,L"b"} }, 3));
	add(L"toArray", new Signature(L"", { Array,false }, new Arg[2]{ {this,false,L"this"},{ Class,false,L"c" } }, 2));
}

DTO::SourceFile* DTO::List::create(std::wstring newName, Instanciable** gens, size_t genSize)
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
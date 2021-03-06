#include <iostream>

#include "../Parser/Parser.h"
#include "../DTO/RedLang.h"
#include "../DTO/MemoryFunction.h"
#include "../DTO/MemoryObject.h"
#include "../DTO/String.h"
#include "../DTO/Global.h"
#include "../DTO/Instanciable.h"

void help() {
	std::cout << "program [file]" << std::endl;
	std::cout << "[file] = A src file." << std::endl;
}

int main(int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "no path." << std::endl;
		return 0;
	}

	std::string ppath{ std::string{argv[1]} };
	std::filesystem::path path{ std::filesystem::absolute(std::wstring(ppath.begin(),ppath.end())) };
	if (!path.has_extension()) {
		throw "no extension";
	}

	if (path.extension() != ".RedSrc") {
		throw "not a RedSrc file";
	}

	DTO::RedLang::importRedLang(Parser::Parser::loadFile);

	DTO::Class* c1{ (DTO::Class*)Parser::Parser::loadFile(path) };

	DTO::MemoryObject* mem{ new DTO::MemoryObject {} };
	DTO::CommandReturn* r{ c1->getFuncs()->get(L"main", (DTO::Instanciable**)nullptr, 0)->exec(*mem, (DTO::Command**)nullptr,0) };
	if (r->isThrow()) {
		DTO::Instanciable** cc{ new DTO::Instanciable * [1] {r->getObject()->getClass()} };
		DTO::Function* func{ r->getObject()->getClass()->getFuncs()->get(L"toString",cc,1) };
		delete[] cc;
		DTO::IObject** cmds{ new DTO::IObject * [1] {r->getObject()} };
		DTO::MemoryObject _mem{ };
		DTO::CommandReturn* r2{ func->exec(_mem, cmds) };
		delete[] cmds;
		std::wcout << L"Exception catched:" << ((DTO::StringO*)r2->getObject())->m_value << std::endl;
		delete r2;
	}
	delete r;

	delete mem;
	DTO::GLOBAL::Delete();
}
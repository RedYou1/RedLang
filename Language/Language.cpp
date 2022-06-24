#include <iostream>

#include "Class.h"
#include "RedLang.h"
#include "MemoryFunction.h"
#include "MemoryObject.h"
#include "String.h"
#include "Global.h"
#include "GarbageCollector.h"

using namespace std;

void help() {
	cout << "program [file]" << endl;
	cout << "[file] = A src file." << endl;
}

int main(int argc, char** argv)
{
	{
		if (argc <= 1) {
			cout << "no path." << endl;
			return 0;
		}

		string path{ argv[1] };
		if (!path.substr(path.size() - 7, 7)._Equal(".RedSrc")) {
			cout << "path not ok." << endl;
			return 0;
		}

		RedLang::importRedLang();

		Class* c1{ (Class*)SourceFile::loadFile(path) };

		MemoryObject* mem{ new MemoryObject {} };
		CommandReturn* r{ c1->getStatFuncs()->get("main", nullptr, 0)->exec(*mem, (Command**)nullptr,0) };
		if (r->isThrow()) {
			Interface** cc{ new Interface * [1] {r->getObject()->getClass()} };
			Function* func{ r->getObject()->getClass()->getFuncs()->get("toString",cc,1) };
			delete[] cc;
			IObject** cmds{ new IObject * [1] {r->getObject()} };
			MemoryObject _mem{ };
			CommandReturn* r2{ func->exec(_mem, cmds) };
			delete[] cmds;
			std::cout << "Exception catched:" << ((StringO*)r2->getObject())->m_value << std::endl;
			delete r2;
		}
		delete r;

		delete mem;
		GLOBAL::Delete();
		GarbageCollector::Clear();
	}
}
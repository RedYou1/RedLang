#pragma once
#include <string>
#include <iostream>
#include "MemoryObject.h"
#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Function.h"
#include "MemoryFunction.h"
#include "Global.h"

namespace DTO {
	class System :public Class {
	public:
		System() :Class("System", Paths::System, GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class GetWord :public Command {
		private:
			StringC* m_o;
		public:
			GetWord(StringC* o) :m_o(o) {}
			CommandReturn* exec(MemoryObject& mem) {
				std::string a{ "" };
				std::cin >> a;
				return new CommandReturn(new StringO(m_o, a), true, false);
			}
			Command* clone()override { return new GetWord(m_o); }
		};
		class print :public Command {
		public:
			CommandReturn* exec(MemoryObject& mem) {
				IObject* s{ mem.get("s") };
				Instanciable** cc{ new Instanciable * [1] {s->getClass()} };
				Function* func{ s->getClass()->getFuncs()->get("toString",cc,1) };
				delete[] cc;
				IObject** cmds{ new IObject * [1] {s} };
				CommandReturn* r{ func->exec(mem, cmds,1) };
				delete[] cmds;
				std::cout << ((StringO*)r->getObject())->m_value;
				delete r;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new print(); }
		};
		class println :public Command {
		public:
			CommandReturn* exec(MemoryObject& mem) {
				IObject* s{ mem.get("s") };
				Instanciable** cc{ new Instanciable * [1] {s->getClass()} };
				Function* func{ s->getClass()->getFuncs()->get("toString",cc,1) };
				delete[] cc;
				IObject** cmds{ new IObject * [1] {s} };
				CommandReturn* r{ func->exec(mem, cmds,1) };
				delete[] cmds;
				std::cout << ((StringO*)r->getObject())->m_value << std::endl;
				delete r;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new println(); }
		};
	};
}
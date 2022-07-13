#pragma once
#include <string>
#include <filesystem>
#include "Exception.h"
#include "MemoryFunction.h"
#include "MemoryStatVar.h"
#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Global.h"
#include "Boolean.h"
#include "StatVar.h"
#include "File.h"

namespace DTO {
	class ClassO : public Object {
	public:
		SourceFile* m_value;

		ClassO(Class* type, SourceFile* value) :Object(type) {
			m_value = value;
		}
		Object* clone()override { return new ClassO(m_type, m_value); }
	};

	class ClassClass : public Class {
	public:
		ClassClass() : Class(L"Class", Paths::Class, GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* o{ (ClassO*)mem.get(L"this") };
				ClassO* c{ (ClassO*)mem.get(L"c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ClassConstruct :public Command {
		public:
			ClassClass* m_s;
			SourceFile* (*m_parser)(std::filesystem::path);
			ClassConstruct(ClassClass* s, SourceFile* (*parser)(std::filesystem::path)) :m_s(s), m_parser(parser) {}
			CommandReturn* exec(MemoryObject& mem) override {
				std::filesystem::path a{ ((FileO*)mem.get(L"c"))->m_path };
				Object* c{ new ClassO(m_s, m_parser(a)) };
				mem.set(L"this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ClassConstruct(m_s, m_parser); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* a{ (ClassO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, a->m_value->getName()), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};

		class GetName :public Command {
		public:
			StringC* m_s;
			GetName(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* a{ (ClassO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, a->m_value->getName()), true, false);
			}
			Command* clone()override { return new GetName(m_s); }
		};

		class GetStatVar :public Command {
		public:
			StringC* m_s;
			GetStatVar(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* a{ (ClassO*)mem.get(L"this") };
				StringO* b{ (StringO*)mem.get(L"c") };
				return new CommandReturn(((Class*)a->m_value)->getStatVars()->get(b->m_value)->GetValue(), true, false);
			}
			Command* clone()override { return new GetStatVar(m_s); }
		};

		class Invoke :public Command {
		public:
			StringC* m_s;
			Invoke(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* a{ (ClassO*)mem.get(L"this") };
				Class* c{ (Class*)a->m_value };
				StringO* s{ (StringO*)mem.get(L"func") };

				size_t size{ mem.size() - 2 };
				IObject** args{ new IObject * [size] };
				Instanciable** argsT{ new Instanciable * [size] };
				for (size_t i{ 0 }; i < size; i++) {
					args[i] = mem.get(std::to_wstring(i));
					argsT[i] = args[i]->getClass();
				}

				Function* func;
				try {
					func = c->getFuncs()->get(s->m_value, argsT, size);
				}
				catch (std::wstring e) {
					delete[] argsT;
					delete[] args;
					return new CommandReturn(new StringO(m_s, e), true, false);
				}
				delete[] argsT;

				CommandReturn* re{ func->exec(mem, args,size) };

				delete[] args;

				return re;
			}
			Command* clone()override { return new Invoke(m_s); }
		};

		class InstanceOf :public Command {
		public:
			BooleanC* m_s;
			InstanceOf(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* a{ (ClassO*)mem.get(L"this") };
				ClassO* b{ (ClassO*)mem.get(L"c") };

				if (dynamic_cast<Interface*>(a->m_value) == nullptr || dynamic_cast<Interface*>(b->m_value) == nullptr)
					return new CommandReturn(new ExceptionO(GLOBAL::getClasses()->getClass(Paths::IllegalArgumentException), L"Class.instanceOf"), false, true);

				return new CommandReturn(new BooleanO(m_s, ((Interface*)a->m_value)->instanceOf((Interface*)b->m_value)), true, false);
			}
			Command* clone()override { return new InstanceOf(m_s); }
		};
	};
}
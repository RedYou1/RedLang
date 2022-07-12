#pragma once
#pragma once
#include <string>
#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Global.h"
#include "Boolean.h"

namespace DTO {
	class FunctionO : public Object {
	public:
		Function* m_value;

		FunctionO(Class* type, Function* value) :Object(type) {
			m_value = value;
		}

		Object* clone()override { return new FunctionO(m_type, m_value); }
	};

	class FunctionClass : public Class {
	public:
		FunctionClass() : Class(L"Function", Paths::Function, GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FunctionO* o{ (FunctionO*)mem.get(L"this") };
				FunctionO* c{ (FunctionO*)mem.get(L"c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FunctionO* a{ (FunctionO*)mem.get(L"this") };
				Signature* sig{ a->m_value->getSignature() };
				std::wstring s{ L"Function<" };
				for (size_t c{ 0 }; c < sig->getArgsLen(); c++)
					s += sig->getArgs()[c].type->getName() + L",L";
				s += sig->getReturnType() == nullptr ? L"void" : sig->getReturnType()->getName();
				return new CommandReturn(new StringO(m_s, s + L">"), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
		class FunctionConstruct :public Command {
		public:
			FunctionClass* m_s;
			FunctionConstruct(FunctionClass* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get(L"c") };
				mem.set(L"this", a);
				return new CommandReturn(a, true, false);
			}
			Command* clone()override { return new FunctionConstruct(m_s); }
		};
		class Execute : public Command {
		public:
			FunctionClass* m_s;
			Execute(FunctionClass* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FunctionO* a{ (FunctionO*)mem.get(L"this") };
				size_t size{ mem.size() - 1 };
				IObject** args{ new IObject * [size] };
				for (size_t c{ 0 }; c < size; c++) {
					args[c] = mem.get(std::to_wstring(c));
				}
				MemoryObject mem2{};
				CommandReturn* q{ a->m_value->exec(mem2, args, size) };
				delete[] args;
				return q;
			}
			Command* clone()override { return new Execute(m_s); }
		};
	};
}
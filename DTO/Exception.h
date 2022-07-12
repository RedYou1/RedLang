#pragma once
#include <string>

#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Boolean.h"
#include "Global.h"

namespace DTO {
	class ExceptionO : public Object {
	public:
		std::wstring m_message;

		ExceptionO(Class* type) :Object(type), m_message(L"") {}

		ExceptionO(Class* type, std::wstring message) :Object(type), m_message(message) {}

		Object* clone()override { return new ExceptionO(m_type, m_message); }
	};

	class ExceptionC : public Class {
	public:
		ExceptionC() : Class(L"Exception", Paths::Exception, GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ExceptionO* o{ (ExceptionO*)mem.get(L"this") };
				ExceptionO* c{ (ExceptionO*)mem.get(L"c") };
				return new CommandReturn(new BooleanO(m_s, o->m_message == c->m_message), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ExceptionConstruct :public Command {
		public:
			ExceptionC* m_s;
			ExceptionConstruct(ExceptionC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				StringO* a{ (StringO*)mem.get(L"c") };
				Object* c{ new ExceptionO(m_s, a->m_value) };
				mem.set(L"this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ExceptionConstruct(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ExceptionO* a{ (ExceptionO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, a->getClass()->getName() + L":" + a->m_message), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
	};
}
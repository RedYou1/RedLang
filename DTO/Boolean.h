#pragma once
#include <string>
#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Global.h"

namespace DTO {
	class BooleanO : public Object {
	public:
		bool m_value;

		BooleanO(Class* type) :Object(type) {
			m_value = false;
		}

		BooleanO(Class* type, bool value) :Object(type) {
			m_value = value;
		}

		Object* clone()override { return new BooleanO(m_type, m_value); }
	};

	class BooleanC : public Class {
	public:
		BooleanC() : Class("Boolean", "", GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Not : public Command {
		public:
			BooleanC* m_s;
			Not(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				BooleanO* o{ (BooleanO*)mem.get("this") };
				return new CommandReturn(new BooleanO(m_s, !o->m_value), true, false);
			}
			Command* clone()override { return new Not(m_s); }
		};

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				BooleanO* o{ (BooleanO*)mem.get("this") };
				BooleanO* c{ (BooleanO*)mem.get("c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class BooleanConstruct :public Command {
		public:
			BooleanC* m_s;
			BooleanConstruct(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				Object* c{ new BooleanO(m_s, ((BooleanO*)mem.get("c"))->m_value) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new BooleanConstruct(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				BooleanO* a{ (BooleanO*)mem.get("this") };
				return new CommandReturn(new StringO(m_s, a->m_value ? "TRUE" : "FALSE"), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
	};
}
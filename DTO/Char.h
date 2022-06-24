#pragma once
#include <string>
#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Boolean.h"
#include "Global.h"

namespace DTO {
	class CharO : public Object {
	public:
		char m_value;

		CharO(Class* type) :Object(type) {
			m_value = 0;
		}

		CharO(Class* type, char value) :Object(type) {
			m_value = value;
		}
		Object* clone()override { return new CharO(m_type, m_value); }
	};

	class CharC : public Class {
	public:
		CharC() : Class("Char", "", GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				CharO* o{ (CharO*)mem.get("this") };
				CharO* c{ (CharO*)mem.get("c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class CharConstruct :public Command {
		public:
			CharC* m_s;
			CharConstruct(CharC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				CharO* a{ (CharO*)mem.get("c") };
				Object* c{ new CharO(m_s, a->m_value) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new CharConstruct(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				CharO* a{ (CharO*)mem.get("this") };
				return new CommandReturn(new StringO(m_s, std::to_string(a->m_value)), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
	};
}
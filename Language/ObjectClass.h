#pragma once
#include <string>
#include <sstream>

#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "String.h"
#include "Boolean.h"

class ClassClass;
class LongC;

class ObjectClass : public Class {
public:
	ObjectClass() : Class("Object", "", nullptr) {
	}

	class Equals :public Command {
	public:
		BooleanC* m_s;
		Equals(BooleanC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			Object* o{ mem.get("this") };
			Object* c{ mem.get("c") };
			return new CommandReturn(new BooleanO(m_s, o == c), true, false);
		}
		Command* clone()override { return new Equals(m_s); }
	};
	class ToString :public Command {
	public:
		StringC* m_s;
		ToString(StringC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			Object* o{ (Object*)mem.get("this") };
			std::stringstream ss;

			ss << o->getClass()->getName() << "@" << std::hex << (void*)o;

			return new CommandReturn(new StringO(m_s, ss.str()), true, false);
		}
		Command* clone()override { return new ToString(m_s); }
	};
	class GetClass :public Command {
	public:
		ClassClass* m_s;
		GetClass(ClassClass* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override { return new GetClass(m_s); }
	};
	class HashCode :public Command {
	public:
		LongC* m_s;
		HashCode(LongC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override { return new HashCode(m_s); }
	};
};
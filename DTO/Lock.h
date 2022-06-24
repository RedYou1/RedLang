#pragma once
#include <string>
#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Global.h"
#include <thread>
#include "Boolean.h"
#include "Integer.h"
#include "myString.h"
#include <mutex>

namespace DTO {
	class LockO : public Object {
	public:
		std::mutex* m_mutex;
		std::lock_guard<std::mutex>* m_lock;

		LockO(Class* type) :Object(type), m_mutex(new std::mutex()), m_lock(nullptr) {
		}

		~LockO() override {
			delete m_lock;
			delete m_mutex;
		}

		Object* clone()override { return new LockO(m_type); }
	};

	class LockC : public Class {
	public:
		LockC() : Class("Lock", "", GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				LockO* o{ (LockO*)mem.get("this") };
				LockO* c{ (LockO*)mem.get("c") };
				return new CommandReturn(new BooleanO(m_s, o->m_mutex == c->m_mutex), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				LockO* a{ (LockO*)mem.get("this") };
				std::string s{ a->m_lock == nullptr ? "not " : "" };
				return new CommandReturn(new StringO(m_s, "Lock is " + s + "locked"), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};

		class LockConstruct :public Command {
		public:
			LockC* m_s;
			LockConstruct(LockC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				Object* c{ new LockO(m_s) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new LockConstruct(m_s); }
		};

		class Lock :public Command {
		public:
			StringC* m_s;
			Lock(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				LockO* a{ (LockO*)mem.get("this") };
				a->m_lock = new std::lock_guard<std::mutex>(*a->m_mutex);
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Lock(m_s); }
		};

		class Unlock :public Command {
		public:
			StringC* m_s;
			Unlock(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				LockO* a{ (LockO*)mem.get("this") };
				std::lock_guard<std::mutex>* b{ a->m_lock };
				if (b == nullptr)
					return new CommandReturn(new StringO(m_s, "Not locked"), false, true);
				a->m_lock = nullptr;
				delete b;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Unlock(m_s); }
		};
	};
}
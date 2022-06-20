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
#include "FunctionClass.h"

class ThreadO : public Object {
public:
	Function* m_func;

	Object** m_args;
	size_t m_argsLen;

	std::thread* m_thread;
	bool m_joined;

	ThreadO(Class* type, Function* func, Object** args, size_t argsLen)
		:Object(type), m_func(func), m_thread(nullptr), m_joined(false),
		m_args{ args }, m_argsLen(argsLen)
	{}

	~ThreadO() override {
		if (m_thread != nullptr) {
			m_joined = true;
			m_thread->join();
			delete m_thread;
		}
		for (size_t c{ 0 }; c < m_argsLen; c++)
			GarbageCollector::Remove(m_args[c]);
		delete[] m_args;
	}

	Object* clone()override {
		Object** cmd{ new Object * [m_argsLen] };
		for (size_t c{ 0 }; c < m_argsLen; c++) {
			cmd[c] = (Object*)m_args[c]->clone();
			GarbageCollector::Add(cmd[c]);
		}
		return new ThreadO(m_type, m_func, cmd, m_argsLen);
	}
};

class ThreadC : public Class {
public:
	ThreadC() : Class("Thread", "", GLOBAL::getClasses()->getClass(Paths::Object)) {
	}

	class Equals :public Command {
	public:
		BooleanC* m_s;
		Equals(BooleanC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			ThreadO* o{ (ThreadO*)mem.get("this") };
			ThreadO* c{ (ThreadO*)mem.get("c") };
			return new CommandReturn(new BooleanO(m_s, o->m_func == c->m_func), true, false);
		}
		Command* clone()override { return new Equals(m_s); }
	};

	class ToString :public Command {
	public:
		StringC* m_s;
		ToString(StringC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			ThreadO* a{ (ThreadO*)mem.get("this") };
			std::string s{ a->m_thread == nullptr ? "not " : "" };
			return new CommandReturn(new StringO(m_s, "Thread is " + s + "running"), true, false);
		}
		Command* clone()override { return new ToString(m_s); }
	};

	class HardwareConcurrency :public Command {
	public:
		IntegerC* m_s;
		HardwareConcurrency(IntegerC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			return new CommandReturn(new IntegerO(m_s, std::thread::hardware_concurrency()), true, false);
		}
		Command* clone()override { return new HardwareConcurrency(m_s); }
	};

	class ThreadConstruct :public Command {
	public:
		ThreadC* m_s;
		ThreadConstruct(ThreadC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			FunctionO* s{ (FunctionO*)mem.get("func") };

			size_t size{ mem.size() - 2 };

			Object** args{ new Object * [size] };
			for (size_t i{ 0 }; i < size; i++) {
				args[i] = mem.get(std::to_string(i));
				GarbageCollector::Add(args[i]);
			}

			Object* c{ new ThreadO(m_s, s->m_value, args, size) };
			mem.set("this", c);
			return new CommandReturn(c, true, false);
		}
		Command* clone()override { return new ThreadConstruct(m_s); }
	};

	class Start :public Command {
	private:
		inline static void start(ThreadO* a) {
			MemoryObject* mem{ new MemoryObject{} };
			try {
				delete a->m_func->exec(*mem, a->m_args, a->m_argsLen);
			}
			catch (...) {}
			delete mem;

			if (!a->m_joined) {
				std::thread* t{ a->m_thread };
				a->m_thread = nullptr;
				t->detach();
				delete t;
			}
		}
	public:
		StringC* m_s;
		Start(StringC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			ThreadO* a{ (ThreadO*)mem.get("this") };
			if (a->m_thread != nullptr)
				return new CommandReturn(new StringO(m_s, "Thread already running"), true, true);

			a->m_joined = false;
			a->m_thread = new std::thread(start, a);

			return new CommandReturn(nullptr, true, false);
		}
		Command* clone()override { return new Start(m_s); }
	};

	class Join :public Command {
	public:
		StringC* m_s;
		Join(StringC* s) :m_s(s) {}
		CommandReturn* exec(MemoryObject& mem) override {
			ThreadO* a{ (ThreadO*)mem.get("this") };
			if (a->m_thread == nullptr)
				return new CommandReturn(nullptr, true, false);
			if (!a->m_thread->joinable())
				return new CommandReturn(new StringO(m_s, "Thread not joinable"), true, true);
			a->m_joined = true;
			a->m_thread->join();

			std::thread* t{ a->m_thread };
			a->m_thread = nullptr;
			delete t;

			return new CommandReturn(nullptr, true, false);
		}
		Command* clone()override { return new Join(m_s); }
	};
};
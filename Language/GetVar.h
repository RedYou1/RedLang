#pragma once
#include <string>

#include "MemoryVar.h"
#include "MemoryStatVar.h"
#include "Return.h"
#include "Command.h"
#include "Object.h"
#include "Class.h"
#include "Var.h"
#include "StatVar.h"
#include "Exception.h"

class GetStaticVar :public Command {
private:
	Class* m_c;
	std::string m_name;
public:
	GetStaticVar(Class* c, std::string name) :m_c(c), m_name(name) {}
	virtual ~GetStaticVar() override {}

	CommandReturn* exec(MemoryObject& mem) override {
		return new CommandReturn(m_c->getStatVars()->get(m_name)->GetValue(), false, false);
	}

	Command* clone()override { return new GetStaticVar(m_c, m_name); }
};

class GetVar :public Command {
private:
	Object* m_o;
	std::string m_name;
public:
	GetVar(Object* o, std::string name) :m_o(o), m_name(name) { GarbageCollector::Add(m_o); }
	virtual ~GetVar() override { GarbageCollector::Remove(m_o); }

	CommandReturn* exec(MemoryObject& mem) override {
		Object* cc{ m_o->get(m_name) };
		return new CommandReturn(cc, true, false);
	}
	Command* clone()override { return new GetVar((Object*)m_o->clone(), m_name); }
};

class GetVarFunc :public Command {
private:
	Command* m_o;
	std::string m_name;
public:
	GetVarFunc(Command* o, std::string name) :m_o(o), m_name(name) {}
	virtual ~GetVarFunc() override { delete m_o; }

	CommandReturn* exec(MemoryObject& mem) override {
		CommandReturn* a{ m_o->exec(mem) };
		if (a->getObject() == nullptr) {
			delete a;
			return new CommandReturn(new ExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), "GetVarFunc"), false, true);
		}
		Object* cc{ a->getObject()->get(m_name) };
		CommandReturn* r{ new CommandReturn(cc,true,false) };
		delete a;
		return r;
	}
	Command* clone()override { return new GetVarFunc(m_o->clone(), m_name); }
};
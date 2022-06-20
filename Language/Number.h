#pragma once
#include <string>

#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Boolean.h"
#include "Global.h"

class Number : public Interface {
public:
	Number() : Interface("Number", "") {
	}
};

#define Number_Constructor_A(name) \
name::name(Class* type) :Object(type), m_value(0){}

#define Number_Constructor_B(name,intType) \
name::name(Class* type,intType value) :Object(type), m_value(value){}

#define Number_Clone(name) \
Object* name::clone() {return new name(m_type,m_value);}

#define Number_Equals(name,name2) \
CommandReturn* name::Equals::exec(MemoryObject& mem) {\
	name2* o{ (name2*)mem.get("this") };\
	name2* c{ (name2*)mem.get("c") };\
	return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);\
}

#define Number_Methode_Constructor(name,name2) \
CommandReturn* name::name2::exec(MemoryObject& mem) {\
	Object* c{ (Object*)mem.get("c")->clone() };\
	mem.set("this", c);\
	return new CommandReturn(c,true,false);\
}

#define Number_Methode_ToString(name,name2) \
CommandReturn* name::ToString::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get("this") };\
	return new CommandReturn(new StringO(m_s, std::to_string(a->m_value)), true, false);\
}

#define Number_Methode_LessThan(name,name2,name3) \
CommandReturn* name::LessThan::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		return new CommandReturn(new BooleanO(m_s, a->m_value < ((name2*)b)->m_value), true, false);\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".lt"), false, true);\
}

#define Number_Methode_LessThanEquals(name,name2,name3) \
CommandReturn* name::LessThanEquals::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		return new CommandReturn(new BooleanO(m_s, a->m_value <= ((name2*)b)->m_value), true, false);\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".lte"), false, true);\
}

#define Number_Methode_GreaterThan(name,name2,name3) \
CommandReturn* name::GreaterThan::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		return new CommandReturn(new BooleanO(m_s, a->m_value > ((name2*)b)->m_value), true, false); \
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".gt"), false, true); \
}

#define Number_Methode_GreaterThanEquals(name,name2,name3)\
CommandReturn* name::GreaterThanEquals::exec(MemoryObject& mem){\
	name2* a{ (name2*)mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		return new CommandReturn(new BooleanO(m_s, a->m_value >= ((name2*)b)->m_value), true, false);\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".gte"), false, true);\
}

#define Number_Methode_Add(name,name2,name3) \
CommandReturn* name::Add::exec(MemoryObject& mem){\
	Object* a{ mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		((name2*)a)->m_value += ((name2*)b)->m_value;\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".add"), false, true);\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Sub(name,name2,name3) \
CommandReturn* name::Sub::exec(MemoryObject& mem){\
	Object* a{ mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		((name2*)a)->m_value -= ((name2*)b)->m_value;\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".sub"), false, true);\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Mult(name,name2,name3) \
CommandReturn* name::Mult::exec(MemoryObject& mem){\
	Object* a{ mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		((name2*)a)->m_value *= ((name2*)b)->m_value;\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".mult"), false, true);\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Div(name,name2,name3) \
CommandReturn* name::Div::exec(MemoryObject& mem){\
	Object* a{ mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		((name2*)a)->m_value /= ((name2*)b)->m_value;\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".div"), false, true);\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Mod(name,name2,name3) \
CommandReturn* name::Mod::exec(MemoryObject& mem){\
	Object* a{ mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		((name2*)a)->m_value %= ((name2*)b)->m_value;\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".mod"), false, true);\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_fMod(name,name2,name3)\
CommandReturn* name::Mod::exec(MemoryObject& mem){\
	Object* a{ mem.get("this") };\
	Object* b{ mem.get("other") };\
\
	if (dynamic_cast<name2*>(b))\
		((name2*)a)->m_value = std::fmod(((name2*)a)->m_value, ((name2*)b)->m_value);\
	else\
		return new CommandReturn(new IllegalArgumentExceptionO(m_except, name3".mod"), false, true);\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_toThis(name,toName)\
CommandReturn* name::toName::exec(MemoryObject& mem){return new CommandReturn(mem.get("this"),true,false);}

#define Number_Methode_toOther(name,name2,toname,oname2,intSize)\
CommandReturn* name::toname::exec(MemoryObject& mem){\
	name2* a{ (name2*)mem.get("this") };\
	return new CommandReturn(new oname2(m_s, (intSize)a->m_value), true, false);\
}
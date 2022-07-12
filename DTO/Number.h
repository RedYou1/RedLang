#pragma once
#include <string>

#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Boolean.h"
#include "Global.h"

namespace DTO {
	class NumberO : public Object {
	public:
		NumberO(Class* type) :Object(type) {}
		virtual int64_t toLong() = 0;
		virtual double_t toDouble() = 0;
		virtual bool isInteger() = 0;
	};
	class Number : public Interface {
	public:
		Number() : Interface(L"Number", Paths::Number) {
		}
	};
}
#define Number_Constructor_A(name) \
DTO::name::name(Class* type) :NumberO(type), m_value(0){}

#define Number_Constructor_B(name,intType) \
DTO::name::name(Class* type,intType value) :NumberO(type), m_value(value){}

#define Number_Clone(name) \
DTO::Object* DTO::name::clone() {return new name(m_type,m_value);}

#define Number_Equals(name,name2) \
DTO::CommandReturn* DTO::name::Equals::exec(MemoryObject& mem) {\
	name2* o{ (name2*)mem.get(L"this") };\
	name2* c{ (name2*)mem.get(L"c") };\
	return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);\
}

#define Number_Methode_Constructor(name,name2) \
DTO::CommandReturn* DTO::name::name2::exec(MemoryObject& mem) {\
	Object* c{ (Object*)mem.get(L"c")->clone() };\
	mem.set(L"this", c);\
	return new CommandReturn(c,true,false);\
}

#define Number_Methode_ToString(name,name2) \
DTO::CommandReturn* DTO::name::ToString::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get(L"this") };\
	return new CommandReturn(new StringO(m_s, std::to_wstring(a->m_value)), true, false);\
}

#define Number_Methode_LessThan(name,name2,name3) \
DTO::CommandReturn* DTO::name::LessThan::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger() && b->isInteger())\
		return new CommandReturn(new BooleanO(m_s, a->toLong() < b->toLong()), true, false);\
	else\
		return new CommandReturn(new BooleanO(m_s, a->toDouble() < b->toDouble()), true, false);\
}

#define Number_Methode_LessThanEquals(name,name2,name3) \
DTO::CommandReturn* DTO::name::LessThanEquals::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger() && b->isInteger())\
		return new CommandReturn(new BooleanO(m_s, a->toLong() <= b->toLong()), true, false);\
	else\
		return new CommandReturn(new BooleanO(m_s, a->toDouble() <= b->toDouble()), true, false);\
}

#define Number_Methode_GreaterThan(name,name2,name3) \
DTO::CommandReturn* DTO::name::GreaterThan::exec(MemoryObject& mem) {\
	name2* a{ (name2*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger() && b->isInteger())\
		return new CommandReturn(new BooleanO(m_s, a->toLong() > b->toLong()), true, false);\
	else\
		return new CommandReturn(new BooleanO(m_s, a->toDouble() > b->toDouble()), true, false);\
}

#define Number_Methode_GreaterThanEquals(name,name2,name3)\
DTO::CommandReturn* DTO::name::GreaterThanEquals::exec(MemoryObject& mem){\
	name2* a{ (name2*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger() && b->isInteger())\
		return new CommandReturn(new BooleanO(m_s, a->toLong() >= b->toLong()), true, false);\
	else\
		return new CommandReturn(new BooleanO(m_s, a->toDouble() >= b->toDouble()), true, false);\
}

#define Number_Methode_Add(name,name2,name3,type) \
DTO::CommandReturn* DTO::name::Add::exec(MemoryObject& mem){\
	NumberO* a{ (NumberO*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger())\
		((name2*)a)->m_value += (type)b->toLong();\
	else\
		((name2*)a)->m_value += (type)b->toDouble();\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Sub(name,name2,name3,type) \
DTO::CommandReturn* DTO::name::Sub::exec(MemoryObject& mem){\
	NumberO* a{ (NumberO*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger())\
		((name2*)a)->m_value -= (type)b->toLong();\
	else\
		((name2*)a)->m_value -= (type)b->toDouble();\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Mult(name,name2,name3,type) \
DTO::CommandReturn* DTO::name::Mult::exec(MemoryObject& mem){\
	NumberO* a{ (NumberO*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger())\
		((name2*)a)->m_value *= (type)b->toLong();\
	else\
		((name2*)a)->m_value *= (type)b->toDouble();\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Div(name,name2,name3,type) \
DTO::CommandReturn* DTO::name::Div::exec(MemoryObject& mem){\
	NumberO* a{ (NumberO*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	if(a->isInteger())\
		((name2*)a)->m_value /= (type)b->toLong();\
	else\
		((name2*)a)->m_value /= (type)b->toDouble();\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_Mod(name,name2,name3,type) \
DTO::CommandReturn* DTO::name::Mod::exec(MemoryObject& mem){\
	NumberO* a{ (NumberO*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	((name2*)a)->m_value %= (type)b->toLong();\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_fMod(name,name2,name3,type)\
DTO::CommandReturn* DTO::name::Mod::exec(MemoryObject& mem){\
	NumberO* a{ (NumberO*)mem.get(L"this") };\
	NumberO* b{ (NumberO*)mem.get(L"other") };\
\
	((name2*)a)->m_value = (type)std::fmodl(((name2*)a)->m_value, b->toDouble());\
	return new CommandReturn(a, true, false);\
}

#define Number_Methode_toThis(name,toName)\
DTO::CommandReturn* DTO::name::toName::exec(MemoryObject& mem){return new CommandReturn(mem.get(L"this"),true,false);}

#define Number_Methode_toOther(name,name2,toname,oname2,intSize)\
DTO::CommandReturn* DTO::name::toname::exec(MemoryObject& mem){\
	name2* a{ (name2*)mem.get(L"this") };\
	return new CommandReturn(new oname2(m_s, (intSize)a->m_value), true, false);\
}
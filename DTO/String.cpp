#include "String.h"
#include "Char.h"
#include "Boolean.h"

DTO::CommandReturn* DTO::StringC::Equals::exec(MemoryObject& mem) {
	StringO* o{ (StringO*)mem.get(L"this") };
	StringO* c{ (StringO*)mem.get(L"c") };
	return new CommandReturn(new BooleanO(m_s, o->m_value._Equal(c->m_value)), true, false);
}

DTO::CommandReturn* DTO::StringC::CharConstruct::exec(MemoryObject& mem) {
	CharO* a{ (CharO*)mem.get(L"c") };
	Object* c{ new StringO(m_s, a->m_value + L"") };
	mem.set(L"this", c);
	return new CommandReturn(c, true, false);
}


DTO::CommandReturn* DTO::StringC::StringConstruct::exec(MemoryObject& mem) {
	StringO* a{ (StringO*)mem.get(L"c") };
	Object* c{ new StringO(m_s, a->m_value) };
	mem.set(L"this", c);
	return new CommandReturn(c, true, false);
}
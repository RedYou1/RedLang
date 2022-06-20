#include "String.h"
#include "Char.h"
#include "Boolean.h"

CommandReturn* StringC::Equals::exec(MemoryObject& mem) {
	StringO* o{ (StringO*)mem.get("this") };
	StringO* c{ (StringO*)mem.get("c") };
	return new CommandReturn(new BooleanO(m_s, o->m_value._Equal(c->m_value)), true, false);
}

CommandReturn* StringC::CharConstruct::exec(MemoryObject& mem) {
	CharO* a{ (CharO*)mem.get("c") };
	Object* c{ new StringO(m_s, a->m_value + "") };
	mem.set("this", c);
	return new CommandReturn(c, true, false);
}


CommandReturn* StringC::StringConstruct::exec(MemoryObject& mem) {
	StringO* a{ (StringO*)mem.get("c") };
	Object* c{ new StringO(m_s, a->m_value) };
	mem.set("this", c);
	return new CommandReturn(c, true, false);
}
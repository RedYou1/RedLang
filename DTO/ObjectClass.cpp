#include "ObjectClass.h"
#include "ClassClass.h"
#include "Long.h"

DTO::CommandReturn* DTO::ObjectClass::GetClass::exec(MemoryObject& mem)
{
	IObject* o{ mem.get("this") };
	return new CommandReturn(new ClassO(m_s, o->getClass()), true, false);
}

DTO::CommandReturn* DTO::ObjectClass::HashCode::exec(MemoryObject& mem)
{
	IObject* o{ mem.get("this") };
	return new CommandReturn(new LongO(m_s, (uintptr_t)o), true, false);//TODO put it to ULong
}

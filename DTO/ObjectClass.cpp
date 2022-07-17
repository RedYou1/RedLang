#include "ObjectClass.h"
#include "ClassClass.h"
#include "Long.h"

DTO::CommandReturn* DTO::ObjectClass::GetClass::exec(MemoryObject& mem)
{
	IObject* o{ mem.get(L"this") };
	return new CommandReturn(new ClassO(m_s, o->getClass()), true, false);
}

DTO::CommandReturn* DTO::ObjectClass::HashCode::exec(MemoryObject& mem)
{
	IObject* o{ mem.get(L"this") };
	return new CommandReturn(new LongO(m_s, (uintptr_t)o), true, false);//TODO put it to ULong
}

DTO::CommandReturn* DTO::ObjectClass::GetPtr::exec(MemoryObject& mem)
{
	return new CommandReturn(new LongO(m_s, (int64_t)mem.get(L"this")), true, false);
}

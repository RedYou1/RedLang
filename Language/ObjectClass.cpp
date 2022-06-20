#include "ObjectClass.h"
#include "ClassClass.h"
#include "Long.h"

CommandReturn* ObjectClass::GetClass::exec(MemoryObject& mem)
{
	Object* o{ mem.get("this") };
	return new CommandReturn(new ClassO(m_s, o->getClass()), true, false);
}

CommandReturn* ObjectClass::HashCode::exec(MemoryObject& mem)
{
	Object* o{ mem.get("this") };
	return new CommandReturn(new LongO(m_s, (uintptr_t)o), true, false);//TODO put it to ULong
}

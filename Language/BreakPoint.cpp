#include "BreakPoint.h"
#include "Object.h"
#include "Global.h"

BreakPoint::BreakPoint()
{
}

CommandReturn* BreakPoint::exec(MemoryObject& mem)
{
	return new CommandReturn(new NullObject(GLOBAL::getClasses()->getClass(Paths::Object)), false, false);
}

Command* BreakPoint::clone()
{
	return new BreakPoint();
}

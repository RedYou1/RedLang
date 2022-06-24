#include "BreakPoint.h"
#include "Object.h"
#include "Global.h"

DTO::BreakPoint::BreakPoint()
{
}

DTO::CommandReturn* DTO::BreakPoint::exec(MemoryObject& mem)
{
	return new CommandReturn(new NullObject(), false, false);
}

DTO::Command* DTO::BreakPoint::clone()
{
	return new BreakPoint();
}

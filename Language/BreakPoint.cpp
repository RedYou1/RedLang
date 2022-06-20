#include "BreakPoint.h"

BreakPoint::BreakPoint()
{
}

CommandReturn* BreakPoint::exec(MemoryObject& mem)
{
	return new CommandReturn(nullptr, false, false);
}

Command* BreakPoint::clone()
{
	return new BreakPoint();
}

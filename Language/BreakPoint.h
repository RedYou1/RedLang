#pragma once
#include "Command.h"

class BreakPoint : public Command {
public:
	BreakPoint();

	CommandReturn* exec(MemoryObject& mem) override;

	Command* clone() override;
};
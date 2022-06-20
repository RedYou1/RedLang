#pragma once
#include <string>
#include "Command.h"
#include "Class.h"
#include "Object.h"

#include "Function.h"

class If : public Command {
private:
	Command* m_cond;
	Command* m_block;
public:
	If(Command* cond, Command* block);
	virtual ~If() override { delete m_cond; delete m_block; }

	CommandReturn* exec(MemoryObject& mem) override;
	Command* clone()override;
};
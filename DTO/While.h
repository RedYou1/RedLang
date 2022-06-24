#pragma once
#include <string>
#include "Command.h"
#include "Class.h"
#include "Object.h"

#include "Function.h"

namespace DTO {
	class While : public Command {
	private:
		Command* m_cond;
		Command* m_block;
	public:
		While(Command* cond, Command* block);
		virtual ~While() override { delete m_cond; delete m_block; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};

	class For : public Command {
	private:
		Command* m_init;
		Command* m_cond;
		Command* m_end;
		Command* m_block;
	public:
		For(Command* init, Command* cond, Command* end, Command* block);
		virtual ~For() override { delete m_init; delete m_cond; delete m_end; delete m_block; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
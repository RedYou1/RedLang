#pragma once
#include "Command.h"

namespace DTO {
	class If : public Command {
	private:
		Command* m_cond;
		Command* m_block;
	public:
		If(Command* cond, Command* block);
		virtual ~If() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
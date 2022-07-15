#pragma once
#include "Command.h"

namespace DTO {
	class For : public Command {
	private:
		Command* m_init;
		Command* m_cond;
		Command* m_end;
		Command* m_block;
	public:
		For(Command* init, Command* cond, Command* end, Command* block);
		virtual ~For() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
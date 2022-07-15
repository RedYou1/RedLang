#pragma once
#include "Command.h"

namespace DTO {
	class While : public Command {
	private:
		Command* m_cond;
		Command* m_block;
	public:
		While(Command* cond, Command* block);
		virtual ~While() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
#pragma once
#include "Command.h"

namespace DTO {
	class Throw :public Command {
	private:
		Command* m_cmd;
	public:
		Throw(Command* cmd);
		~Throw() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
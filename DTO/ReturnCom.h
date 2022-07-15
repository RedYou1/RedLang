#pragma once
#include "Command.h"

namespace DTO {
	class ReturnCom :public Command {
	private:
		Command* m_cmd;
	public:
		ReturnCom(Command* cmd);
		~ReturnCom() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
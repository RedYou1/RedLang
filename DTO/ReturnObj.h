#pragma once
#include "Command.h"
#include "Object.h"

namespace DTO {
	class ReturnObj :public Command {
	private:
		IObject* m_cmd;
	public:
		ReturnObj(IObject* cmd);
		~ReturnObj() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
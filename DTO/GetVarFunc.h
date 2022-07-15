#pragma once
#include "Command.h"

namespace DTO {
	class GetVarFunc :public Command {
	private:
		Command* m_o;
		std::wstring m_name;
	public:
		GetVarFunc(Command* o, std::wstring name);
		virtual ~GetVarFunc() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
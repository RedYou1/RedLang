#pragma once
#include "Command.h"

namespace DTO {
	class ObFunc :public Command {
	private:
		std::wstring m_name;
		Command** m_args;
		size_t m_argsLen;
	public:
		ObFunc(std::wstring name, Command** args, size_t argsLen);
		virtual ~ObFunc() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone()override;
	};
}
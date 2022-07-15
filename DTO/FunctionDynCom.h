#pragma once
#include "Command.h"

namespace DTO {
	class FunctionDynCom :public Command {
	private:
		Command* m_ob;
		std::wstring m_name;
		Command** m_args;
		size_t m_argsLen;
	public:
		FunctionDynCom(Command* ob, std::wstring name, Command** args, size_t argsLen);
		virtual ~FunctionDynCom() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone() override;
	};
}
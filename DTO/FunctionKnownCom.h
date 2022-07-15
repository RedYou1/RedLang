#pragma once
#include "Function.h"

namespace DTO {
	class FunctionKnownCom :public Command {
	private:
		Function* m_func;
		Command** m_args;
		size_t m_argsLen;
	public:
		FunctionKnownCom(Function* func, Command** args, size_t argsLen);
		virtual ~FunctionKnownCom() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone() override;
	};
}
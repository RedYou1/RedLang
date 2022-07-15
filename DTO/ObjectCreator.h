#pragma once
#include "Command.h"
#include "Function.h"

namespace DTO {
	class ObjectCreator :public Command {
	public:
		Function* m_func;
		IObject** m_args;
		size_t m_argsLen;
	public:
		ObjectCreator(Function* func, IObject** args, size_t argsLen);
		virtual ~ObjectCreator() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone()override;
	};
}
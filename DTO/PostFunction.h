#pragma once
#include "IFunction.h"

namespace DTO {
	class PostFunction :public IFunction {
	private:
		Command** m_commands;
		size_t m_commandLen;

		CommandReturn* exec(MemoryObject& mem);
	public:
		PostFunction(Signature* signature, Command** commands, size_t commandLen);
		virtual ~PostFunction() override;

		CommandReturn* exec(MemoryObject& pre_mem, Command** args) override;
		CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size) override;
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args) override;
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size) override;
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args) override;
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size) override;

		Command** getCommands();
		size_t getcommandLen();

		IFunction* clone() override;
	};
}
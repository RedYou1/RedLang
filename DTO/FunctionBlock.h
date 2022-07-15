#pragma once
#include "Command.h"
#include "MemoryObject.h"

namespace DTO {
	class FunctionBlock : public Command {
	private:
		Command** m_commands;
		size_t m_commandLen;
	public:
		FunctionBlock(Command** commands, size_t commandLen);
		virtual ~FunctionBlock();

		CommandReturn* exec(MemoryObject& pre_mem) override;
		CommandReturn* exec2(MemoryObject& pre_mem, std::wstring* name, IObject** args, size_t argsLen);

		Command** getCommands();
		size_t getcommandLen();

		Command* clone() override;

		void clear();
	};
}
#include "FunctionBlock.h"

DTO::FunctionBlock::FunctionBlock(Command** commands, size_t commandLen) :m_commands(commands), m_commandLen(commandLen) {}
DTO::FunctionBlock::~FunctionBlock() {
	for (size_t c(0); c < m_commandLen; c++)
		delete m_commands[c];
	delete[] m_commands;
}

DTO::CommandReturn* DTO::FunctionBlock::exec(MemoryObject& pre_mem) {
	MemoryObject mem(&pre_mem);
	for (size_t c{ 0 }; c < m_commandLen; c++) {
		CommandReturn* r{ m_commands[c]->exec(mem) };
		if (r->exitFunction())
			return r;
		delete r;
	}
	return new CommandReturn(new NullObject(), false, false);
}

DTO::CommandReturn* DTO::FunctionBlock::exec2(MemoryObject& pre_mem, std::wstring* name, IObject** args, size_t argsLen) {
	MemoryObject mem(&pre_mem);

	for (size_t c(0); c < argsLen; c++)
		mem.add(name[c], args[c], args[c]->getClass(), true);

	for (size_t c{ 0 }; c < m_commandLen; c++) {
		CommandReturn* r{ m_commands[c]->exec(mem) };
		if (r->exitFunction())
			return r;
		delete r;
	}
	return new CommandReturn(new NullObject(), false, false);
}

DTO::Command** DTO::FunctionBlock::getCommands()
{
	return m_commands;
}

size_t DTO::FunctionBlock::getcommandLen()
{
	return m_commandLen;
}

DTO::Command* DTO::FunctionBlock::clone()
{
	Command** cmds{ new Command * [m_commandLen] };
	for (size_t c(0); c < m_commandLen; c++)
		cmds[c] = m_commands[c]->clone();
	return new FunctionBlock(cmds, m_commandLen);
}

void DTO::FunctionBlock::clear()
{
	m_commandLen = 0;
	m_commands = nullptr;
}
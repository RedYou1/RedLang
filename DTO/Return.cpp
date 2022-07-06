#include "Return.h"
#include "myString.h"
#include "MemoryFunction.h"
#include "GarbageCollector.h"

DTO::Return::Return(std::string name, Interface* returnType) : m_name(name), m_returnType(returnType) { }

DTO::CommandReturn* DTO::Return::exec(MemoryObject& mem) {
	return new CommandReturn(mem.get(m_name), true, false);
}

DTO::Command* DTO::Return::clone()
{
	return new Return(m_name, m_returnType);
}

DTO::ReturnFunc::ReturnFunc(Class* _class, std::string name, Command** args, size_t argsLen)
	:m_class(_class), m_name(name), m_args(args), m_argsLen(argsLen) {
}
DTO::ReturnFunc::~ReturnFunc() {
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::ReturnFunc::exec(MemoryObject& mem) {
	Interface** argsType{ new Interface * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(mem);
		argsType[c] = args[c]->getObject()->getClass();
	}

	CommandReturn* r{ m_class->getFuncs()->get(m_name, argsType, m_argsLen)->exec(mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;
	return r;
}

DTO::Command* DTO::ReturnFunc::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		if (m_args[c] != nullptr)
			cmds[c] = m_args[c]->clone();
		else
			cmds[c] = nullptr;
	return new ReturnFunc(m_class, m_name, cmds, m_argsLen);
}


DTO::InstanceFunc::InstanceFunc(Class* _class, Command** args, size_t argsLen)
	:m_class(_class), m_args(args), m_argsLen(argsLen) {
}

DTO::InstanceFunc::~InstanceFunc() {
	for (size_t c(0); c < m_argsLen - 1; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::InstanceFunc::exec(MemoryObject& pre_mem) {
	Interface** argsType{ new Interface * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	args[0] = new CommandReturn(new Object(m_class), false, false);
	argsType[0] = m_class;

	for (size_t c{ 1 }; c < m_argsLen; c++) {
		args[c] = m_args[c - 1]->exec(pre_mem);
		argsType[c] = args[c]->getObject()->getClass();
	}
	std::string name{ std::string(m_class->getName()) };
	CommandReturn* r{ m_class->getFuncs()->get(myString{&name}.extract2(), argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	//TODO can return an exception
	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] args;
	delete[] argsType;
	r->setReturn(false);
	return r;
}

DTO::Command* DTO::InstanceFunc::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++)
		cmds[c] = m_args[c - 1]->clone();
	return new InstanceFunc(m_class, cmds, m_argsLen);
}

DTO::ReturnCom::ReturnCom(Command* cmd)
	:m_cmd(cmd)
{
	if (m_cmd == nullptr)
		throw "??";
}

DTO::ReturnCom::~ReturnCom()
{
	delete m_cmd;
}

DTO::CommandReturn* DTO::ReturnCom::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_cmd->exec(mem) };
	r->setReturn(true);
	return r;
}

DTO::Command* DTO::ReturnCom::clone()
{
	return new ReturnCom(m_cmd->clone());
}

DTO::ReturnObj::ReturnObj(IObject* cmd)
	:m_cmd(cmd)
{
	if (m_cmd == nullptr)
		throw "??";
	GarbageCollector::Add(m_cmd);
}

DTO::ReturnObj::~ReturnObj()
{
	GarbageCollector::Remove(m_cmd);
}

DTO::CommandReturn* DTO::ReturnObj::exec(MemoryObject& mem)
{
	return new CommandReturn(m_cmd, true, false);
}

DTO::Command* DTO::ReturnObj::clone()
{
	return new ReturnObj(m_cmd->clone());
}


DTO::Throw::Throw(Command* cmd)
	:m_cmd(cmd)
{
}

DTO::Throw::~Throw()
{
	delete m_cmd;
}

DTO::CommandReturn* DTO::Throw::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_cmd->exec(mem) };
	r->setThrow(true);
	return r;
}

DTO::Command* DTO::Throw::clone()
{
	return new Throw(m_cmd->clone());
}

DTO::ObjectCreator::ObjectCreator(Function* func, IObject** args, size_t argsLen)
	:m_func(func), m_args(args), m_argsLen(argsLen)
{
	for (size_t c(1); c < m_argsLen; c++)
		GarbageCollector::Add(m_args[c]);
}

DTO::ObjectCreator::~ObjectCreator()
{
	for (size_t c(1); c < m_argsLen; c++)
		GarbageCollector::Remove(m_args[c]);
	delete[] m_args;
}

DTO::CommandReturn* DTO::ObjectCreator::exec(MemoryObject& pre_mem)
{
	IObject** cmds{ new IObject * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++) {
		cmds[c] = m_args[c]->clone();
		GarbageCollector::Add(cmds[c]);
	}
	CommandReturn* r{ m_func->exec(pre_mem, cmds, m_argsLen) };
	for (size_t c(1); c < m_argsLen; c++) {
		GarbageCollector::Remove(cmds[c]);
	}
	delete[] cmds;
	return r;
}

DTO::Command* DTO::ObjectCreator::clone()
{
	IObject** cmds{ new IObject * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new ObjectCreator(m_func, cmds, m_argsLen);
}

DTO::ObFunc::ObFunc(std::string name, Command** args, size_t argsLen)
	:m_name(name), m_args(args), m_argsLen(argsLen)
{
}

DTO::ObFunc::~ObFunc()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::ObFunc::exec(MemoryObject& pre_mem)
{

	Interface** argsType{ new Interface * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(pre_mem);
		argsType[c] = args[c]->getObject()->getClass();
	}

	CommandReturn* r{ args[0]->getObject()->getClass()->getFuncs()->get(m_name, argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;
	return r;
}

DTO::Command* DTO::ObFunc::clone()
{
	throw "??";
}

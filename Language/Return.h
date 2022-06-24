#pragma once
#include <string>

#include "MemoryVariable.h"
#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Function.h"

class Return :public Command {
private:
	std::string m_name;
	Interface* m_returnType;
public:
	Return(std::string name, Interface* returnType);
	virtual ~Return() override {}

	CommandReturn* exec(MemoryObject& mem) override;
	Command* clone()override;

	static Command* parse(MemoryVariable& variables, std::string& s, MemorySourceFile& genTypes);
};

class ReturnCom :public Command {
private:
	Command* m_cmd;
public:
	ReturnCom(Command* cmd);
	~ReturnCom();

	CommandReturn* exec(MemoryObject& mem) override;
	Command* clone()override;
};

class ReturnObj :public Command {
private:
	IObject* m_cmd;
public:
	ReturnObj(IObject* cmd);
	~ReturnObj();

	CommandReturn* exec(MemoryObject& mem) override;
	Command* clone()override;
};

class Throw :public Command {
private:
	Command* m_cmd;
public:
	Throw(Command* cmd);
	~Throw();

	CommandReturn* exec(MemoryObject& mem) override;
	Command* clone()override;
};

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

class ReturnFunc :public Command {
private:
	Class* m_class;
	std::string m_name;
	Command** m_args;
	size_t m_argsLen;
public:
	ReturnFunc(Class* _class, std::string name, Command** args, size_t argsLen);
	virtual ~ReturnFunc() override;
	CommandReturn* exec(MemoryObject& pre_mem) override;
	Command* clone()override;
};

class ObFunc :public Command {
private:
	std::string m_name;
	Command** m_args;
	size_t m_argsLen;
public:
	ObFunc(std::string name, Command** args, size_t argsLen);
	virtual ~ObFunc() override;
	CommandReturn* exec(MemoryObject& pre_mem) override;
	Command* clone()override;
};

class InstanceFunc :public Command {
private:
	Class* m_class;
	Command** m_args;
	size_t m_argsLen;
public:
	InstanceFunc(Class* _class, Command** args, size_t argsLen);
	virtual ~InstanceFunc()override;
	CommandReturn* exec(MemoryObject& pre_mem) override;
	Command* clone()override;
};

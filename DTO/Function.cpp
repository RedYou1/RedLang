#include <queue>
#include "Function.h"
#include "MemoryFunction.h"
#include "CastException.h"
#include "NullException.h"
#include "File.h"

DTO::FunctionCom::FunctionCom(Class* _class, std::wstring name, Command** args, size_t argsLen)
	:m_class(_class), m_name(name), m_args(args), m_argsLen(argsLen) {}

DTO::FunctionCom::~FunctionCom()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionCom::exec(MemoryObject& mem) {

	IObject** argsType{ new IObject * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(mem);
		argsType[c] = args[c]->getObject();
	}

	CommandReturn* r{ m_class->getFuncs()->get(m_name, argsType, m_argsLen)->exec(mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;

	r->setReturn(false);
	return r;
}

DTO::Command* DTO::FunctionCom::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new FunctionCom(m_class, m_name, cmds, m_argsLen);
}

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

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& mem)
{
	if (!m_signature->getPath().empty()) {
		Class* string{ GLOBAL::getClasses()->getClass(Paths::String) };
		mem.add(L">workspace", new FileO(string, m_signature->getPath()), string, false);
	}
	for (size_t c{ 0 }; c < m_commandLen; c++) {
		CommandReturn* r{ m_commands[c]->exec(mem) };
		if (r->exitFunction()) {
			if (r->isThrow()) {
				return r;
			}
			if (dynamic_cast<NullObject*>(r->getObject()) != nullptr ||
				r->getObject()->getClass()->instanceOf(m_signature->getReturnType())) {
				return r;
			}
			else
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionReturn", r, m_signature->getReturnType()), false, true);
		}
		delete r;
	}
	return new CommandReturn(new NullObject(m_signature->getReturnType()), false, false);
}

DTO::PostFunction::PostFunction(Signature* signature, Command** commands, size_t commandLen)
	: IFunction(signature), m_commands(commands), m_commandLen(commandLen) {}

DTO::PostFunction::~PostFunction() {
	for (size_t c(0); c < m_commandLen; c++)
		delete m_commands[c];
	delete[] m_commands;
}

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& pre_mem, Command** args)
{
	return exec(pre_mem, args, m_signature->getArgsLen());
}

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& pre_mem, Command** args, size_t args_size) {
	MemoryObject mem{ &pre_mem };
	for (size_t c{ 0 }; c < m_signature->getArgsLen(); c++) {
		if (c >= args_size || args[c] == nullptr) {
			if (m_signature->getArgs()[c]._default == nullptr) {
				mem.add(m_signature->getArgs()[c].name, new NullObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
			}
			else {
				CommandReturn* q{ m_signature->getArgs()[c]._default->exec(mem) };
				mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
				delete q;
			}
		}
		else {
			CommandReturn* ob{ args[c]->exec(pre_mem) };

			if (ob->isThrow())
				return ob;

			CommandReturn* ex{ nullptr };

			if (dynamic_cast<NullObject*>(ob->getObject()) != nullptr) {
				if (m_signature->getArgs()[c]._default != nullptr) {
					MemoryObject tmem{};
					CommandReturn* q{ m_signature->getArgs()[c]._default->exec(tmem) };
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
					delete q;
					delete ob;
					continue;
				}
				else if (!m_signature->getArgs()[c].nullable)
					ex = new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getObject()->getClass()->instanceOf(m_signature->getArgs()[c].type))
				ex = new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", ob, m_signature->getArgs()[c].type), false, true);

			if (ex != nullptr) {
				delete ob;
				return ex;
			}

			mem.add(m_signature->getArgs()[c].name, ob->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
			delete ob;
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_wstring(c - m_signature->getArgsLen()), new NullObject(), nullptr, true);
		}
		else
		{
			CommandReturn* ob{ args[c]->exec(pre_mem) };

			if (ob->isThrow())
				return ob;

			CommandReturn* ex{ nullptr };

			if (dynamic_cast<NullObject*>(ob->getObject()) != nullptr) {
				if (m_signature->getArgs()[c]._default != nullptr) {
					MemoryObject tmem{};
					CommandReturn* q{ m_signature->getArgs()[c]._default->exec(tmem) };
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
					delete q;
					delete ob;
					continue;
				}
				else if (!m_signature->getArgs()[c].nullable)
					ex = new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getObject()->getClass()->instanceOf(m_signature->getArgs()[c].type))
				ex = new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", new CommandReturn(ob->getObject(), false, false), m_signature->getArgs()[c].type), false, true);

			mem.add(std::to_wstring(c - m_signature->getArgsLen()), ob->getObject(), nullptr, true);
			delete ob;

			if (ex != nullptr)
				return ex;
		}
	}

	return exec(mem);
}

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& pre_mem, IObject** args)
{
	return exec(pre_mem, args, m_signature->getArgsLen());
}

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& pre_mem, IObject** args, size_t args_size) {
	MemoryObject mem{ &pre_mem };
	for (size_t c{ 0 }; c < m_signature->getArgsLen(); c++) {
		if (c >= args_size || args[c] == nullptr) {
			if (m_signature->getArgs()[c]._default == nullptr) {
				mem.add(m_signature->getArgs()[c].name, new NullObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
			}
			else {
				CommandReturn* q{ m_signature->getArgs()[c]._default->exec(mem) };
				mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
				delete q;
			}
		}
		else {
			IObject* ob{ args[c] };

			if (dynamic_cast<NullObject*>(ob) != nullptr) {
				if (m_signature->getArgs()[c]._default != nullptr) {
					MemoryObject tmem{};
					CommandReturn* q{ m_signature->getArgs()[c]._default->exec(tmem) };
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
					delete q;
					continue;
				}
				else if (!m_signature->getArgs()[c].nullable)
					return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getClass()->instanceOf(m_signature->getArgs()[c].type))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", new CommandReturn(ob, false, false), m_signature->getArgs()[c].type), false, true);

			mem.add(m_signature->getArgs()[c].name, ob, m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_wstring(c - m_signature->getArgsLen()), new NullObject(), nullptr, true);
		}
		else {
			IObject* ob{ args[c] };

			mem.add(std::to_wstring(c - m_signature->getArgsLen()), ob, nullptr, true);
		}
	}

	return exec(mem);
}

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& pre_mem, CommandReturn** args)
{
	return exec(pre_mem, args, m_signature->getArgsLen());
}

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& pre_mem, CommandReturn** args, size_t args_size) {
	MemoryObject mem{ &pre_mem };
	for (size_t c{ 0 }; c < m_signature->getArgsLen(); c++) {
		if (c >= args_size || args[c] == nullptr) {
			if (m_signature->getArgs()[c]._default == nullptr) {
				mem.add(m_signature->getArgs()[c].name, new NullObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
			}
			else {
				CommandReturn* q{ m_signature->getArgs()[c]._default->exec(mem) };
				mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
				delete q;
			}
		}
		else {
			CommandReturn* ob{ args[c] };

			if (ob->isThrow())
				return ob;

			if (dynamic_cast<NullObject*>(ob->getObject()) != nullptr) {
				if (m_signature->getArgs()[c]._default != nullptr) {
					MemoryObject tmem{};
					CommandReturn* q{ m_signature->getArgs()[c]._default->exec(tmem) };
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
					delete q;
					continue;
				}
				else if (!m_signature->getArgs()[c].nullable)
					return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getObject()->getClass()->instanceOf(m_signature->getArgs()[c].type))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", ob, m_signature->getArgs()[c].type), false, true);

			mem.add(m_signature->getArgs()[c].name, ob->getObject(), m_signature->getArgs()[c].type, m_signature->getArgs()[c].nullable);
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_wstring(c - m_signature->getArgsLen()), new NullObject(), nullptr, m_signature->getArgs()[c].nullable);
		}
		else {
			CommandReturn* ob{ args[c] };

			if (ob->isThrow())
				return ob;

			mem.add(std::to_wstring(c - m_signature->getArgsLen()), ob->getObject(), nullptr, true);
		}
	}

	return exec(mem);
}

DTO::IFunction* DTO::PostFunction::clone()
{
	Command** cmds{ new Command * [m_commandLen] };
	for (size_t c(0); c < m_commandLen; c++)
		cmds[c] = m_commands[c]->clone();
	return new PostFunction(m_signature->clone(), cmds, m_commandLen);
}

DTO::Command* DTO::FunctionBlock::clone()
{
	Command** cmds{ new Command * [m_commandLen] };
	for (size_t c(0); c < m_commandLen; c++)
		cmds[c] = m_commands[c]->clone();
	return new FunctionBlock(cmds, m_commandLen);
}

DTO::FunctionDynCom::FunctionDynCom(Command* ob, std::wstring name, Command** args, size_t argsLen)
	:m_ob(ob), m_name(name), m_args(args), m_argsLen(argsLen)
{
}

DTO::FunctionDynCom::~FunctionDynCom()
{
	if (m_argsLen > 0 && m_args[0] != m_ob)
		delete m_ob;
	for (size_t c(0); c < m_argsLen; c++) {
		delete m_args[c];
	}
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionDynCom::exec(MemoryObject& pre_mem)
{
	CommandReturn* cm(m_ob->exec(pre_mem));
	Class* cl{ cm->getObject()->getClass() };
	delete cm;
	if (cl == nullptr)
		throw "??";

	IObject** argsType{ new IObject * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(pre_mem);
		argsType[c] = args[c]->getObject();
	}

	CommandReturn* r{ cl->getFuncs()->get(m_name, argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] argsType;
	delete[] args;

	r->setReturn(false);
	return r;
}

DTO::Command* DTO::FunctionDynCom::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new FunctionDynCom(m_ob->clone(), m_name, cmds, m_argsLen);
}

void DTO::Function::swap()
{
	std::lock_guard<std::mutex>lock(m_lock);
	if (dynamic_cast<PreFunction*>(m_function)) {
		PostFunction* f{ ((PreFunction*)m_function)->convert(this) };
		delete m_function;
		m_function = f;
	}
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, Command** args)
{
	swap();
	return m_function->exec(pre_mem, args);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, Command** args, size_t size)
{
	swap();
	return m_function->exec(pre_mem, args, size);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, IObject** args)
{
	swap();
	return m_function->exec(pre_mem, args);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, IObject** args, size_t size)
{
	swap();
	return m_function->exec(pre_mem, args, size);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, CommandReturn** args)
{
	swap();
	return m_function->exec(pre_mem, args);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, CommandReturn** args, size_t size)
{
	swap();
	return m_function->exec(pre_mem, args, size);
}

DTO::Function* DTO::Function::clone()
{
	throw "??";
}

DTO::FunctionKnownCom::FunctionKnownCom(Function* func, Command** args, size_t argsLen)
	:m_func(func), m_args(args), m_argsLen(argsLen)
{
}

DTO::FunctionKnownCom::~FunctionKnownCom()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionKnownCom::exec(MemoryObject& pre_mem)
{
	CommandReturn* r{ m_func->exec(pre_mem,m_args,m_argsLen) };
	r->setReturn(false);
	return r;
}

DTO::Command* DTO::FunctionKnownCom::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new FunctionKnownCom(m_func, cmds, m_argsLen);
}

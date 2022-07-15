#include "PostFunction.h"
#include "File.h"
#include "NullException.h"

DTO::CommandReturn* DTO::PostFunction::exec(MemoryObject& mem)
{
	if (!m_signature->getPath().empty()) {
		Class* string{ GLOBAL::getClasses()->getClass(Paths::String) };
		mem.add(L">workspace", new FileO(string, m_signature->getPath()), { string, false });
	}
	for (size_t c{ 0 }; c < m_commandLen; c++) {
		CommandReturn* r{ m_commands[c]->exec(mem) };
		if (r->exitFunction()) {
			if (r->isThrow()) {
				return r;
			}
			if (dynamic_cast<NullObject*>(r->getObject()) != nullptr ||
				r->getObject()->getClass()->instanceOf(m_signature->getReturnType().type)) {
				return r;
			}
			else
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionReturn", r, m_signature->getReturnType().type), false, true);
		}
		delete r;
	}
	return new CommandReturn(new NullObject(m_signature->getReturnType().type), false, false);
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
				mem.add(m_signature->getArgs()[c].name, new NullObject(), m_signature->getArgs()[c].type);
			}
			else {
				CommandReturn* q{ m_signature->getArgs()[c]._default->exec(mem) };
				mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
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
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
					delete q;
					delete ob;
					continue;
				}
				else if (!m_signature->getArgs()[c].type.nullable)
					ex = new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getObject()->getClass()->instanceOf(m_signature->getArgs()[c].type.type))
				ex = new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", ob, m_signature->getArgs()[c].type.type), false, true);

			if (ex != nullptr) {
				delete ob;
				return ex;
			}

			mem.add(m_signature->getArgs()[c].name, ob->getObject(), m_signature->getArgs()[c].type);
			delete ob;
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_wstring(c - m_signature->getArgsLen()), new NullObject(), { nullptr, true });
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
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
					delete q;
					delete ob;
					continue;
				}
				else if (!m_signature->getArgs()[c].type.nullable)
					ex = new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getObject()->getClass()->instanceOf(m_signature->getArgs()[c].type.type))
				ex = new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", new CommandReturn(ob->getObject(), false, false), m_signature->getArgs()[c].type.type), false, true);

			mem.add(std::to_wstring(c - m_signature->getArgsLen()), ob->getObject(), { nullptr, true });
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
				mem.add(m_signature->getArgs()[c].name, new NullObject(), m_signature->getArgs()[c].type);
			}
			else {
				CommandReturn* q{ m_signature->getArgs()[c]._default->exec(mem) };
				mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
				delete q;
			}
		}
		else {
			IObject* ob{ args[c] };

			if (dynamic_cast<NullObject*>(ob) != nullptr) {
				if (m_signature->getArgs()[c]._default != nullptr) {
					MemoryObject tmem{};
					CommandReturn* q{ m_signature->getArgs()[c]._default->exec(tmem) };
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
					delete q;
					continue;
				}
				else if (!m_signature->getArgs()[c].type.nullable)
					return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getClass()->instanceOf(m_signature->getArgs()[c].type.type))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", new CommandReturn(ob, false, false), m_signature->getArgs()[c].type.type), false, true);

			mem.add(m_signature->getArgs()[c].name, ob, m_signature->getArgs()[c].type);
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_wstring(c - m_signature->getArgsLen()), new NullObject(), { nullptr, true });
		}
		else {
			IObject* ob{ args[c] };

			mem.add(std::to_wstring(c - m_signature->getArgsLen()), ob, { nullptr, true });
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
				mem.add(m_signature->getArgs()[c].name, new NullObject(), m_signature->getArgs()[c].type);
			}
			else {
				CommandReturn* q{ m_signature->getArgs()[c]._default->exec(mem) };
				mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
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
					mem.add(m_signature->getArgs()[c].name, q->getObject(), m_signature->getArgs()[c].type);
					delete q;
					continue;
				}
				else if (!m_signature->getArgs()[c].type.nullable)
					return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Null argument at {" + m_signature->getPath().wstring() + L"} at position {" + std::to_wstring(c) + L"}"), true, true);
			}
			else if (!ob->getObject()->getClass()->instanceOf(m_signature->getArgs()[c].type.type))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"FunctionArgs", ob, m_signature->getArgs()[c].type.type), false, true);

			mem.add(m_signature->getArgs()[c].name, ob->getObject(), m_signature->getArgs()[c].type);
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_wstring(c - m_signature->getArgsLen()), new NullObject(), { nullptr, true });
		}
		else {
			CommandReturn* ob{ args[c] };

			if (ob->isThrow())
				return ob;

			mem.add(std::to_wstring(c - m_signature->getArgsLen()), ob->getObject(), { nullptr, true });
		}
	}

	return exec(mem);
}

DTO::Command** DTO::PostFunction::getCommands()
{
	return m_commands;
}

size_t DTO::PostFunction::getcommandLen()
{
	return m_commandLen;
}

DTO::IFunction* DTO::PostFunction::clone()
{
	Command** cmds{ new Command * [m_commandLen] };
	for (size_t c(0); c < m_commandLen; c++)
		cmds[c] = m_commands[c]->clone();
	return new PostFunction(m_signature->clone(), cmds, m_commandLen);
}
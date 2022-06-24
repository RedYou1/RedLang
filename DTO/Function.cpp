#include <queue>
#include "Function.h"
#include "Var.h"
#include "ReplaceVarInOb.h"
#include "MemoryFunction.h"
#include "myString.h"
#include "Return.h"
#include "Global.h"
#include "CastException.h"

DTO::FunctionCom::FunctionCom(Class* _class, std::string name, Command** args, size_t argsLen)
	:m_class(_class), m_name(name), m_args(args), m_argsLen(argsLen) {}

DTO::FunctionCom::~FunctionCom()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

DTO::CommandReturn* DTO::FunctionCom::exec(MemoryObject& mem) {

	Interface** argsType{ new Interface * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(mem);
		argsType[c] = args[c]->getObject()->getClass();
	}

	CommandReturn* r{ m_class->getStatFuncs()->get(m_name, argsType, m_argsLen)->exec(mem, args,m_argsLen) };

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

DTO::CommandReturn* DTO::FunctionBlock::exec2(MemoryObject& pre_mem, std::string* name, IObject** args, size_t argsLen) {
	MemoryObject mem(&pre_mem);

	for (size_t c(0); c < argsLen; c++)
		mem.add(name[c], args[c], args[c]->getClass());

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
		mem.add(">workspace", new StringO(string, m_signature->getPath()), string);
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
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "FunctionReturn", r, m_signature->getReturnType()), false, true);
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
		if (args[c] == nullptr) {
			mem.add(m_signature->getArgsName()[c], nullptr, m_signature->getArgsType()[c]);
		}
		else {
			CommandReturn* ob{ args[c]->exec(pre_mem) };

			if (ob->isThrow())
				return ob;

			if (dynamic_cast<Object*>(ob->getObject()) != nullptr &&
				!ob->getObject()->getClass()->instanceOf(m_signature->getArgsType()[c]))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "FunctionArgs", ob, m_signature->getArgsType()[c]), false, true);

			mem.add(m_signature->getArgsName()[c], ob->getObject(), m_signature->getArgsType()[c]);
			delete ob;
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_string(c - m_signature->getArgsLen()), nullptr, nullptr);
		}
		else {
			CommandReturn* ob{ args[c]->exec(pre_mem) };

			if (ob->isThrow())
				return ob;

			mem.add(std::to_string(c - m_signature->getArgsLen()), ob->getObject(), nullptr);
			delete ob;
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
		if (args[c] == nullptr) {
			mem.add(m_signature->getArgsName()[c], nullptr, m_signature->getArgsType()[c]);
		}
		else {
			IObject* ob{ args[c] };

			if (dynamic_cast<Object*>(ob) != nullptr &&
				!ob->getClass()->instanceOf(m_signature->getArgsType()[c]))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "FunctionArgs", new CommandReturn(ob, false, false), m_signature->getArgsType()[c]), false, true);

			mem.add(m_signature->getArgsName()[c], ob, m_signature->getArgsType()[c]);
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_string(c - m_signature->getArgsLen()), nullptr, nullptr);
		}
		else {
			IObject* ob{ args[c] };

			mem.add(std::to_string(c - m_signature->getArgsLen()), ob, nullptr);
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
		if (args[c] == nullptr) {
			mem.add(m_signature->getArgsName()[c], nullptr, m_signature->getArgsType()[c]);
		}
		else {
			CommandReturn* ob{ args[c] };

			if (ob->isThrow())
				return ob;

			if (dynamic_cast<Object*>(ob->getObject()) != nullptr &&
				!ob->getObject()->getClass()->instanceOf(m_signature->getArgsType()[c]))
				return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "FunctionArgs", ob, m_signature->getArgsType()[c]), false, true);

			mem.add(m_signature->getArgsName()[c], ob->getObject(), m_signature->getArgsType()[c]);
		}
	}

	for (size_t c{ m_signature->getArgsLen() }; c < args_size; c++) {
		if (args[c] == nullptr) {
			mem.add(std::to_string(c - m_signature->getArgsLen()), nullptr, nullptr);
		}
		else {
			CommandReturn* ob{ args[c] };

			if (ob->isThrow())
				return ob;

			mem.add(std::to_string(c - m_signature->getArgsLen()), ob->getObject(), nullptr);
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

DTO::Function* DTO::Function::parseDef(bool isNotStatic, Class* methodeOf, std::string* name, std::string* str, MemorySourceFile& genTypes) {
	MemoryVariable variables{ };
	myString s{ str };

	Interface* returnType{ nullptr };
	std::string returnTypeName{ s.extractName() };
	std::string methodeName{ std::string(methodeOf->getName()) };
	if (returnTypeName == myString{ &methodeName }.extract2()) {
		isNotStatic = true;
		*name = returnTypeName;
		returnType = methodeOf;
		s.removeUseless();
	}
	else {
		if (returnTypeName != "void") {
			if (genTypes.containKey(&returnTypeName))
				returnType = genTypes.getType(returnTypeName);
			else if (GLOBAL::getClasses()->containKey(&returnTypeName, &genTypes))
				returnType = GLOBAL::getClasses()->getType(returnTypeName);
			else
				throw "??";
			if (returnType == nullptr)
				throw "??";
		}
		s.removeUseless();
		*name = s.extractName();
		s.removeUseless();
	}


	std::queue<std::string> args{ myString(&s.extractFunc2()).split2(',') };

	size_t argsLen{ args.size() };
	if (isNotStatic) {
		argsLen++;
	}
	Interface** classes{ new Interface * [argsLen] };
	std::string* argsName{ new std::string[argsLen] };
	size_t argI{ 0 };
	if (isNotStatic) {
		argI++;
		classes[0] = methodeOf;
		argsName[0] = std::string{ "this" };
		variables.add(argsName[0], classes[0]);
	}

	while (!args.empty() && argI < argsLen) {
		std::string current = args.front();
		myString m{ &current };
		m.removeUseless();
		std::string typeName{ m.extractName() };
		if (genTypes.containKey(&typeName))
			classes[argI] = genTypes.getType(typeName);
		else if (GLOBAL::getClasses()->containKey(&typeName, &genTypes))
			classes[argI] = GLOBAL::getClasses()->getType(typeName);
		else
			throw "??";
		m.removeUseless();
		argsName[argI] = current;

		variables.add(argsName[argI], classes[argI]);

		args.pop();
		argI++;
	}

	if (!args.empty() || argI < argsLen)
		throw "?";

	s.removeUseless();

	Signature* sig{ new Signature(methodeOf->getPath(),returnType, classes, argsName, argsLen) };
	return new Function(sig, new PreFunction(*name, methodeOf, sig, s.extractFunc2(), &genTypes));
}

DTO::Command* DTO::FunctionBlock::clone()
{
	Command** cmds{ new Command * [m_commandLen] };
	for (size_t c(0); c < m_commandLen; c++)
		cmds[c] = m_commands[c]->clone();
	return new FunctionBlock(cmds, m_commandLen);
}

DTO::FunctionBlock* DTO::FunctionBlock::parse(MemoryVariable& variables, std::string* str, MemorySourceFile& genTypes) {
	myString s{ str };
	std::queue<Command*> commands{};
	std::queue<std::string> func{ myString(&s.extractFunc2()).split2(';') };

	if (!func.empty()) {
		std::string a{ func.front() };
		myString m{ &a };
		func.pop();

		while (!func.empty()) {

			m.removeUseless();
			if (m.startWith("try")) {
				while (true) {
					std::string n{ func.front() };
					myString m2{ &n };
					m2.removeUseless();
					if (m2.startWith("catch")) {
						a += n;
						func.pop();
						continue;
					}
					break;
				}
			}

			commands.push(Command::parse(nullptr, nullptr, variables, a, genTypes));

			a = func.front();
			func.pop();
		}
	}

	size_t len{ commands.size() };
	Command** cmds{ new Command * [len] };
	size_t i{ 0 };
	while (!commands.empty() && i < len) {
		cmds[i] = commands.front();
		commands.pop();
		i++;
	}
	if (i < len - 1)
		throw "?";

	s.removeUseless();
	return new FunctionBlock(cmds, len);
}

DTO::FunctionDynCom::FunctionDynCom(Command* ob, std::string name, Command** args, size_t argsLen)
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

	Interface** argsType{ new Interface * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	for (size_t c{ 0 }; c < m_argsLen; c++) {
		args[c] = m_args[c]->exec(pre_mem);
		argsType[c] = args[c]->getObject()->getClass();
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

DTO::PreFunction::PreFunction(std::string name, Class* functionOf, Signature* signature, std::string definition, MemorySourceFile* genTypes)
	:IFunction(signature), m_name(name), m_functionOf(functionOf), m_definition(definition), m_genTypes(genTypes)
{
}

DTO::PostFunction* DTO::PreFunction::convert(Function* _f)
{
	MemoryVariable variables{};
	for (size_t i{ 0 }; i < m_signature->getArgsLen(); i++)
		variables.add(m_signature->getArgsName()[i], m_signature->getArgsType()[i]);

	myString s{ &m_definition };
	std::queue<Command*> commands{};

	s.removeUseless();

	bool constructor{ false };
	{
		std::string sss{ m_functionOf->getName() };
		constructor = myString{ &sss }.startWith(m_name);
	}


	if (constructor) {
		//only classes
		if (m_definition.at(0) == ':') {
			s.extract(1);
			s.removeUseless();
			MemoryVariable var{};
			commands.push(Command::parse(m_functionOf->extends(), nullptr, var, m_definition, *m_genTypes));
			s.removeUseless();
		}
		else {
			Class* parent{ m_functionOf->extends() };
			if (!parent->getName()._Equal("Object")) {
				Interface** cc{ new Interface * [1]{parent} };
				if (Function * func{ parent->getStatFuncs()->get(parent->getName(), cc, 1) }) {
					Command** cc2{ new Command * [1]{new Return("this", m_functionOf)} };
					commands.push(new FunctionKnownCom(func, cc2, 1));
				}
				else {
					throw "no default parent constructor";
				}
				delete[] cc;
			}
		}
	}

	std::queue<std::string> func{ myString(&s.extractFunc2()).split2(';') };

	if (!func.empty()) {
		std::string a{ func.front() };
		myString m{ &a };
		func.pop();

		while (!func.empty()) {
			m.removeUseless();
			if (m.startWith("try")) {
				while (true) {
					std::string n{ func.front() };
					myString m2{ &n };
					m2.removeUseless();
					if (m2.startWith("catch")) {
						a += n;
						func.pop();
						continue;
					}
					break;
				}
			}
			commands.push(Command::parse(nullptr, nullptr, variables, a, *m_genTypes));

			a = func.front();
			func.pop();
		}
	}

	if (constructor) {
		commands.push(new Return("this", m_signature->getReturnType()));
	}

	size_t len{ commands.size() };
	Command** cmds{ new Command * [len] };
	size_t i{ 0 };
	while (!commands.empty() && i < len) {
		cmds[i] = commands.front();
		commands.pop();
		i++;
	}
	if (i < len - 1)
		throw "?";

	return new PostFunction(m_signature, cmds, len);
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

#include "Command.h"
#include "myString.h"
#include "Declaration.h"
#include "Var.h"
#include "ReplaceVar.h"
#include "ReplaceVarInOb.h"
#include "GetVar.h"
#include "MemoryFunction.h"
#include "If.h"
#include "While.h"
#include "Try.h"
#include "BreakPoint.h"

#include "Object.h"
#include "Interface.h"
#include "Class.h"
#include "MemoryObject.h"
#include "MemorySourceFile.h"

#include "MemoryVariable.h"
#include "Global.h"
#include "FunctionClass.h"
#include "ClassClass.h"
#include "ReplaceStatVar.h"

#include "GarbageCollector.h"

CommandReturn::CommandReturn(IObject* object, bool _return, bool _throw)
	:m_object(object), m_return(_return), m_throw(_throw)
{
	if (m_object == nullptr)
		throw "??";
	GarbageCollector::Add(m_object);
}

CommandReturn::CommandReturn(CommandReturn* value)
	: m_object(value->m_object), m_return(value->m_return), m_throw(value->m_throw)
{
	if (m_object == nullptr)
		throw "??";
	GarbageCollector::Add(m_object);
}

CommandReturn::~CommandReturn()
{
	GarbageCollector::Remove(m_object);
}

IObject* CommandReturn::getObject() { return m_object; }
bool CommandReturn::exitFunction()
{
	return m_return || m_throw;
}
bool CommandReturn::isReturn() { return m_return; }
void CommandReturn::setReturn(bool _return) { m_return = _return; }

bool CommandReturn::isThrow()
{
	return m_throw;
}

void CommandReturn::setThrow(bool _throw)
{
	m_throw = _throw;
}


Command* Command::parse(Class* preC, Command* pre, MemoryVariable& variables, std::string line, MemorySourceFile& genTypes) {
	if (pre != nullptr && preC != nullptr)
		throw "??";

	if (line.empty()) {
		if (preC == nullptr)
			throw "??";
		return new ReturnObj(new ClassO(GLOBAL::getClasses()->getClass(Paths::Class), preC));
	}

	myString m{ &line };
	m.removeUseless();
	std::string word{ m.extractName() };
	if (word._Equal("return")) {
		m.removeUseless();
		if (line.empty())
			return new ReturnObj(new NullObject());
		else
			return new ReturnCom(Return::parse(variables, line, genTypes));
	}
	else if (word._Equal("bp")) {
		m.removeUseless();
		return new BreakPoint();
	}
	else if (line.empty()) {
		if (word.at(word.size() - 1) == '>') {
			std::string name{ myString(&word).extract2() };
			std::queue<std::string> args{ myString(&myString(&word).extractFunc2()).split2(',') };
			size_t size{ args.size() };
			Interface** argsType{ new Interface * [size] };
			size_t i{ 0 };
			while (!args.empty() && i < size) {
				argsType[i] = nullptr;
				std::string argName{ args.front() };
				if (genTypes.containKey(&argName))
					argsType[i] = genTypes.getType(argName);
				else if (GLOBAL::getClasses()->containKey(&argName, &genTypes))
					argsType[i] = GLOBAL::getClasses()->getType(argName);
				else
					throw "??";
				if (argsType[i] == nullptr)
					throw "??";
				args.pop();
				i++;
			}
			if (!args.empty() || i < size)
				throw "??";
			FunctionO* func{ nullptr };
			if (preC->getStatFuncs()->containsI(name, argsType, size))
				func = new FunctionO(GLOBAL::getClasses()->getClass(Paths::Function), preC->getStatFuncs()->get(name, argsType, size));
			else if (preC->getFuncs()->containsI(name, argsType, size))
				func = new FunctionO(GLOBAL::getClasses()->getClass(Paths::Function), preC->getFuncs()->get(name, argsType, size));
			else
				throw "??";
			delete[] argsType;
			return new ReturnObj(func);
		}
		else if (pre != nullptr) {
			return new GetVarFunc(pre, word);
		}
		else if (preC != nullptr) {
			return new GetStaticVar(preC, word);
		}
		else if (variables.containKey(word)) {
			return new Return(word, variables.get(word));
		}
		throw "??";
	}
	else if (line.at(0) == '{') {
		MemoryVariable var{ &variables };
		return FunctionBlock::parse(var, &line, genTypes);
	}
	else if (word._Equal("if")) {
		m.removeUseless();
		std::string in{ m.extractFunc() };
		MemoryVariable var{ &variables };
		Command* cond{ Return::parse(var,in,genTypes) };
		return new If(cond, FunctionBlock::parse(var, &line, genTypes));
	}
	else if (word._Equal("while")) {
		m.removeUseless();
		std::string in{ m.extractFunc() };
		MemoryVariable var{ &variables };
		Command* cond{ Return::parse(var,in,genTypes) };
		return new While(cond, FunctionBlock::parse(var, &line, genTypes));
	}
	else if (word._Equal("for")) {
		m.removeUseless();
		std::string in{ m.extractFunc2() };

		std::queue<std::string> func{ myString(&in).split2(';') };
		if (func.size() != 3)
			throw "??";
		MemoryVariable var{ &variables };
		Command* init{ Command::parse(nullptr,nullptr,var,func.front(),genTypes) };
		func.pop();
		Command* cond{ Return::parse(var,func.front(),genTypes) };
		func.pop();
		Command* end{ Command::parse(nullptr,nullptr,var,func.front(),genTypes) };
		return new For(init, cond, end, FunctionBlock::parse(var, &line, genTypes));
	}
	else if (word._Equal("throw")) {
		m.removeUseless();
		return new Throw(Return::parse(variables, line, genTypes));
	}
	else if (word._Equal("try")) {
		m.removeUseless();
		MemoryVariable var{ &variables };
		FunctionBlock* t_try{ FunctionBlock::parse(var, &line, genTypes) };
		m.removeUseless();
		word = m.extractName();
		std::queue<std::tuple<std::string, Interface*, FunctionBlock*>> catchs{};
		while (word._Equal("catch")) {
			m.removeUseless();

			std::string arg{ m.extractFunc() };
			myString m2{ &arg };
			Interface* type{ nullptr };

			std::string typeName{ m2.extractName() };
			if (genTypes.containKey(&typeName))
				type = genTypes.getType(typeName);
			else if (GLOBAL::getClasses()->containKey(&typeName, &genTypes))
				type = GLOBAL::getClasses()->getType(typeName);
			else
				throw "??";

			if (type == nullptr)
				throw "??";

			m2.removeUseless();
			std::string name{ m2.extractName() };

			m.removeUseless();

			MemoryVariable var2{ &variables };
			var2.add(name, type);

			FunctionBlock* t_catch{ FunctionBlock::parse(var2, &line, genTypes) };
			catchs.push(std::tuple<std::string, Interface*, FunctionBlock*>(name, type, t_catch));
			m.removeUseless();
			word = m.extractName();
		}
		m.removeUseless();
		size_t size{ catchs.size() };
		std::string* names{ new std::string[size] };
		Interface** types{ new Interface * [size] };
		FunctionBlock** catches{ new FunctionBlock * [size] };

		size_t i{ 0 };
		while (!catchs.empty() && i < size) {
			std::tie(names[i], types[i], catches[i]) = catchs.front();
			catchs.pop();
			i++;
		}
		if (!catchs.empty() || i < size)
			throw "??";

		return new Try(t_try, types, names, catches, size);
	}
	switch (line.at(0)) {
	case '(': {
		if (pre == nullptr && preC == nullptr)
			throw "??";

		std::queue<std::string> args{ myString(&m.extractFunc2()).split2(',') };

		size_t size{ args.size() };
		if (pre != nullptr)
			size++;
		Command** commandsI{ new Command * [size] };
		size_t argI{ 0 };
		if (pre != nullptr) {
			commandsI[0] = pre;
			argI++;
		}

		while (!args.empty() && argI < size) {
			commandsI[argI] = Return::parse(variables, args.front(), genTypes);
			args.pop();
			argI++;
		}

		if (!args.empty())
			throw "?";

		Command* cmd{ nullptr };
		if (pre != nullptr)
			cmd = new FunctionDynCom(pre, word, commandsI, size);
		if (preC != nullptr)
			cmd = new FunctionCom(preC, word, commandsI, size);

		m.removeUseless();

		if (line.empty())
			return cmd;
		else if (line.at(0) == '.') {
			m.extract(1);
			return parse(nullptr, cmd, variables, line, genTypes);
		}

		throw "??";
	}
	case ' ': {
		m.extract(1);
		if (pre != nullptr) {
			if (line.at(0) == '=') {
				m.extract(1);
				m.removeUseless();
				return new ReplaceVarInOb(pre, true, word, Return::parse(variables, line, genTypes));
			}
		}
		if (preC != nullptr) {
			if (line.at(0) == '=') {
				m.extract(1);
				m.removeUseless();
				return new ReplaceStatVar(preC, word, Return::parse(variables, line, genTypes));
			}
		}
		else if (variables.containKey(word)) {
			if (line.at(0) == '=') {
				m.extract(1);
				m.removeUseless();
				return new ReplaceVar(variables.get(word), word, Return::parse(variables, line, genTypes));
			}
			throw "??";
		}
		else if (genTypes.containKey(&word)) {
			Interface* cl{ genTypes.getType(word) };
			m.removeUseless();
			std::string name{ m.extractName() };
			m.removeUseless();
			if (line.at(0) == '=') {
				m.extract(1);
				m.removeUseless();
				variables.add(name, cl);
				return new Declaration(cl, name, Return::parse(variables, line, genTypes));
			}
			throw "?";
		}
		else if (GLOBAL::getClasses()->containKey(&word, &genTypes)) {
			Interface* cl{ GLOBAL::getClasses()->getType(word) };
			m.removeUseless();
			std::string name{ m.extractName() };
			m.removeUseless();
			if (line.at(0) == '=') {
				m.extract(1);
				m.removeUseless();
				variables.add(name, cl);
				return new Declaration(cl, name, Return::parse(variables, line, genTypes));
			}
			throw "?";
		}
		throw "?";
	}
	case '.': {
		m.extract(1);
		if (pre != nullptr) {
			return parse(nullptr, new GetVarFunc(pre, word), variables, line, genTypes);
		}
		else if (preC != nullptr) {
			return parse(nullptr, new GetStaticVar(preC, word), variables, line, genTypes);
		}
		else if (variables.containKey(word)) {
			return parse(nullptr, new Return(word, variables.get(word)), variables, line, genTypes);
		}
		else if (genTypes.containKey(&word)) {
			return parse(genTypes.getClass(word), nullptr, variables, line, genTypes);
		}
		else if (GLOBAL::getClasses()->containKey(&word, &genTypes)) {
			return parse(GLOBAL::getClasses()->getClass(word), nullptr, variables, line, genTypes);
		}
		throw "??";
	}
	case '<': {
		return parse(GLOBAL::getClasses()->getClass(word + '<' + m.extractFunc() + '>'), nullptr, variables, line, genTypes);
	}
	default:
		throw "??";
	}
	throw "??";
}

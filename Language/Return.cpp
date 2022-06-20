#include "Return.h"
#include "myString.h"
#include "MemoryFunction.h"
#include "String.h"
#include "Char.h"
#include "GetVar.h"
#include "ClassClass.h"
#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"
#include "GarbageCollector.h"

Return::Return(std::string name, Interface* returnType) : m_name(name), m_returnType(returnType) { }

CommandReturn* Return::exec(MemoryObject& mem) {
	return new CommandReturn(mem.get(m_name), true, false);
}

Command* Return::clone()
{
	return new Return(m_name, m_returnType);
}

Command* Return::parse(MemoryVariable& variables, std::string& s, MemorySourceFile& genTypes) {
	myString ms{ &s };
	ms.removeUseless();

	if (s._Equal("NULL")) {
		return nullptr;
	}

	Command* pre{ nullptr };

	if (s.substr(0, 3)._Equal("new")) {
		ms.extract(3);
		ms.removeUseless();
		std::string className{ ms.extractName() };
		Class* sc{ GLOBAL::getClasses()->getClass(className) };//TODO new on interface when declaring the missing function
		if (sc == nullptr)
			throw "??";

		std::queue<std::string> args{ myString(&ms.extractFunc2()).split2(',') };
		size_t size{ args.size() };
		Command** commandsI{ new Command * [size] };

		size_t argI{ 0 };

		while (!args.empty() && argI < size) {
			commandsI[argI] = Return::parse(variables, args.front(), genTypes);
			args.pop();
			argI++;
		}

		if (!args.empty() || argI > size)
			throw "?";

		pre = new InstanceFunc(sc, commandsI, size + 1);
	}

	else if (s.at(0) == '"') {
		ms.extract(1);
		std::string v{ "" };
		char temp;
		while ((temp = ms.extract(1).at(0)) != '\"') {
			if (temp == '\\') {
				v += myString::backSlachChar(ms.extract(1).at(0));
			}
			else {
				v += temp;
			}
		}
		Class* string{ GLOBAL::getClasses()->getClass(Paths::String) };
		Interface** in{ new Interface * [2]{string,string} };
		Function* f{ string->getStatFuncs()->get("String",in,2) };
		delete[] in;
		pre = new ObjectCreator(f, new Object * [2]{ nullptr, new StringO(string,v) }, 2);
	}

	else if (s.at(0) == '\'') {
		ms.extract(1);
		char temp{ ms.extract(1).at(0) };
		if (temp == '\\') {
			temp = myString::backSlachChar(ms.extract(1).at(0));
		}
		if (ms.extract(1).at(0) != '\'')
			throw "?";

		Class* _char{ GLOBAL::getClasses()->getClass(Paths::Char) };
		Interface** in{ new Interface * [2]{_char,_char} };
		Function* f{ _char->getStatFuncs()->get("Char",in,2) };
		delete[] in;
		pre = new ObjectCreator(f, new Object * [2]{ nullptr, new CharO(_char,temp) }, 2);
	}
	else {
		std::string temp{ ms.extractName() };
		if (myString::is_number(temp)) {
			char a{ temp.back() };
			temp.pop_back();
			switch (a) {
			case 'b': {
				Class* Byte{ GLOBAL::getClasses()->getClass(Paths::Byte) };
				Interface** in{ new Interface * [2]{Byte,Byte} };
				Function* f{ Byte->getStatFuncs()->get("Byte",in,2) };
				delete[] in;
				pre = new ObjectCreator(f, new Object * [2]{ nullptr, new ByteO(Byte,(int8_t)std::strtoll(temp.c_str(),NULL,10)) }, 2);
				break;
			}case 's': {
				Class* Short{ GLOBAL::getClasses()->getClass(Paths::Short) };
				Interface** in{ new Interface * [2]{Short,Short} };
				Function* f{ Short->getStatFuncs()->get("Short",in,2) };
				delete[] in;
				pre = new ObjectCreator(f, new Object * [2]{ nullptr, new ShortO(Short,(int16_t)std::strtoll(temp.c_str(),NULL,10)) }, 2);
				break;
			}case 'i': {
				Class* Integer{ GLOBAL::getClasses()->getClass(Paths::Interger) };
				Interface** in{ new Interface * [2]{Integer,Integer} };
				Function* f{ Integer->getStatFuncs()->get("Integer",in,2) };
				delete[] in;
				pre = new ObjectCreator(f, new Object * [2]{ nullptr, new IntegerO(Integer,(int32_t)std::strtoll(temp.c_str(),NULL,10)) }, 2);
				break;
			}case 'f': {
				Class* Float{ GLOBAL::getClasses()->getClass(Paths::Float) };
				Interface** in{ new Interface * [2]{Float,Float} };
				Function* f{ Float->getStatFuncs()->get("Float",in,2) };
				delete[] in;
				pre = new ObjectCreator(f, new Object * [2]{ nullptr, new FloatO(Float,(float_t)std::strtod(temp.c_str(),NULL)) }, 2);
				break;
			}case 'l': {
				Class* Long{ GLOBAL::getClasses()->getClass(Paths::Long) };
				Interface** in{ new Interface * [2]{Long,Long} };
				Function* f{ Long->getStatFuncs()->get("Long",in,2) };
				delete[] in;
				pre = new ObjectCreator(f, new Object * [2]{ nullptr,new LongO(Long,(int64_t)std::strtoll(temp.c_str(),NULL,10)) }, 2);
				break;
			}case 'd': {
				Class* Double{ GLOBAL::getClasses()->getClass(Paths::Double) };
				Interface** in{ new Interface * [2]{Double,Double} };
				Function* f{ Double->getStatFuncs()->get("Double",in,2) };
				delete[] in;
				pre = new ObjectCreator(f, new Object * [2]{ nullptr, new DoubleO(Double,(double_t)std::strtod(temp.c_str(),NULL)) }, 2);
				break;
			}default: {
				throw "??";
			}
			}
		}
		else if (temp._Equal("true")) {
			Class* Bool{ GLOBAL::getClasses()->getClass(Paths::Boolean) };
			Interface** in{ new Interface * [2]{Bool,Bool} };
			Function* f{ Bool->getStatFuncs()->get("Boolean",in,2) };
			delete[] in;
			pre = new ObjectCreator(f, new Object * [2]{ nullptr, new BooleanO(Bool, true) }, 2);
		}
		else if (temp._Equal("false")) {
			Class* Bool{ GLOBAL::getClasses()->getClass(Paths::Boolean) };
			Interface** in{ new Interface * [2]{Bool,Bool} };
			Function* f{ Bool->getStatFuncs()->get("Boolean",in,2) };
			delete[] in;
			pre = new ObjectCreator(f, new Object * [2]{ nullptr, new BooleanO(Bool, false) }, 2);
		}
		else
			s = temp + s;
	}
	std::string arg1{ "" };

	if (pre == nullptr) {
		arg1 = ms.extract2();

		if (variables.containKey(arg1)) {
			pre = new Return(arg1, variables.get(arg1));
		}
		else if (genTypes.containKey(arg1)) {
			ms.extract(1);
			SourceFile* c{ genTypes.get(arg1) };
			if (dynamic_cast<Class*>(c))
				pre = Command::parse((Class*)c, nullptr, variables, s, genTypes);
			else {
				if (!s.empty())
					throw "??";
				return new ReturnObj(new ClassO(GLOBAL::getClasses()->getClass(Paths::Class), c));
			}
		}
		else if (GLOBAL::getClasses()->containKey(arg1)) {
			ms.extract(1);
			SourceFile* c{ GLOBAL::getClasses()->get(arg1) };
			if (dynamic_cast<Class*>(c))
				pre = Command::parse((Class*)c, nullptr, variables, s, genTypes);
			else {
				if (!s.empty())
					throw "??";
				return new ReturnObj(new ClassO(GLOBAL::getClasses()->getClass(Paths::Class), c));
			}
		}
		else {
			throw "??";
		}
	}

	while (!s.empty() && s.at(0) == '.') {
		Class* args1C{ nullptr };
		ms.extract(1);
		std::string arg2{ ms.extractName() };
		if (s.empty()) {
			if (pre == nullptr)
				throw "??";
			return new GetVarFunc(pre, arg2);
		}
		else if (s.at(0) == '.') {
			if (pre == nullptr) {
				pre = new Return(arg2, variables.get(arg2));
			}
			else {
				pre = new GetVarFunc(pre, arg2);
			}
		}
		else if (s.at(0) == '(') {

			std::queue<std::string> args{ myString(&ms.extractFunc2()).split2(',') };
			size_t size{ args.size() + 1 };
			Command** commandsI{ new Command * [size] };
			if (pre == nullptr)
				pre = new Return(arg1, variables.get(arg1));
			size_t argI{ 1 };

			commandsI[0] = pre;

			while (!args.empty() && argI < size) {
				commandsI[argI] = Return::parse(variables, args.front(), genTypes);
				args.pop();
				argI++;
			}

			if (!args.empty() || argI > size)
				throw "??";

			pre = new ObFunc(arg2, commandsI, size);
		}
	}

	if (pre == nullptr)
		return new Return(arg1, variables.get(arg1));
	else
		return pre;
}

ReturnFunc::ReturnFunc(Class* _class, std::string name, Command** args, size_t argsLen)
	:m_class(_class), m_name(name), m_args(args), m_argsLen(argsLen) {
}
ReturnFunc::~ReturnFunc() {
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

CommandReturn* ReturnFunc::exec(MemoryObject& mem) {
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
	return r;
}

Command* ReturnFunc::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		if (m_args[c] != nullptr)
			cmds[c] = m_args[c]->clone();
		else
			cmds[c] = nullptr;
	return new ReturnFunc(m_class, m_name, cmds, m_argsLen);
}


InstanceFunc::InstanceFunc(Class* _class, Command** args, size_t argsLen)
	:m_class(_class), m_args(args), m_argsLen(argsLen) {
}

InstanceFunc::~InstanceFunc() {
	for (size_t c(0); c < m_argsLen - 1; c++)
		delete m_args[c];
	delete[] m_args;
}

CommandReturn* InstanceFunc::exec(MemoryObject& pre_mem) {
	Interface** argsType{ new Interface * [m_argsLen] };
	CommandReturn** args{ new CommandReturn * [m_argsLen] };

	args[0] = new CommandReturn(new Object(m_class), false, false);
	argsType[0] = m_class;

	for (size_t c{ 1 }; c < m_argsLen; c++) {
		args[c] = m_args[c - 1]->exec(pre_mem);
		argsType[c] = args[c]->getObject()->getClass();
	}
	std::string name{ std::string(m_class->getName()) };
	CommandReturn* r{ m_class->getStatFuncs()->get(myString{&name}.extract2(), argsType, m_argsLen)->exec(pre_mem, args,m_argsLen) };

	//TODO can return an exception
	for (size_t c{ 0 }; c < m_argsLen; c++) {
		delete args[c];
	}
	delete[] args;
	delete[] argsType;
	CommandReturn* c{ new CommandReturn(r->getObject(), false, false) };
	delete r;
	return c;
}

Command* InstanceFunc::clone()
{
	Command** cmds{ new Command * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++)
		cmds[c] = m_args[c - 1]->clone();
	return new InstanceFunc(m_class, cmds, m_argsLen);
}

ReturnCom::ReturnCom(Command* cmd)
	:m_cmd(cmd)
{}

ReturnCom::~ReturnCom()
{
	delete m_cmd;
}

CommandReturn* ReturnCom::exec(MemoryObject& mem)
{
	if (m_cmd == nullptr)
		return new CommandReturn(nullptr, true, false);
	CommandReturn* r{ m_cmd->exec(mem) };
	r->setReturn(true);
	return r;
}

Command* ReturnCom::clone()
{
	return new ReturnCom(m_cmd->clone());
}

ReturnObj::ReturnObj(Object* cmd)
	:m_cmd(cmd)
{
	GarbageCollector::Add(m_cmd);
}

ReturnObj::~ReturnObj()
{
	GarbageCollector::Remove(m_cmd);
}

CommandReturn* ReturnObj::exec(MemoryObject& mem)
{
	return new CommandReturn(m_cmd, true, false);
}

Command* ReturnObj::clone()
{
	return new ReturnObj((Object*)m_cmd->clone());
}


Throw::Throw(Command* cmd)
	:m_cmd(cmd)
{
}

Throw::~Throw()
{
	delete m_cmd;
}

CommandReturn* Throw::exec(MemoryObject& mem)
{
	CommandReturn* r{ m_cmd->exec(mem) };
	r->setThrow(true);
	return r;
}

Command* Throw::clone()
{
	return new Throw(m_cmd->clone());
}

ObjectCreator::ObjectCreator(Function* func, Object** args, size_t argsLen)
	:m_func(func), m_args(args), m_argsLen(argsLen)
{
	for (size_t c(1); c < m_argsLen; c++)
		GarbageCollector::Add(m_args[c]);
}

ObjectCreator::~ObjectCreator()
{
	for (size_t c(1); c < m_argsLen; c++)
		GarbageCollector::Remove(m_args[c]);
	delete[] m_args;
}

CommandReturn* ObjectCreator::exec(MemoryObject& pre_mem)
{
	Object** cmds{ new Object * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++) {
		cmds[c] = (Object*)m_args[c]->clone();
		GarbageCollector::Add(cmds[c]);
	}
	CommandReturn* r{ m_func->exec(pre_mem, cmds, m_argsLen) };
	for (size_t c(1); c < m_argsLen; c++) {
		GarbageCollector::Remove(cmds[c]);
	}
	delete[] cmds;
	return r;
}

Command* ObjectCreator::clone()
{
	Object** cmds{ new Object * [m_argsLen] };
	cmds[0] = nullptr;
	for (size_t c(1); c < m_argsLen; c++)
		cmds[c] = m_args[c]->clone();
	return new ObjectCreator(m_func, cmds, m_argsLen);
}

ObFunc::ObFunc(std::string name, Command** args, size_t argsLen)
	:m_name(name), m_args(args), m_argsLen(argsLen)
{
}

ObFunc::~ObFunc()
{
	for (size_t c(0); c < m_argsLen; c++)
		delete m_args[c];
	delete[] m_args;
}

CommandReturn* ObFunc::exec(MemoryObject& pre_mem)
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

Command* ObFunc::clone()
{
	throw "??";
}

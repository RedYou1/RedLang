#include "Parser.h"
#include <fstream>
#include <filesystem>
#include "GenericDef.h"
#include "FunctionDef.h"
#include "../DTO/ClassClass.h"
#include "../DTO/Return.h"
#include "../DTO/GLOBAL.h"
#include "../DTO/Declaration.h"
#include "../DTO/myString.h"
#include "../DTO/BreakPoint.h"
#include "../DTO/FunctionClass.h"
#include "../DTO/GetVar.h"
#include "../DTO/While.h"
#include "../DTO/If.h"
#include "../DTO/Try.h"
#include "../DTO/ReplaceStatVar.h"
#include "../DTO/ReplaceVar.h"
#include "../DTO/ReplaceVarInOb.h"
#include "../DTO/Byte.h"
#include "../DTO/Short.h"
#include "../DTO/Integer.h"
#include "../DTO/Float.h"
#include "../DTO/Long.h"
#include "../DTO/Double.h"
#include "../DTO/Char.h"


DTO::Command* Parser::Parser::parseCommand(DTO::Class* preC, DTO::Command* pre, DTO::MemoryVariable& variables, std::wstring line, DTO::MemorySourceFile& genTypes)
{
	if (pre != nullptr && preC != nullptr)
		throw "??";

	if (line.empty()) {
		if (preC == nullptr)
			throw "??";
		return new DTO::ReturnObj(new DTO::ClassO(DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Class), preC));
	}

	DTO::myString m{ &line };
	m.removeUseless();
	std::wstring word{ m.extractName() };

	if (pre == nullptr && preC == nullptr) {
		if (word._Equal(L"return")) {
			m.removeUseless();
			if (line.empty())
				return new DTO::ReturnObj(new DTO::NullObject());
			else
				return new DTO::ReturnCom(parseReturn(variables, line, genTypes));
		}
		else if (word._Equal(L"bp")) {
			m.removeUseless();
			return new DTO::BreakPoint();
		}
		else if (line.at(0) == L'{') {
			DTO::MemoryVariable var{ &variables };
			return parseFunctionBlock(var, &line, genTypes);
		}
		else if (word._Equal(L"if")) {
			m.removeUseless();
			std::wstring in{ m.extractFunc() };
			DTO::MemoryVariable var{ &variables };
			DTO::Command* cond{ parseReturn(var,in,genTypes) };
			return new DTO::If(cond, parseFunctionBlock(var, &line, genTypes));
		}
		else if (word._Equal(L"while")) {
			m.removeUseless();
			std::wstring in{ m.extractFunc() };
			DTO::MemoryVariable var{ &variables };
			DTO::Command* cond{ parseReturn(var,in,genTypes) };
			return new DTO::While(cond, parseFunctionBlock(var, &line, genTypes));
		}
		else if (word._Equal(L"for")) {
			m.removeUseless();
			std::wstring in{ m.extractFunc2() };

			std::queue<std::wstring> func{ DTO::myString(&in).split2(';') };
			if (func.size() != 3)
				throw "??";
			DTO::MemoryVariable var{ &variables };
			DTO::Command* init{ parseCommand(nullptr,nullptr,var,func.front(),genTypes) };
			func.pop();
			DTO::Command* cond{ parseReturn(var,func.front(),genTypes) };
			func.pop();
			DTO::Command* end{ parseCommand(nullptr,nullptr,var,func.front(),genTypes) };
			return new DTO::For(init, cond, end, parseFunctionBlock(var, &line, genTypes));
		}
		else if (word._Equal(L"forEach")) {
			m.removeUseless();
			std::wstring in{ m.extractFunc2() };

			DTO::myString inS{ &in };

			inS.removeUseless();
			inS.extract(1);
			inS.removeUseless();
			std::wstring typeName{ inS.extractName() };
			DTO::Instanciable* type{ DTO::GLOBAL::getClasses()->getType(typeName) };
			inS.removeUseless();
			std::wstring name{ inS.extract2() };
			inS.removeUseless();
			inS.extract(1);
			inS.removeUseless();
			in = in.substr(0, in.find_last_of(')'));
			DTO::MemoryVariable var{ &variables };
			var.add(name, type);
			DTO::Command* iterable{ parseReturn(var,in,genTypes) };

			DTO::FunctionBlock* fb{ parseFunctionBlock(var, &line, genTypes) };
			DTO::Function* func{ new DTO::Function(new DTO::Signature(L"",nullptr,new DTO::Arg[1]{type,name},1),fb->getCommands(),fb->getcommandLen()) };

			fb->clear();
			delete fb;

			return new DTO::ForEach(type, name, iterable, func);
		}
		else if (word._Equal(L"throw")) {
			m.removeUseless();
			return new DTO::Throw(parseReturn(variables, line, genTypes));
		}
		else if (word._Equal(L"try")) {
			m.removeUseless();
			DTO::MemoryVariable var{ &variables };
			DTO::FunctionBlock* t_try{ parseFunctionBlock(var, &line, genTypes) };
			m.removeUseless();
			word = m.extractName();
			std::queue<std::tuple<std::wstring, DTO::Interface*, DTO::FunctionBlock*>> catchs{};
			while (word._Equal(L"catch")) {
				m.removeUseless();

				std::wstring arg{ m.extractFunc() };
				DTO::myString m2{ &arg };
				DTO::Interface* type{ nullptr };

				std::wstring typeName{ m2.extractName() };
				if (genTypes.containKey(&typeName))
					type = genTypes.getInterface(typeName);
				else if (DTO::GLOBAL::getClasses()->containKey(&typeName, &genTypes))
					type = DTO::GLOBAL::getClasses()->getInterface(typeName);
				else
					throw "??";

				if (type == nullptr)
					throw "??";

				m2.removeUseless();
				std::wstring name{ m2.extractName() };

				m.removeUseless();

				DTO::MemoryVariable var2{ &variables };
				var2.add(name, type);

				DTO::FunctionBlock* t_catch{ parseFunctionBlock(var2, &line, genTypes) };
				catchs.push(std::tuple<std::wstring, DTO::Interface*, DTO::FunctionBlock*>(name, type, t_catch));
				m.removeUseless();
				word = m.extractName();
			}
			m.removeUseless();
			size_t size{ catchs.size() };
			std::wstring* names{ new std::wstring[size] };
			DTO::Instanciable** types{ new DTO::Instanciable * [size] };
			DTO::FunctionBlock** catches{ new DTO::FunctionBlock * [size] };

			size_t i{ 0 };
			while (!catchs.empty() && i < size) {
				std::tie(names[i], types[i], catches[i]) = catchs.front();
				catchs.pop();
				i++;
			}
			if (!catchs.empty() || i < size)
				throw "??";

			DTO::FunctionBlock* finally { nullptr };
			if (word._Equal(L"finally")) {
				m.removeUseless();
				finally = parseFunctionBlock(var, &line, genTypes);
			}

			return new DTO::Try(t_try, types, names, catches, size, finally);
		}
	}
	else if (line.empty()) {
		if (word.at(word.size() - 1) == L'>') {
			std::wstring name{ DTO::myString(&word).extract2() };
			std::queue<std::wstring> args{ DTO::myString(&DTO::myString(&word).extractFunc2()).split2(',') };
			size_t size{ args.size() };
			DTO::Instanciable** argsType{ new DTO::Instanciable * [size] };
			size_t i{ 0 };
			while (!args.empty() && i < size) {
				argsType[i] = nullptr;
				std::wstring argName{ args.front() };
				if (genTypes.containKey(&argName))
					argsType[i] = genTypes.getType(argName);
				else if (DTO::GLOBAL::getClasses()->containKey(&argName, &genTypes))
					argsType[i] = DTO::GLOBAL::getClasses()->getType(argName);
				else
					throw "??";
				if (argsType[i] == nullptr)
					throw "??";
				args.pop();
				i++;
			}
			if (!args.empty() || i < size)
				throw "??";
			DTO::FunctionO* func{ nullptr };
			if (preC->getFuncs()->containsI(name, argsType, size))
				func = new DTO::FunctionO(DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Function), preC->getFuncs()->get(name, argsType, size));
			else
				throw "??";
			delete[] argsType;
			return new DTO::ReturnObj(func);
		}
		else if (pre != nullptr) {
			return new DTO::GetVarFunc(pre, word);
		}
		else if (preC != nullptr) {
			return new DTO::GetStaticVar(preC, word);
		}
		else if (variables.containKey(word)) {
			return new DTO::Return(word, variables.get(word));
		}
		throw "??";
	}
	switch (line.at(0)) {
	case '(': {
		if (pre == nullptr && preC == nullptr)
			throw "??";

		std::queue<std::wstring> args{ DTO::myString(&m.extractFunc2()).split2(',') };

		size_t size{ args.size() };
		if (pre != nullptr)
			size++;
		DTO::Command** commandsI{ new DTO::Command * [size] };
		size_t argI{ 0 };
		if (pre != nullptr) {
			commandsI[0] = pre;
			argI++;
		}

		while (!args.empty() && argI < size) {
			commandsI[argI] = parseReturn(variables, args.front(), genTypes);
			args.pop();
			argI++;
		}

		if (!args.empty())
			throw "?";

		DTO::Command* cmd{ nullptr };
		if (pre != nullptr)
			cmd = new DTO::FunctionDynCom(pre, word, commandsI, size);
		if (preC != nullptr)
			cmd = new DTO::FunctionCom(preC, word, commandsI, size);

		m.removeUseless();

		if (line.empty())
			return cmd;
		else if (line.at(0) == L'.') {
			m.extract(1);
			return parseCommand(nullptr, cmd, variables, line, genTypes);
		}

		throw "??";
	}
	case ' ': {
		m.extract(1);
		if (pre != nullptr) {
			if (line.at(0) == L'=') {
				m.extract(1);
				m.removeUseless();
				return new DTO::ReplaceVarInOb(pre, true, word, parseReturn(variables, line, genTypes));
			}
		}
		if (preC != nullptr) {
			if (line.at(0) == L'=') {
				m.extract(1);
				m.removeUseless();
				return new DTO::ReplaceStatVar(preC, word, parseReturn(variables, line, genTypes));
			}
		}
		else if (variables.containKey(word)) {
			if (line.at(0) == L'=') {
				m.extract(1);
				m.removeUseless();
				return new DTO::ReplaceVar(variables.get(word), word, parseReturn(variables, line, genTypes));
			}
			throw "??";
		}
		else if (genTypes.containKey(&word)) {
			DTO::Instanciable* cl{ genTypes.getType(word) };
			m.removeUseless();
			std::wstring name{ m.extractName() };
			m.removeUseless();
			if (line.at(0) == L'=') {
				m.extract(1);
				m.removeUseless();
				variables.add(name, cl);
				return new DTO::Declaration(cl, name, parseReturn(variables, line, genTypes));
			}
			throw "?";
		}
		else if (DTO::GLOBAL::getClasses()->containKey(&word, &genTypes)) {
			DTO::Instanciable* cl{ DTO::GLOBAL::getClasses()->getType(word) };
			m.removeUseless();
			std::wstring name{ m.extractName() };
			m.removeUseless();
			if (line.at(0) == L'=') {
				m.extract(1);
				m.removeUseless();
				variables.add(name, cl);
				return new DTO::Declaration(cl, name, parseReturn(variables, line, genTypes));
			}
			throw "?";
		}
		throw "?";
	}
	case '.': {
		m.extract(1);
		if (pre != nullptr) {
			return parseCommand(nullptr, new DTO::GetVarFunc(pre, word), variables, line, genTypes);
		}
		else if (preC != nullptr) {
			return parseCommand(nullptr, new DTO::GetStaticVar(preC, word), variables, line, genTypes);
		}
		else if (variables.containKey(word)) {
			return parseCommand(nullptr, new DTO::Return(word, variables.get(word)), variables, line, genTypes);
		}
		else if (genTypes.containKey(&word)) {
			return parseCommand(genTypes.getClass(word), nullptr, variables, line, genTypes);
		}
		else if (DTO::GLOBAL::getClasses()->containKey(&word, &genTypes)) {
			return parseCommand(DTO::GLOBAL::getClasses()->getClass(word), nullptr, variables, line, genTypes);
		}
		throw "??";
	}
	case '<': {
		return parseCommand(DTO::GLOBAL::getClasses()->getClass(word + L'<' + m.extractFunc() + L'>'), nullptr, variables, line, genTypes);
	}
	default:
		throw "??";
	}
	throw "??";
}

DTO::Class* Parser::Parser::parseClass(std::wstring path, std::wstring str, DTO::MemorySourceFile& genTypes) {
	DTO::myString s{ &str };

	s.removeUseless();

	std::wstring name{ s.extractName() };
	s.removeUseless();

	DTO::Class* parent{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Object) };

	if (s.startWith(L"extends")) {
		s.extract(7);
		s.removeUseless();
		std::wstring tname{ s.extractName() };
		if (!DTO::GLOBAL::getClasses()->containKey(&tname, &genTypes))
			throw "??";
		parent = DTO::GLOBAL::getClasses()->getClass(tname);
		if (parent == nullptr)
			throw "??";
		s.removeUseless();
	}

	DTO::Interface** implements;
	size_t implementsLen;
	{
		std::queue<DTO::Interface*> interfaces{};
		if (s.startWith(L"implements")) {
			s.extract(10);
			s.removeUseless();
			do {
				std::wstring interfaceName{ s.extractName() };
				if (!DTO::GLOBAL::getClasses()->containKey(&interfaceName, &genTypes))
					throw "??";
				interfaces.push(DTO::GLOBAL::getClasses()->getInterface(interfaceName));
				s.removeUseless();
			} while (str.at(0) != L'{');
		}

		implementsLen = interfaces.size();
		implements = new DTO::Interface * [implementsLen];
		size_t c{ 0 };
		while (!interfaces.empty() && c < implementsLen) {
			implements[c] = interfaces.front();
			interfaces.pop();
			c++;
		}
		if (!interfaces.empty())
			throw "??";
	}

	str = s.extractFunc();
	DTO::myString s2{ &str };
	s2.removeUseless();

	DTO::Class* _class{ new DTO::Class(name,path,parent,implements,implementsLen,&genTypes) };
	if (name.at(name.size() - 1) != L'>')
		DTO::GLOBAL::getClasses()->set(name, _class);

	while (!str.empty()) {
		s2.removeUseless();
		bool _static{ false };

		std::wstring word{ s.extractName() };
		s2.removeUseless();
		if (word == L"static") {
			_static = true;
			word = s.extractName();
			s2.removeUseless();
		}

		DTO::Instanciable* type{ nullptr };


		if (word == L"function") {
			std::wstring signature{};
			DTO::Function* func{ parseFunction(!_static,_class,&signature, &str,genTypes) };
			_class->getFuncs()->add(signature, func);
			continue;
		}

		if (genTypes.containKey(&word))
			type = genTypes.getType(word);
		else if (DTO::GLOBAL::getClasses()->containKey(&word, &genTypes))
			type = DTO::GLOBAL::getClasses()->getType(word);
		else
			throw "??";
		if (type == nullptr)
			throw "no type in var";

		std::wstring name{ s.extractName() };

		s.removeUseless();

		if (_static) {
			DTO::StatVar* _default{ new DTO::StatVar{type,nullptr} };
			if (str.at(0) == L'=') {
				s.extract(1);
				s.removeUseless();
				DTO::MemoryVariable variables{ };
				DTO::MemoryObject a{ };
				DTO::Command* r{ parseReturn(variables, str,genTypes) };
				DTO::CommandReturn* re{ r->exec(a) };

				_default->SetValue(re->getObject());

				delete re;
				delete r;
				if (!_default->GetValue()->getClass()->instanceOf(type))
					throw "CastException";
			}
			_class->getStatVars()->add(name, _default);
		}
		else {
			DTO::Command* _default{ nullptr };
			if (str.at(0) == L'=') {
				s.extract(1);
				s.removeUseless();
				DTO::MemoryVariable variables{ };
				_default = parseReturn(variables, str, genTypes);
			}
			_class->getVars()->add(name, new DTO::Var(_class->getVars()->size(), type, _default));
		}
		s.extract(1);
	}

	for (size_t c(0); c < _class->getInterfaceLen(); c++)
		if (!_class->getInterfaces()[c]->containsAll(_class))
			throw "??";
	return _class;
}

DTO::Command* Parser::Parser::parseReturn(DTO::MemoryVariable& variables, std::wstring& s, DTO::MemorySourceFile& genTypes) {
	DTO::myString ms{ &s };
	ms.removeUseless();

	if (s._Equal(L"NULL")) {
		return new DTO::ReturnObj(new DTO::NullObject());
	}

	DTO::Command* pre{ nullptr };

	if (s.substr(0, 3)._Equal(L"new")) {
		ms.extract(3);
		ms.removeUseless();
		std::wstring className{ ms.extractName() };
		if (!DTO::GLOBAL::getClasses()->containKey(&className, &genTypes))
			throw "??";
		DTO::Class* sc{ DTO::GLOBAL::getClasses()->getClass(className) };//TODO new on interface when declaring the missing function
		if (sc == nullptr)
			throw "??";

		std::queue<std::wstring> args{ DTO::myString(&ms.extractFunc2()).split2(',') };
		size_t size{ args.size() };
		DTO::Command** commandsI{ new DTO::Command * [size] };

		size_t argI{ 0 };

		while (!args.empty() && argI < size) {
			commandsI[argI] = parseReturn(variables, args.front(), genTypes);
			args.pop();
			argI++;
		}

		if (!args.empty() || argI > size)
			throw "?";

		pre = new DTO::InstanceFunc(sc, commandsI, size + 1);
	}

	else if (s.at(0) == L'"') {
		ms.extract(1);
		std::wstring v{ L"" };
		wchar_t temp;
		while ((temp = ms.extract(1).at(0)) != L'\"') {
			if (temp == L'\\') {
				v += DTO::myString::backSlachChar(ms.extract(1).at(0));
			}
			else {
				v += temp;
			}
		}
		DTO::Class* string{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::String) };
		DTO::Instanciable** in{ new DTO::Instanciable * [2]{string,string} };
		DTO::Function* f{ string->getFuncs()->get(L"String",in,2) };
		delete[] in;
		pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::StringO(string,v) }, 2);
	}

	else if (s.at(0) == L'\'') {
		ms.extract(1);
		wchar_t temp{ ms.extract(1).at(0) };
		if (temp == L'\\') {
			temp = DTO::myString::backSlachChar(ms.extract(1).at(0));
		}
		if (ms.extract(1).at(0) != L'\'')
			throw "?";

		DTO::Class* _char{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Char) };
		DTO::Instanciable** in{ new DTO::Instanciable * [2]{_char,_char} };
		DTO::Function* f{ _char->getFuncs()->get(L"Char",in,2) };
		delete[] in;
		pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::CharO(_char,temp) }, 2);
	}
	else {
		std::wstring temp{ ms.extractName() };
		std::string number{ temp.begin(),temp.end() };
		if (DTO::myString::is_number(temp)) {
			wchar_t a{ temp.back() };
			temp.pop_back();
			switch (a) {
			case 'b': {
				DTO::Class* Byte{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Byte) };
				DTO::Instanciable** in{ new DTO::Instanciable * [2]{Byte,Byte} };
				DTO::Function* f{ Byte->getFuncs()->get(L"Byte",in,2) };
				delete[] in;
				pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::ByteO(Byte,(int8_t)std::strtoll(number.c_str(),NULL,10)) }, 2);
				break;
			}case 's': {
				DTO::Class* Short{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Short) };
				DTO::Instanciable** in{ new DTO::Instanciable * [2]{Short,Short} };
				DTO::Function* f{ Short->getFuncs()->get(L"Short",in,2) };
				delete[] in;
				pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::ShortO(Short,(int16_t)std::strtoll(number.c_str(),NULL,10)) }, 2);
				break;
			}case 'i': {
				DTO::Class* Integer{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Integer) };
				DTO::Instanciable** in{ new DTO::Instanciable * [2]{Integer,Integer} };
				DTO::Function* f{ Integer->getFuncs()->get(L"Integer",in,2) };
				delete[] in;
				pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::IntegerO(Integer,(int32_t)std::strtoll(number.c_str(),NULL,10)) }, 2);
				break;
			}case 'f': {
				DTO::Class* Float{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Float) };
				DTO::Instanciable** in{ new DTO::Instanciable * [2]{Float,Float} };
				DTO::Function* f{ Float->getFuncs()->get(L"Float",in,2) };
				delete[] in;
				pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::FloatO(Float,(float_t)std::strtod(number.c_str(),NULL)) }, 2);
				break;
			}case 'l': {
				DTO::Class* Long{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Long) };
				DTO::Instanciable** in{ new DTO::Instanciable * [2]{Long,Long} };
				DTO::Function* f{ Long->getFuncs()->get(L"Long",in,2) };
				delete[] in;
				pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr,new DTO::LongO(Long,(int64_t)std::strtoll(number.c_str(),NULL,10)) }, 2);
				break;
			}case 'd': {
				DTO::Class* Double{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Double) };
				DTO::Instanciable** in{ new DTO::Instanciable * [2]{Double,Double} };
				DTO::Function* f{ Double->getFuncs()->get(L"Double",in,2) };
				delete[] in;
				pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::DoubleO(Double,(double_t)std::strtod(number.c_str(),NULL)) }, 2);
				break;
			}default: {
				throw "??";
			}
			}
		}
		else if (temp._Equal(L"true")) {
			DTO::Class* Bool{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Boolean) };
			DTO::Instanciable** in{ new DTO::Instanciable * [2]{Bool,Bool} };
			DTO::Function* f{ Bool->getFuncs()->get(L"Boolean",in,2) };
			delete[] in;
			pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::BooleanO(Bool, true) }, 2);
		}
		else if (temp._Equal(L"false")) {
			DTO::Class* Bool{ DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Boolean) };
			DTO::Instanciable** in{ new DTO::Instanciable * [2]{Bool,Bool} };
			DTO::Function* f{ Bool->getFuncs()->get(L"Boolean",in,2) };
			delete[] in;
			pre = new DTO::ObjectCreator(f, new DTO::IObject * [2]{ nullptr, new DTO::BooleanO(Bool, false) }, 2);
		}
		else
			s = temp + s;
	}
	std::wstring arg1{ L"" };

	if (pre == nullptr) {
		arg1 = ms.extract2();

		if (variables.containKey(arg1)) {
			pre = new DTO::Return(arg1, variables.get(arg1));
		}
		else if (genTypes.containKey(&arg1)) {
			ms.extract(1);
			DTO::SourceFile* c{ genTypes.get(arg1) };
			if (dynamic_cast<DTO::Class*>(c))
				pre = parseCommand((DTO::Class*)c, nullptr, variables, s, genTypes);
			else {
				if (!s.empty())
					throw "??";
				return new DTO::ReturnObj(new DTO::ClassO(DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Class), c));
			}
		}
		else if (DTO::GLOBAL::getClasses()->containKey(&arg1, &genTypes)) {
			ms.extract(1);
			DTO::SourceFile* c{ DTO::GLOBAL::getClasses()->get(arg1) };
			if (dynamic_cast<DTO::Class*>(c))
				pre = parseCommand((DTO::Class*)c, nullptr, variables, s, genTypes);
			else {
				if (!s.empty())
					throw "??";
				return new DTO::ReturnObj(new DTO::ClassO(DTO::GLOBAL::getClasses()->getClass(DTO::Paths::Class), c));
			}
		}
		else {
			throw "??";
		}
	}

	while (!s.empty() && s.at(0) == L'.') {
		DTO::Class* args1C{ nullptr };
		ms.extract(1);
		std::wstring arg2{ ms.extractName() };
		if (s.empty()) {
			if (pre == nullptr)
				throw "??";
			return new DTO::GetVarFunc(pre, arg2);
		}
		else if (s.at(0) == L'.') {
			if (pre == nullptr) {
				pre = new DTO::Return(arg2, variables.get(arg2));
			}
			else {
				pre = new DTO::GetVarFunc(pre, arg2);
			}
		}
		else if (s.at(0) == L'(') {

			std::queue<std::wstring> args{ DTO::myString(&ms.extractFunc2()).split2(',') };
			size_t size{ args.size() + 1 };
			DTO::Command** commandsI{ new DTO::Command * [size] };
			if (pre == nullptr)
				pre = new DTO::Return(arg1, variables.get(arg1));
			size_t argI{ 1 };

			commandsI[0] = pre;

			while (!args.empty() && argI < size) {
				commandsI[argI] = parseReturn(variables, args.front(), genTypes);
				args.pop();
				argI++;
			}

			if (!args.empty() || argI > size)
				throw "??";

			pre = new DTO::ObFunc(arg2, commandsI, size);
		}
	}

	if (pre == nullptr)
		return new DTO::Return(arg1, variables.get(arg1));
	else
		return pre;
}

DTO::Function* Parser::Parser::parseFunction(bool isNotStatic, DTO::Class* methodeOf, std::wstring* name, std::wstring* str, DTO::MemorySourceFile& genTypes) {
	DTO::MemoryVariable variables{ };
	DTO::myString s{ str };

	DTO::Instanciable* returnType{ nullptr };
	std::wstring returnTypeName{ s.extractName() };
	std::wstring methodeName{ std::wstring(methodeOf->getName()) };
	if (returnTypeName == DTO::myString{ &methodeName }.extract2()) {
		isNotStatic = true;
		*name = returnTypeName;
		returnType = methodeOf;
		s.removeUseless();
	}
	else {
		if (returnTypeName != L"void") {
			if (genTypes.containKey(&returnTypeName))
				returnType = genTypes.getType(returnTypeName);
			else if (DTO::GLOBAL::getClasses()->containKey(&returnTypeName, &genTypes))
				returnType = DTO::GLOBAL::getClasses()->getType(returnTypeName);
			else
				throw "??";
			if (returnType == nullptr)
				throw "??";
		}
		s.removeUseless();
		*name = s.extractName();
		s.removeUseless();
	}


	std::queue<std::wstring> argsQueue{ DTO::myString(&s.extractFunc2()).split2(',') };

	size_t argsLen{ argsQueue.size() };
	if (isNotStatic) {
		argsLen++;
	}
	DTO::Arg* args{ new DTO::Arg[argsLen] };
	size_t argI{ 0 };
	if (isNotStatic) {
		argI++;
		args[0].type = methodeOf;
		args[0].name = std::wstring{ L"this" };
		variables.add(args[0].name, args[0].type);
	}

	while (!argsQueue.empty() && argI < argsLen) {
		std::wstring current = argsQueue.front();
		DTO::myString m{ &current };
		m.removeUseless();
		std::wstring typeName{ m.extractName() };
		if (genTypes.containKey(&typeName))
			args[argI].type = genTypes.getType(typeName);
		else if (DTO::GLOBAL::getClasses()->containKey(&typeName, &genTypes))
			args[argI].type = DTO::GLOBAL::getClasses()->getType(typeName);
		else
			throw "??";
		m.removeUseless();
		args[argI].name = current;

		variables.add(args[argI].name, args[argI].type);

		argsQueue.pop();
		argI++;
	}

	if (!argsQueue.empty() || argI < argsLen)
		throw "?";

	s.removeUseless();

	DTO::Signature* sig{ new DTO::Signature(methodeOf->getPath(),returnType, args, argsLen) };
	return new DTO::Function(sig, new FunctionDef(*name, methodeOf, sig, s.extractFunc2(), &genTypes));
}

DTO::FunctionBlock* Parser::Parser::parseFunctionBlock(DTO::MemoryVariable& variables, std::wstring* str, DTO::MemorySourceFile& genTypes) {
	DTO::myString s{ str };
	std::queue<DTO::Command*> commands{};
	std::queue<std::wstring> func{ DTO::myString(&s.extractFunc2()).split2(';') };

	if (!func.empty()) {
		std::wstring a{ func.front() };
		DTO::myString m{ &a };
		func.pop();

		while (!func.empty()) {

			m.removeUseless();
			if (m.startWith(L"try")) {
				while (true) {
					std::wstring n{ func.front() };
					DTO::myString m2{ &n };
					m2.removeUseless();
					if (m2.startWith(L"catch")) {
						a += n;
						func.pop();
						continue;
					}
					if (m2.startWith(L"finally")) {
						a += n;
						func.pop();
					}
					break;
				}
			}

			commands.push(parseCommand(nullptr, nullptr, variables, a, genTypes));

			a = func.front();
			func.pop();
		}
	}

	size_t len{ commands.size() };
	DTO::Command** cmds{ new DTO::Command * [len] };
	size_t i{ 0 };
	while (!commands.empty() && i < len) {
		cmds[i] = commands.front();
		commands.pop();
		i++;
	}
	if (i < len - 1)
		throw "?";

	s.removeUseless();
	return new DTO::FunctionBlock(cmds, len);
}

DTO::SourceFile* Parser::Parser::loadFile(std::wstring filePath)
{
	std::ifstream file(filePath);
	std::wstring tempPath{ filePath };
	std::queue<std::wstring> pathqueue{ DTO::myString{ &tempPath }.split(L"\\") };
	std::wstring path{ pathqueue.front() };
	pathqueue.pop();
	while (pathqueue.size() > 1) {
		path += std::filesystem::path::preferred_separator + pathqueue.front();
		pathqueue.pop();
	}
	if (file.is_open())
	{
		{
			std::wstring p{ filePath };
			p = DTO::myString{ &p }.split(L"\\").back();
			p = DTO::myString{ &p }.extract(L".");
			if (DTO::GLOBAL::getClasses()->containKey(&p)) {
				file.close();
				return DTO::GLOBAL::getClasses()->get(p);
			}
			else {
				DTO::GLOBAL::getClasses()->add(p, nullptr);
			}
		}
		std::string line;
		std::string content8{ "" };
		while (std::getline(file, line)) {
			content8 += line;
		}
		file.close();

		std::wstring  content{ std::wstring(content8.begin(),content8.end()) };

		DTO::myString s{ &content };
		std::wstring instru{ L"" };
		do {
			s.removeUseless();
			instru = s.extract(L" ");
			if (instru == L"include") {
				s.removeUseless();
				s.extract(1);
				std::wstring v{ path + L'\\' };
				wchar_t temp;
				while ((temp = s.extract(1).at(0)) != L'\"') {
					if (temp == L'\\') {
						v += DTO::myString::backSlachChar(s.extract(1).at(0));
					}
					else {
						v += temp;
					}
				}
				loadFile(v);
			}
		} while (instru != L"class" && instru != L"interface");

		s.removeUseless();
		std::wstring name{ s.extract2() };
		s.removeUseless();
		if (content.at(0) == L'<') {
			std::queue<std::wstring> q{ DTO::myString(&s.extractFunc2()).split2(',') };
			GenericDef* g{ new GenericDef(name,path, instru, q, content) };
			DTO::GLOBAL::getClasses()->set(name, g);
			return g;
		}
		else {
			DTO::MemorySourceFile* g{ new DTO::MemorySourceFile {false} };
			if (instru == L"class") {
				return parseClass(path, name + L' ' + content, *g);
			}
			else {
				return parseInterface(path, name + L' ' + content, *g);
			}
		}
	}
	else throw "Unable to open file";
}

DTO::Interface* Parser::Parser::parseInterface(std::wstring path, std::wstring str, DTO::MemorySourceFile& genTypes)
{
	DTO::myString s{ &str };

	s.removeUseless();

	std::wstring name{ s.extractName() };
	s.removeUseless();

	DTO::Interface** implements;
	size_t implementsLen;
	{
		std::queue<DTO::Interface*> interfaces{};
		if (s.startWith(L"extends")) {
			s.extract(7);
			s.removeUseless();
			do {
				std::wstring interfaceName{ s.extractName() };
				if (!DTO::GLOBAL::getClasses()->containKey(&interfaceName, &genTypes))
					throw "??";
				interfaces.push(DTO::GLOBAL::getClasses()->getInterface(interfaceName));
				s.removeUseless();
			} while (str.at(0) != L'{');
		}

		implementsLen = interfaces.size();
		implements = new DTO::Interface * [implementsLen];
		size_t c{ 0 };
		while (!interfaces.empty() && c < implementsLen) {
			implements[c] = interfaces.front();
			interfaces.pop();
			c++;
		}
		if (c > implementsLen)
			throw "??";
	}

	str = s.extractFunc();
	s = DTO::myString{ &str };
	s.removeUseless();

	DTO::Interface* _interface{ new DTO::Interface(name,path,&genTypes,implements,implementsLen) };
	if (name.at(name.size() - 1) != L'>')
		DTO::GLOBAL::getClasses()->set(name, _interface);

	while (!str.empty()) {
		s.removeUseless();
		std::wstring typeName{ s.extractName() };
		DTO::Instanciable* type{ nullptr };
		if (typeName != L"void") {
			if (genTypes.containKey(&typeName))
				type = genTypes.getType(typeName);
			else if (DTO::GLOBAL::getClasses()->containKey(&typeName, &genTypes))
				type = DTO::GLOBAL::getClasses()->getType(typeName);
			else
				throw "??";
			if (type == nullptr)
				throw "??";
		}
		s.removeUseless();
		std::wstring name{ s.extractName() };
		s.removeUseless();

		std::queue<std::wstring> argsQueue{ DTO::myString(&s.extractFunc2()).split2(',') };

		size_t argsLen{ argsQueue.size() + 1 };
		DTO::Arg* args{ new DTO::Arg[argsLen] };
		size_t argI{ 1 };

		args[0].type = _interface;
		args[0].name = std::wstring{ L"this" };

		while (!argsQueue.empty() && argI < argsLen) {
			std::wstring current = argsQueue.front();
			DTO::myString m{ &current };

			std::wstring argTypeName{ m.extractName() };
			if (genTypes.containKey(&argTypeName))
				args[argI].type = genTypes.getType(argTypeName);
			else if (genTypes.containKey(&argTypeName))
				args[argI].type = DTO::GLOBAL::getClasses()->getType(argTypeName);
			else
				throw "??";

			m.removeUseless();
			args[argI].name = current;

			argsQueue.pop();
			argI++;
		}

		if (!argsQueue.empty() || argI < argsLen)
			throw "?";

		_interface->add(name, new DTO::Signature(path, type, args, argsLen));
		s.extract(1);
	}


	return _interface;
}
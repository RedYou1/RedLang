#include "FunctionDef.h"
#include "Parser.h"
#include "../DTO/myString.h"
#include "../DTO/Return.h"
#include "../DTO/MemoryFunction.h"

Parser::FunctionDef::FunctionDef(std::string name, DTO::Class* functionOf, DTO::Signature* signature, std::string definition, DTO::MemorySourceFile* genTypes)
	:DTO::PreFunction(signature), m_name(name), m_functionOf(functionOf), m_definition(definition), m_genTypes(genTypes)
{
}

DTO::PostFunction* Parser::FunctionDef::convert(DTO::Function* _f)
{
	DTO::MemoryVariable variables{};
	for (size_t i{ 0 }; i < m_signature->getArgsLen(); i++)
		variables.add(m_signature->getArgs()[i].name, m_signature->getArgs()[i].type);

	DTO::myString s{ &m_definition };
	std::queue<DTO::Command*> commands{};

	s.removeUseless();

	bool constructor{ false };
	{
		std::string sss{ m_functionOf->getName() };
		constructor = DTO::myString{ &sss }.startWith(m_name);
	}


	if (constructor) {
		//only classes
		if (m_definition.at(0) == ':') {
			s.extract(1);
			s.removeUseless();
			DTO::MemoryVariable var{};
			commands.push(Parser::parseCommand(m_functionOf->extends(), nullptr, var, m_definition, *m_genTypes));
			s.removeUseless();
		}
		else {
			DTO::Class* parent{ m_functionOf->extends() };
			if (!parent->getName()._Equal("Object")) {
				DTO::Instanciable** cc{ new DTO::Instanciable * [1]{parent} };
				if (DTO::Function * func{ parent->getFuncs()->get(parent->getName(), cc, 1) }) {
					DTO::Command** cc2{ new DTO::Command * [1]{new DTO::Return("this", m_functionOf)} };
					commands.push(new DTO::FunctionKnownCom(func, cc2, 1));
				}
				else {
					throw "no default parent constructor";
				}
				delete[] cc;
			}
		}
	}

	std::queue<std::string> func{ DTO::myString(&s.extractFunc2()).split2(';') };

	if (!func.empty()) {
		std::string a{ func.front() };
		DTO::myString m{ &a };
		func.pop();

		while (!func.empty()) {
			m.removeUseless();
			if (m.startWith("try")) {
				while (true) {
					std::string n{ func.front() };
					DTO::myString m2{ &n };
					m2.removeUseless();
					if (m2.startWith("catch")) {
						a += n;
						func.pop();
						continue;
					}
					break;
				}
			}
			commands.push(Parser::parseCommand(nullptr, nullptr, variables, a, *m_genTypes));

			a = func.front();
			func.pop();
		}
	}

	if (constructor) {
		commands.push(new DTO::Return("this", m_signature->getReturnType()));
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

	return new DTO::PostFunction(m_signature, cmds, len);
}
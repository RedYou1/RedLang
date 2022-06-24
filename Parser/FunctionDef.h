#pragma once
#include <string>
#include "../DTO/Class.h"
#include "../DTO/Function.h"
#include "../DTO/MemoryObject.h"
#include "../DTO/MemorySourceFile.h"
#include "../DTO/Command.h"

namespace Parser {
	class FunctionDef :public DTO::PreFunction {
	private:
		std::string m_name;
		DTO::Class* m_functionOf;
		std::string m_definition;
		DTO::MemorySourceFile* m_genTypes;
	public:
		FunctionDef(std::string name, DTO::Class* functionOf, DTO::Signature* signature, std::string definition, DTO::MemorySourceFile* genTypes);

		std::string getDefinition() { return m_definition; }

		DTO::PostFunction* convert(DTO::Function* func) override;
	};
}
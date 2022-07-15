#pragma once
#include <string>
#include "../DTO/Class.h"
#include "../DTO/Function.h"
#include "../DTO/MemoryObject.h"
#include "../DTO/MemorySourceFile.h"
#include "../DTO/Command.h"
#include "../DTO/PreFunction.h"

namespace Parser {
	class FunctionDef :public DTO::PreFunction {
	private:
		std::wstring m_name;
		DTO::Class* m_functionOf;
		std::wstring m_definition;
		DTO::MemorySourceFile* m_genTypes;
	public:
		FunctionDef(std::wstring name, DTO::Class* functionOf, DTO::Signature* signature, std::wstring definition, DTO::MemorySourceFile* genTypes);

		std::wstring getDefinition() { return m_definition; }

		DTO::PostFunction* convert(DTO::Function* func) override;
	};
}
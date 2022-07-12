#pragma once
#include "../DTO/Class.h"
#include "../DTO/Command.h"
#include "../DTO/Function.h"
#include "../DTO/MemoryVariable.h"
#include "../DTO/MemorySourceFile.h"

namespace Parser {
	class Parser {
	public:
		static DTO::Command* parseCommand(DTO::Class* preC, DTO::Command* pre, DTO::MemoryVariable& variables, std::wstring line, DTO::MemorySourceFile& genTypes);
		static DTO::Class* parseClass(std::wstring path, std::wstring str, DTO::MemorySourceFile& genTypes);
		static DTO::Command* parseReturn(DTO::MemoryVariable& variables, std::wstring& s, DTO::MemorySourceFile& genTypes);
		static DTO::Function* parseFunction(bool isNotStatic, DTO::Class* methodeOf, std::wstring* name, std::wstring* str, DTO::MemorySourceFile& genTypes);
		static DTO::FunctionBlock* parseFunctionBlock(DTO::MemoryVariable& variables, std::wstring* str, DTO::MemorySourceFile& genTypes);
		static DTO::SourceFile* loadFile(std::wstring filePath);
		static DTO::Interface* parseInterface(std::wstring path, std::wstring str, DTO::MemorySourceFile& genTypes);
	};
}
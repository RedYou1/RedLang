#pragma once
#include <filesystem>
#include "../DTO/Class.h"
#include "../DTO/Command.h"
#include "../DTO/Function.h"
#include "../DTO/MemoryVariable.h"
#include "../DTO/MemorySourceFile.h"
#include "../DTO/FunctionBlock.h"

namespace Parser {
	class Parser {
	public:
		static DTO::Command* parseCommand(DTO::Class* preC, DTO::Command* pre, DTO::MemoryVariable& variables, std::filesystem::path path, std::wstring line, DTO::MemorySourceFile& genTypes);
		static DTO::Class* parseClass(std::filesystem::path path, std::wstring str, DTO::MemorySourceFile& genTypes);
		static DTO::Command* parseReturn(DTO::MemoryVariable& variables, std::filesystem::path path, std::wstring& s, DTO::MemorySourceFile& genTypes);
		static DTO::Function* parseFunction(bool isNotStatic, DTO::Class* methodeOf, std::wstring* name, std::wstring* str, DTO::MemorySourceFile& genTypes);
		static DTO::FunctionBlock* parseFunctionBlock(DTO::MemoryVariable& variables, std::filesystem::path path, std::wstring* str, DTO::MemorySourceFile& genTypes);
		static DTO::SourceFile* loadFile(std::filesystem::path filePath);
		static DTO::Interface* parseInterface(std::filesystem::path path, std::wstring str, DTO::MemorySourceFile& genTypes);
	};
}
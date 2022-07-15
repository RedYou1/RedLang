#pragma once
#include <string>
#include "Interface.h"
#include "Arg.h"

namespace DTO {
	class Command;
	class MemoryVar;
	class MemoryFunction;
	class MemoryStatVar;

	class Class :public Interface {
	protected:
		Class* m_parent;

		MemoryFunction* m_funcs;

		MemoryVar* m_vars;
		MemoryStatVar* m_statVars;
	public:
		Class(std::wstring name, std::filesystem::path path, Class* parent);
		Class(std::wstring name, std::filesystem::path path, Class* parent, Interface** implements, size_t implementsLen);
		Class(std::wstring name, std::filesystem::path path, Class* parent, MemorySourceFile* genTypes);
		Class(std::wstring name, std::filesystem::path path, Class* parent, Interface** implements, size_t implementsLen, MemorySourceFile* genTypes);
		virtual ~Class() override;

		Class* extends();

		bool instanceOf(Instanciable* other) override;

		MemoryFunction* getFuncs();
		MemoryVar* getVars();
		MemoryStatVar* getStatVars();

		void addFunc(std::wstring name, Instanciable* returnType, Arg* args, size_t argsLen, Command* cmd, bool infinity = false);
		void addFunc(std::wstring name, Instanciable* returnType, Arg* args, size_t argsLen, Command** cmds, size_t cmdsLen, bool infinity = false);
	};
}
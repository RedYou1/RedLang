#pragma once
#include <string>
#include "Interface.h"

namespace DTO {
	class Command;
	class MemoryVar;
	class MemoryFunction;
	class MemoryObject;
	class MemoryStatVar;

	class Class :public Interface {
	protected:
		Class* m_parent;

		MemoryFunction* m_funcs;

		MemoryVar* m_vars;
		MemoryStatVar* m_statVars;
	public:
		Class(std::string name, std::string path, Class* parent);
		Class(std::string name, std::string path, Class* parent, Interface** implements, size_t implementsLen);
		Class(std::string name, std::string path, Class* parent, MemorySourceFile* genTypes);
		Class(std::string name, std::string path, Class* parent, Interface** implements, size_t implementsLen, MemorySourceFile* genTypes);
		virtual ~Class() override;

		Class* extends();

		bool instanceOf(Interface* other) override;

		MemoryFunction* getFuncs() { return m_funcs; }
		MemoryVar* getVars() { return m_vars; }
		MemoryStatVar* getStatVars() { return m_statVars; }
	};
}
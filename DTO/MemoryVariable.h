#pragma once
#include <string>
#include <map>
#include "Type.h"

namespace DTO {
	class MemoryVariable {
	private:
		MemoryVariable* m_parent;
		std::map<std::wstring, Type> m_vars;
	public:
		MemoryVariable() :m_parent(nullptr), m_vars(std::map<std::wstring, Type>()) {}
		MemoryVariable(MemoryVariable* dub);

		void add(std::wstring name, Type o);

		bool containKey(std::wstring name);

		Type get(std::wstring name);

		size_t size();
	};
}
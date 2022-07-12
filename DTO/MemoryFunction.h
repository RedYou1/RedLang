#pragma once
#include <map>
#include <string>
#include <list>
#include "Function.h"
#include "Interface.h"

namespace DTO {
	class MemoryFunction {
	private:
		MemoryFunction* m_parent;
		std::map<std::wstring, std::list<Function*>> m_vars;
	public:
		MemoryFunction(MemoryFunction* parent) : m_parent{ parent }, m_vars(std::map<std::wstring, std::list<Function*>>()) {}
		~MemoryFunction();

		void add(std::wstring name, Function* o);

		std::list<Function*> get(std::wstring name);
		Function* get(std::wstring name, Instanciable** argsType, size_t argsLen);

		bool containsI(std::wstring name, Instanciable** argsType, size_t argsLen);
		bool containsI(std::wstring name, Arg* args, size_t argsLen);

		template <class K>
		void forEach(K* global, void(*func)(K* global, std::wstring name, std::list<Function*> ob)) {
			for (typename std::map<std::wstring, std::list<Function*>>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
				func(global, it->first, it->second);
			}
		}
	};
}
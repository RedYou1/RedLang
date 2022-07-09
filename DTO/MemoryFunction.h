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
		std::map<std::string, std::list<Function*>> m_vars;
	public:
		MemoryFunction(MemoryFunction* parent) : m_parent{ parent }, m_vars(std::map<std::string, std::list<Function*>>()) {}
		~MemoryFunction();

		void add(std::string name, Function* o);

		std::list<Function*> get(std::string name);
		Function* get(std::string name, Interface** argsType, size_t argsLen);

		bool containsI(std::string name, Interface** argsType, size_t argsLen);
		bool containsI(std::string name, Arg* args, size_t argsLen);

		template <class K>
		void forEach(K* global, void(*func)(K* global, std::string name, std::list<Function*> ob)) {
			for (typename std::map<std::string, std::list<Function*>>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
				func(global, it->first, it->second);
			}
		}
	};
}
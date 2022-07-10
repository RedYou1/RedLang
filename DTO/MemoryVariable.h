#pragma once
#include <string>
#include <map>
#include "Interface.h"

namespace DTO {
	class MemoryVariable {
	private:
		MemoryVariable* m_parent;
		std::map<std::string, Instanciable*> m_vars;
	public:
		MemoryVariable() :m_parent(nullptr), m_vars(std::map<std::string, Instanciable*>()) {}
		MemoryVariable(MemoryVariable* dub);

		void add(std::string name, Instanciable* o);
		void set(std::string name, Instanciable* o);

		bool containKey(std::string name);

		Instanciable* get(std::string name);

		size_t size();

		template <class K>
		void forEach(K* global, void(*func)(K* global, std::string name, Instanciable* ob)) {
			for (typename std::map<std::string, Instanciable*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
				func(global, it->first, it->second);
			}
		}
	};
}
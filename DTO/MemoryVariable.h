#pragma once
#include <string>
#include <map>
#include "Interface.h"

namespace DTO {
	class MemoryVariable {
	private:
		MemoryVariable* m_parent;
		std::map<std::wstring, Instanciable*> m_vars;
	public:
		MemoryVariable() :m_parent(nullptr), m_vars(std::map<std::wstring, Instanciable*>()) {}
		MemoryVariable(MemoryVariable* dub);

		void add(std::wstring name, Instanciable* o);
		void set(std::wstring name, Instanciable* o);

		bool containKey(std::wstring name);

		Instanciable* get(std::wstring name);

		size_t size();

		template <class K>
		void forEach(K* global, void(*func)(K* global, std::wstring name, Instanciable* ob)) {
			for (typename std::map<std::wstring, Instanciable*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
				func(global, it->first, it->second);
			}
		}
	};
}
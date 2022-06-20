#pragma once
#include <string>
#include <map>
#include "Interface.h"

class MemoryVariable {
private:
	MemoryVariable* m_parent;
	std::map<std::string, Interface*> m_vars;
public:
	MemoryVariable() :m_parent(nullptr), m_vars(std::map<std::string, Interface*>()) {}
	MemoryVariable(MemoryVariable* dub);

	void add(std::string name, Interface* o);
	void set(std::string name, Interface* o);

	bool containKey(std::string name);

	Interface* get(std::string name);

	size_t size();

	template <class K>
	void forEach(K* global, void(*func)(K* global, std::string name, Interface* ob)) {
		for (typename std::map<std::string, Interface*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
			func(global, it->first, it->second);
		}
	}
};
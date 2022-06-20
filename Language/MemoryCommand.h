#pragma once
#include <string>
#include <map>
#include "Command.h"

class MemoryObject {
private:
	MemoryObject* m_parent;
	std::map<std::string, CommandClass*> m_vars;
public:
	MemoryObject() : m_parent{ nullptr }, m_vars(std::map<std::string, CommandClass*>()) {}
	MemoryObject(MemoryObject* parent);
	~MemoryObject();

	void add(std::string name, CommandClass* o);
	void set(std::string name, CommandClass* o);

	bool containKey(std::string name);

	CommandClass* get(std::string name);

	size_t size();

	template <class K>
	void forEach(K* global, void(*func)(K* global, std::string name, CommandClass* ob)) {
		for (typename std::map<std::string, CommandClass*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
			func(global, it->first, it->second);
		}
	}
};
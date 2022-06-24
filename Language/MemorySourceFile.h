#pragma once
#include <string>
#include <map>
#include "SourceFile.h"
#include "Interface.h"
#include "Class.h"

class MemorySourceFile {
private:
	std::map<std::string, SourceFile*> m_vars;
	bool m_delete;
public:
	MemorySourceFile(bool _delete) : m_delete(_delete), m_vars(std::map<std::string, SourceFile*>()) {}
	~MemorySourceFile();

	void add(std::string name, SourceFile* o);
	void set(std::string name, SourceFile* o);

	bool containKey(std::string* name, MemorySourceFile* _genTypes = nullptr);

	SourceFile* get(std::string name);
	Interface* getType(std::string name);
	Interface* getInterface(std::string name);
	Class* getClass(std::string name);

	size_t size();

	template <class K>
	void forEach(K* global, void(*func)(K* global, std::string name, SourceFile* ob)) {
		for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
			func(global, it->first, it->second);
		}
	}
};
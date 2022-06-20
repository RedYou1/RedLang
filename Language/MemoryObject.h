#pragma once
#include <string>
#include <map>
#include "Object.h"
#include "Interface.h"

class MemoryObject {
private:

	class Memory {
	private:
		Object* m_object;
		Interface* m_type;
	public:
		Memory(Object* object, Interface* type);
		~Memory();

		Object* getObject();
		Interface* getType();

		void setObject(Object* object);
	};

	MemoryObject* m_parent;
	std::map<std::string, Memory*> m_vars;
public:
	MemoryObject() : m_parent{ nullptr }, m_vars(std::map<std::string, Memory*>()) {}
	MemoryObject(MemoryObject* parent);
	~MemoryObject();

	void add(std::string name, Object* object, Interface* type);
	void set(std::string name, Object* object);

	bool containKey(std::string name);

	Object* get(std::string name);

	size_t size();
};
#pragma once
#include <string>
#include "Command.h"
class Class;

class Object {
protected:
	Class* m_type;

	Object** m_vars;
	size_t m_size;
	Object(Class* type, Object** vars, size_t size);
public:
	Object(Class* type);

	virtual ~Object();

	Class* getClass() { return m_type; }
	size_t getVarsSize() { return m_size; }
	Object* get(std::string);
	Object* get(size_t i);
	void set(size_t i, Object* obj);

	virtual Object* clone();
};
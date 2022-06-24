#pragma once
#include <string>
#include "Command.h"
#include "Global.h"
class Class;

class IObject {
protected:
	Class* m_type;
public:
	IObject(Class* type) :m_type(type) {}

	virtual ~IObject() = default;

	Class* getClass() { return m_type; }
	virtual IObject* clone() = 0;
};

class NullObject : public IObject {
public:
	NullObject() :IObject(GLOBAL::getClasses()->getClass(Paths::Object)) {}
	NullObject(Interface* _interface) :IObject(dynamic_cast<Class*>(_interface) == nullptr ? GLOBAL::getClasses()->getClass(Paths::Object) : (Class*)_interface) {}
	NullObject(Class* suposedType) : IObject(suposedType) {}
	IObject* clone() override { return new NullObject(m_type); }
};

class Object : public IObject {
protected:
	IObject** m_vars;
	size_t m_size;
	Object(Class* type, IObject** vars, size_t size);
public:
	Object(Class* type);

	virtual ~Object() override;

	size_t getVarsSize() { return m_size; }
	IObject* get(std::string);
	IObject* get(size_t i);
	void set(size_t i, IObject* obj);

	virtual IObject* clone() override;
};
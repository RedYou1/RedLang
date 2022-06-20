#pragma once
#include "Class.h"
#include "Command.h"
#include "GarbageCollector.h"

class StatVar {
private:
	Interface* m_type;
	Object* m_value;
public:
	StatVar(Interface* type, Object* value) :m_type(type), m_value(value) { GarbageCollector::Add(m_value); }
	~StatVar() { GarbageCollector::Remove(m_value); }

	Interface* GetType() { return m_type; }

	Object* GetValue() { return m_value; }

	void SetValue(Object* value) {
		GarbageCollector::Remove(m_value);
		m_value = value;
		GarbageCollector::Add(m_value);
	}
};
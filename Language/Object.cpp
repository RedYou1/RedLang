#include "Object.h"
#include "MemoryObject.h"
#include "MemoryVar.h"
#include "Class.h"
#include "Var.h"
#include "GarbageCollector.h"

Object::Object(Class* type, IObject** vars, size_t size)
	:IObject(type), m_vars(vars), m_size(size)
{
}

Object::Object(Class* type) : IObject(type), m_size(m_type->getVars()->size()) {
	m_vars = new IObject * [m_size];
	for (size_t c{ 0 }; c < m_size; c++) {
		if (m_type->getVars()->get(c)->m_default == nullptr) {
			m_vars[c] = nullptr;
		}
		else {
			MemoryObject m{ };
			CommandReturn* r{ m_type->getVars()->get(c)->m_default->exec(m) };
			m_vars[c] = r->getObject();
			GarbageCollector::Add(m_vars[c]);
			delete r;
		}
	}
}

Object::~Object() {
	for (size_t c{ 0 }; c < m_size; c++)
		GarbageCollector::Remove(m_vars[c]);
	delete[] m_vars;
}

IObject* Object::get(std::string name) {
	size_t i{ m_type->getVars()->get(name)->m_index };
	if (i >= m_size)
		throw "OutOfBound";
	return m_vars[i];
}

IObject* Object::get(size_t i) {
	if (i >= m_size)
		throw "OutOfBound";
	return m_vars[i];
}

void Object::set(size_t i, IObject* obj)
{
	if (i >= m_size)
		throw "OutOfBound";
	GarbageCollector::Remove(m_vars[i]);
	GarbageCollector::Add(obj);
	m_vars[i] = obj;
}

IObject* Object::clone()
{
	IObject** vars{ new IObject * [m_size] };
	for (size_t c(0); c < m_size; c++) {
		vars[c] = m_vars[c]->clone();
		GarbageCollector::Add(vars[c]);
	}
	return new Object(m_type, vars, m_size);
}

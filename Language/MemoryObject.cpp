#include "MemoryObject.h"
#include "GarbageCollector.h"

MemoryObject::MemoryObject(MemoryObject* parent)
	:m_parent(parent), m_vars(std::map<std::string, Memory*>())
{
}

MemoryObject::~MemoryObject() {
	for (std::map<std::string, Memory*>::iterator it{ m_vars.begin() }; it != m_vars.end(); it++) {
		delete it->second;
	}
}

void MemoryObject::add(std::string name, Object* o, Interface* type) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::string, Memory*>(name, new Memory{ o ,type }));
}

void MemoryObject::set(std::string name, Object* o) {
	typename std::map<std::string, Memory*>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent != nullptr)
			return m_parent->set(name, o);
		else
			throw "not found";
	}
	if (it->second->getObject() == o)
		return;
	it->second->setObject(o);
}

bool MemoryObject::containKey(std::string name) {
	return m_vars.find(name) != m_vars.end() || (m_parent != nullptr && m_parent->containKey(name));
}

Object* MemoryObject::get(std::string name) {
	typename std::map<std::string, Memory*>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent != nullptr)
			return m_parent->get(name);
		else
			throw "not found";
	}
	return it->second->getObject();
}

size_t MemoryObject::size() {
	return m_vars.size();
}

MemoryObject::Memory::Memory(Object* object, Interface* type)
	:m_object(object), m_type(type)
{
	GarbageCollector::Add(m_object);
}

MemoryObject::Memory::~Memory()
{
	GarbageCollector::Remove(m_object);
}

Object* MemoryObject::Memory::getObject()
{
	return m_object;
}

Interface* MemoryObject::Memory::getType()
{
	return m_type;
}

void MemoryObject::Memory::setObject(Object* object)
{
	if (m_object == object)
		return;
	GarbageCollector::Remove(m_object);
	GarbageCollector::Add(object);
	m_object = object;
}

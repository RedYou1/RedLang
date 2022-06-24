#include "MemoryObject.h"
#include "GarbageCollector.h"

DTO::MemoryObject::MemoryObject(MemoryObject* parent)
	:m_parent(parent), m_vars(std::map<std::string, Memory*>())
{
}

DTO::MemoryObject::~MemoryObject() {
	for (std::map<std::string, Memory*>::iterator it{ m_vars.begin() }; it != m_vars.end(); it++) {
		delete it->second;
	}
}

void DTO::MemoryObject::add(std::string name, IObject* o, Interface* type) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::string, Memory*>(name, new Memory{ o ,type }));
}

void DTO::MemoryObject::set(std::string name, IObject* o) {
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

bool DTO::MemoryObject::containKey(std::string name) {
	return m_vars.find(name) != m_vars.end() || (m_parent != nullptr && m_parent->containKey(name));
}

DTO::IObject* DTO::MemoryObject::get(std::string name) {
	typename std::map<std::string, Memory*>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent != nullptr)
			return m_parent->get(name);
		else
			throw "not found";
	}
	return it->second->getObject();
}

size_t DTO::MemoryObject::size() {
	return m_vars.size();
}

DTO::MemoryObject::Memory::Memory(IObject* object, Interface* type)
	:m_object(object), m_type(type)
{
	GarbageCollector::Add(m_object);
}

DTO::MemoryObject::Memory::~Memory()
{
	GarbageCollector::Remove(m_object);
}

DTO::IObject* DTO::MemoryObject::Memory::getObject()
{
	return m_object;
}

DTO::Interface* DTO::MemoryObject::Memory::getType()
{
	return m_type;
}

void DTO::MemoryObject::Memory::setObject(IObject* object)
{
	if (m_object == object)
		return;
	GarbageCollector::Remove(m_object);
	GarbageCollector::Add(object);
	m_object = object;
}

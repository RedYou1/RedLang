#include "MemoryObject.h"

DTO::MemoryObject::MemoryObject(MemoryObject* parent)
	:m_parent(parent), m_vars(std::map<std::wstring, Memory*>())
{
}

DTO::MemoryObject::~MemoryObject() {
	for (std::map<std::wstring, Memory*>::iterator it{ m_vars.begin() }; it != m_vars.end(); it++) {
		delete it->second;
	}
}

void DTO::MemoryObject::add(std::wstring name, IObject* o, Type type) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	if (!type.nullable &&
		dynamic_cast<NullObject*>(o) != nullptr)
		throw "null obj";
	m_vars.insert(std::pair<std::wstring, Memory*>(name, new Memory{ o ,type }));
}

void DTO::MemoryObject::set(std::wstring name, IObject* o) {
	typename std::map<std::wstring, Memory*>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent != nullptr)
			return m_parent->set(name, o);
		else
			throw "not found";
	}
	if (!it->second->getType().nullable &&
		dynamic_cast<NullObject*>(o) != nullptr)
		throw "null obj";
	if (it->second->getObject() == o)
		return;
	it->second->setObject(o);
}

bool DTO::MemoryObject::isNullable(std::wstring name)
{
	typename std::map<std::wstring, Memory*>::iterator it(m_vars.find(name));
	if (it == m_vars.end()) {
		if (m_parent != nullptr)
			return m_parent->isNullable(name);
		else
			throw "not found";
	}
	return it->second->getType().nullable;
}

bool DTO::MemoryObject::containKey(std::wstring name) {
	return m_vars.find(name) != m_vars.end() || (m_parent != nullptr && m_parent->containKey(name));
}

DTO::IObject* DTO::MemoryObject::get(std::wstring name) {
	typename std::map<std::wstring, Memory*>::iterator it(m_vars.find(name));
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

DTO::MemoryObject::Memory::Memory(IObject* object, Type type)
	:m_object(object), m_type(type)
{
	m_object->addRef();
}

DTO::MemoryObject::Memory::~Memory()
{
	m_object->removeRef();
}

DTO::IObject* DTO::MemoryObject::Memory::getObject()
{
	return m_object;
}

DTO::Type DTO::MemoryObject::Memory::getType()
{
	return m_type;
}

void DTO::MemoryObject::Memory::setObject(IObject* object)
{
	if (m_object == object)
		return;
	m_object->removeRef();
	object->addRef();
	m_object = object;
}

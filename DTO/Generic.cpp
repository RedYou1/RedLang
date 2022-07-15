#include "Generic.h"
#include "Interface.h"
#include "MemorySourceFile.h"

void DTO::GenericDynamic::add(GenPossibility* gens, size_t genSize, SourceFile* _class)
{
	m_classes.push_front(std::pair<std::vector<GenPossibility>, SourceFile*>(std::vector<GenPossibility>(&gens[0], &gens[genSize]), _class));
}

bool DTO::GenericDynamic::contains(Instanciable** gens, size_t genSize)
{
	for (std::forward_list<std::pair<std::vector<GenPossibility>, SourceFile*>>::iterator it = m_classes.begin(); it != m_classes.end(); it++) {
		if (it->first.size() != genSize)
			continue;

		bool ok{ true };
		for (size_t i{ 0 }; i < genSize; i++) {
			if (!it->first[i].isOk(gens[i])) {
				ok = false;
				break;
			}
		}
		if (ok)
			return true;
	}
	return false;
}

DTO::SourceFile* DTO::GenericDynamic::get(Instanciable** gens, size_t genSize)
{
	for (std::forward_list<std::pair<std::vector<GenPossibility>, SourceFile*>>::iterator it = m_classes.begin(); it != m_classes.end(); it++) {
		if (it->first.size() != genSize)
			continue;

		bool ok{ true };
		for (size_t i{ 0 }; i < genSize; i++) {
			if (!it->first[i].isOk(gens[i])) {
				ok = false;
				break;
			}
		}
		if (ok)
			return it->second;
	}
	throw "??";
}

DTO::GenericDynamic::~GenericDynamic()
{
	for (std::forward_list<std::pair<std::vector<GenPossibility>, SourceFile*>>::iterator it = m_classes.begin(); it != m_classes.end(); it++) {
		delete it->second;
	}
}

void DTO::GenericStatic::add(GenPossibility* gens, size_t genSize, SourceFile* _class)
{
	if (genSize != m_genSize)
		throw "??";
	m_classes.push_front(std::pair<GenPossibility*, SourceFile*>(gens, _class));
}

DTO::GenericStatic::~GenericStatic()
{
	for (std::forward_list<std::pair<GenPossibility*, SourceFile*>>::iterator it = m_classes.begin(); it != m_classes.end(); it++) {
		delete[] it->first;
		delete it->second;
	}
}

bool DTO::GenericStatic::contains(Instanciable** gens, size_t genSize)
{
	if (m_genSize != genSize)
		throw "??";

	for (std::forward_list<std::pair<GenPossibility*, SourceFile*>>::iterator it = m_classes.begin(); it != m_classes.end(); it++) {
		bool ok{ true };
		for (size_t i{ 0 }; i < genSize; i++) {
			if (!it->first[i].isOk(gens[i])) {
				ok = false;
				break;
			}
		}
		if (ok)
			return true;
	}
	return false;
}

DTO::SourceFile* DTO::GenericStatic::get(Instanciable** gens, size_t genSize)
{
	if (m_genSize != genSize)
		throw "??";

	for (std::forward_list<std::pair<GenPossibility*, SourceFile*>>::iterator it = m_classes.begin(); it != m_classes.end(); it++) {
		bool ok{ true };
		for (size_t i{ 0 }; i < genSize; i++) {
			if (!it->first[i].isOk(gens[i])) {
				ok = false;
				break;
			}
		}
		if (ok)
			return it->second;
	}
	throw "??";
}

bool DTO::GenericI::instanceOf(Instanciable* other)
{
	if (getPath() != other->getPath())
		return false;
	if (GenericI * o{ dynamic_cast<GenericI*>(other) }) {
		for (size_t i{ 0 }; i < m_possSize; i++) {
			if (!m_possibilities[i].isOk(o->m_possibilities[i]))
				return false;
		}
		return true;
	}
	else if (Interface * o2{ dynamic_cast<Interface*>(other) }) {
		for (size_t i{ 0 }; i < m_possSize; i++) {
			if (!m_possibilities[i].isOk(o2->getGenTypes()->getType(m_names[i])))
				return false;
		}
		return true;
	}
	else {
		throw "??";
	}
}

DTO::GenPossibility::GenPossibility()
	:m_type(nullptr), m_possibilitiesLen(0)
{
}

DTO::GenPossibility::GenPossibility(Instanciable* type)
	: m_type(type), m_possibilitiesLen{ 0 }
{
}

DTO::GenPossibility::GenPossibility(Instanciable** possibilities, size_t possibilitiesLen)
	: m_possibilities(possibilities), m_possibilitiesLen(possibilitiesLen)
{
}

DTO::GenPossibility::~GenPossibility()
{
	if (m_possibilitiesLen != 0)
		delete[] m_possibilities;
}

bool DTO::GenPossibility::isOk(Instanciable* type)
{
	if (m_possibilitiesLen == 0) {
		return type == m_type;
	}
	else {
		for (size_t i{ 0 }; i < m_possibilitiesLen; i++) {
			if (!type->instanceOf(m_possibilities[i]))
				return false;
		}
		return true;
	}
}

bool DTO::GenPossibility::isOk(GenPossibility& type)
{
	if (type.m_possibilitiesLen == 0) {
		if (m_possibilities != 0)
			return false;
		return type.m_type == m_type;
	}
	else {
		if (m_possibilitiesLen == 0) {
			return type.isOk(m_type);
		}
		else {
			throw "IDK really complicated";
		}
	}
}

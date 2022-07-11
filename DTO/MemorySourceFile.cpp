#include "MemorySourceFile.h"
#include "Command.h"
#include "Class.h"
#include "myString.h"
#include "Generic.h"

DTO::MemorySourceFile::~MemorySourceFile() {
	if (!m_delete)
		return;
	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		delete it->second;
	}
}

void DTO::MemorySourceFile::add(std::string name, SourceFile* o) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::string, SourceFile*>(name, o));
}

void DTO::MemorySourceFile::set(std::string name, SourceFile* o) {
	typename std::map<std::string, SourceFile*>::iterator it(m_vars.find(name));
	if (it == m_vars.end())
		throw "not found";
	if (it->second != nullptr)
		throw "??";
	it->second = o;
}

bool DTO::MemorySourceFile::containKey(std::string* name, MemorySourceFile* _genTypes) {
	if (m_vars.empty())
		return false;
	bool found{ false };
	SourceFile* s{ nullptr };

	std::string cName{ *name };

	size_t lastDot{ cName.find_last_of('.') };
	if (lastDot == SIZE_MAX) {
		lastDot = 0;
	}

	size_t firstTri{ cName.substr(lastDot).find('<') };
	if (firstTri == SIZE_MAX) {
		firstTri = cName.size();
	}

	std::string genName{ cName.substr(0,lastDot + firstTri) };

	size_t nameSize{ genName.size() };

	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - nameSize, nameSize, genName) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == '.')) {
			if (s != nullptr)
				throw "multiple classes";
			s = it->second;
			found = true;
		}
	}

	if (found && cName.at(cName.size() - 1) == '>') {
		std::string genTypes{ myString{ &cName }.split(".").back() };
		myString{ &genTypes }.extract2();
		std::queue<std::string> q{ myString{&myString{&genTypes}.extractFunc2() }.split2(',') };
		size_t size{ q.size() };
		Instanciable** genTypesArr{ new Instanciable * [size] };
		size_t i{ 0 };
		size_t charIndex{ lastDot + firstTri + 1 };
		while (i < size) {
			std::string genType{ q.front() };
			if (containKey(&genType, _genTypes)) {
				genTypesArr[i] = getType(genType);
				if (name->substr(charIndex).find(genType) != 0) {
					*name = name->substr(0, charIndex) + genType + name->substr(charIndex + q.front().size());
					charIndex += genType.size() - q.front().size();
				}
			}
			else if (_genTypes != nullptr && _genTypes->containKey(&genType)) {
				genTypesArr[i] = _genTypes->getType(genType);
				*name = name->substr(0, charIndex) + genTypesArr[i]->getName() + name->substr(charIndex + q.front().size());
				charIndex += genTypesArr[i]->getName().size() - q.front().size();
			}
			else {
				delete[] genTypesArr;
				return false;
			}
			charIndex += q.front().size() + 1;
			q.pop();
			i++;
		}
		if (i < size || !q.empty())
			throw "??";
		if (!((Generic*)s)->contains(genTypesArr, size)) {
			((Generic*)s)->create(*name, genTypesArr, size);
		}
		delete[] genTypesArr;
		return true;
	}

	return found;
}

DTO::SourceFile* DTO::MemorySourceFile::get(std::string name) {
	SourceFile* s{ nullptr };

	std::string cName{ name };

	size_t lastDot{ cName.find_last_of('.') };
	if (lastDot == SIZE_MAX) {
		lastDot = 0;
	}

	size_t firstTri{ cName.substr(lastDot).find('<') };
	if (firstTri == SIZE_MAX) {
		firstTri = cName.size();
	}

	std::string genName{ cName.substr(0,lastDot + firstTri) };

	size_t nameSize{ genName.size() };

	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - nameSize, nameSize, genName) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == '.')) {
			if (s != nullptr)
				throw "multiple classes";
			s = it->second;
		}
	}

	if (cName.at(cName.size() - 1) == '>') {
		if (s == nullptr)
			throw "not found";
		std::string genTypes{ myString{ &cName }.split(".").back() };
		myString{ &genTypes }.extract2();
		std::queue<std::string> q{ myString{&myString{&genTypes}.extractFunc2() }.split2(',') };
		size_t size{ q.size() };
		Instanciable** genTypesArr{ new Instanciable * [size] };
		size_t i{ 0 };
		size_t charIndex{ genName.size() + 1 };
		bool a{ false };
		while (i < size) {
			std::string genType{ q.front() };
			if (containKey(&genType)) {
				genTypesArr[i] = getType(genType);
			}
			else {
				delete[] genTypesArr;
				throw "??";
			}
			charIndex += q.front().size() + 1;
			q.pop();
			i++;
		}
		if (i < size || !q.empty())
			throw "??";

		SourceFile* r{ ((Generic*)s)->get(genTypesArr, size) };
		delete[] genTypesArr;
		return r;
	}

	return s;
}

DTO::SourceFile* DTO::MemorySourceFile::checkGet(std::string name)
{
	std::string sname{ name };
	bool check{ containKey(&sname) };
	if (!check)
		throw "??";
	return get(sname);
}

DTO::Instanciable* DTO::MemorySourceFile::getType(std::string name)
{
	SourceFile* s{ get(name) };
	Instanciable* i{ dynamic_cast<Instanciable*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Interface* DTO::MemorySourceFile::getInterface(std::string name)
{
	SourceFile* s{ get(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Class* DTO::MemorySourceFile::getClass(std::string name)
{
	SourceFile* s{ get(name) };
	Class* i{ dynamic_cast<Class*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not a Class";
	return i;
}

DTO::Instanciable* DTO::MemorySourceFile::checkGetType(std::string name)
{
	SourceFile* s{ checkGet(name) };
	Instanciable* i{ dynamic_cast<Instanciable*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Interface* DTO::MemorySourceFile::checkGetInterface(std::string name)
{
	SourceFile* s{ checkGet(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Class* DTO::MemorySourceFile::checkGetClass(std::string name)
{
	SourceFile* s{ checkGet(name) };
	Class* i{ dynamic_cast<Class*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not a Class";
	return i;
}

size_t DTO::MemorySourceFile::size() {
	return m_vars.size();
}
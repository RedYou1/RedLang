#include "MemorySourceFile.h"
#include "Command.h"
#include "Class.h"
#include "myString.h"
#include "Generic.h"

DTO::MemorySourceFile::~MemorySourceFile() {
	if (!m_delete)
		return;
	for (typename std::map<std::wstring, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		delete it->second;
	}
}

void DTO::MemorySourceFile::add(std::wstring name, SourceFile* o) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::wstring, SourceFile*>(name, o));
}

void DTO::MemorySourceFile::set(std::wstring name, SourceFile* o) {
	typename std::map<std::wstring, SourceFile*>::iterator it(m_vars.find(name));
	if (it == m_vars.end())
		throw "not found";
	if (it->second != nullptr)
		throw "??";
	it->second = o;
}

bool DTO::MemorySourceFile::containKey(std::wstring* name, MemorySourceFile* _genTypes) {
	if (m_vars.empty())
		return false;
	bool found{ false };
	SourceFile* s{ nullptr };

	std::wstring cName{ *name };

	std::wstring temp{ L"<" + *name + L">" };

	std::queue<std::wstring> q{ myString{&temp}.split2(L'.') };

	size_t first{ cName.size() };
	size_t t{ q.back().find('<') };
	if (t != SIZE_MAX) {
		first = first - q.back().size() + t;
	}

	std::wstring genName{ cName.substr(0, first) };

	size_t nameSize{ genName.size() };

	for (typename std::map<std::wstring, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - nameSize, nameSize, genName) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == L'.')) {
			if (s != nullptr)
				throw "multiple classes";
			s = it->second;
			found = true;
		}
	}

	if (found && cName.at(cName.size() - 1) == L'>') {
		std::wstring genTypes{ q.back() };
		myString{ &genTypes }.extract2();
		temp = myString{ &genTypes }.extractFunc2();
		q = myString{ &temp }.split2(L',');
		size_t size{ q.size() };
		Instanciable** genTypesArr{ new Instanciable * [size] };
		size_t i{ 0 };
		size_t charIndex{ first + 1 };
		while (i < size) {
			std::wstring genType{ q.front() };
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

DTO::SourceFile* DTO::MemorySourceFile::get(std::wstring name) {
	SourceFile* s{ nullptr };

	std::wstring cName{ name };

	std::wstring temp{ L"<" + name + L">" };

	std::queue<std::wstring> q{ myString{&temp}.split2(L'.') };

	size_t first{ cName.size() };
	size_t t{ q.back().find('<') };
	if (t != SIZE_MAX) {
		first = first - q.back().size() + t;
	}

	std::wstring genName{ cName.substr(0, first) };

	size_t nameSize{ genName.size() };

	for (typename std::map<std::wstring, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - nameSize, nameSize, genName) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == L'.')) {
			if (s != nullptr)
				throw "multiple classes";
			s = it->second;
		}
	}

	if (cName.at(cName.size() - 1) == L'>') {
		if (s == nullptr)
			throw "not found";
		std::wstring genTypes{ q.back() };
		myString{ &genTypes }.extract2();
		temp = myString{ &genTypes }.extractFunc2();
		q = myString{ &temp }.split2(L',');
		size_t size{ q.size() };
		Instanciable** genTypesArr{ new Instanciable * [size] };
		size_t i{ 0 };
		bool a{ false };
		while (i < size) {
			std::wstring genType{ q.front() };
			if (containKey(&genType)) {
				genTypesArr[i] = getType(genType);
			}
			else {
				delete[] genTypesArr;
				throw "??";
			}
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

DTO::SourceFile* DTO::MemorySourceFile::checkGet(std::wstring name)
{
	std::wstring sname{ name };
	bool check{ containKey(&sname) };
	if (!check)
		throw "??";
	return get(sname);
}

DTO::Instanciable* DTO::MemorySourceFile::getType(std::wstring name)
{
	SourceFile* s{ get(name) };
	Instanciable* i{ dynamic_cast<Instanciable*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Interface* DTO::MemorySourceFile::getInterface(std::wstring name)
{
	SourceFile* s{ get(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Class* DTO::MemorySourceFile::getClass(std::wstring name)
{
	SourceFile* s{ get(name) };
	Class* i{ dynamic_cast<Class*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not a Class";
	return i;
}

DTO::Instanciable* DTO::MemorySourceFile::checkGetType(std::wstring name)
{
	SourceFile* s{ checkGet(name) };
	Instanciable* i{ dynamic_cast<Instanciable*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Interface* DTO::MemorySourceFile::checkGetInterface(std::wstring name)
{
	SourceFile* s{ checkGet(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

DTO::Class* DTO::MemorySourceFile::checkGetClass(std::wstring name)
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
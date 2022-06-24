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
	bool s{ false };
	size_t nameSize{ name->size() };

	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - name->length(), name->length(), *name) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == '.')) {
			if (s)
				throw "multiple classes";
			s = true;
		}
	}

	if (!s && name->at(nameSize - 1) == '>') {
		std::string genTypes{ myString{ name }.split(".").back() };
		std::string genName{ myString{&genTypes}.extract2() };
		if (!containKey(&genName))
			return false;
		Generic* gen{ (Generic*)get(genName) };
		std::queue<std::string> q{ myString{&myString{&genTypes}.extractFunc2() }.split2(',') };
		size_t size{ q.size() };
		SourceFile** genTypesArr{ new SourceFile * [size] };
		size_t i{ 0 };
		size_t charIndex{ genName.size() + 1 };
		bool a{ false };
		while (i < size) {
			std::string genType{ q.front() };
			if (containKey(&genType, _genTypes)) {
				genTypesArr[i] = get(genType);
				if (name->substr(charIndex).find(genType) != 0) {
					a = true;
					*name = name->substr(0, charIndex) + genType + name->substr(charIndex + q.front().size());
					charIndex += genType.size() - q.front().size();
				}
			}
			else if (_genTypes != nullptr && _genTypes->containKey(&genType)) {
				genTypesArr[i] = _genTypes->get(genType);
				a = true;
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
		if (!a || !containKey(name)) {
			add(*name, nullptr);
			gen->create(*name, genTypesArr, size);
		}
		delete[] genTypesArr;
		return true;
	}

	return s;
}

DTO::SourceFile* DTO::MemorySourceFile::get(std::string name) {
	bool found{ false };
	SourceFile* s{ nullptr };
	size_t nameSize{ name.size() };

	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - name.length(), name.length(), name) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == '.')) {
			if (found)
				throw "multiple classes";
			s = it->second;
			found = true;
		}
	}

	if (!found)
		throw "not found";
	return s;
}

DTO::Interface* DTO::MemorySourceFile::getType(std::string name)
{
	SourceFile* s{ get(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
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

size_t DTO::MemorySourceFile::size() {
	return m_vars.size();
}
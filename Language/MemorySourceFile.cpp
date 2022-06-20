#include "MemorySourceFile.h"
#include "Command.h"
#include "Class.h"
#include "myString.h"
#include "Generic.h"

MemorySourceFile::MemorySourceFile(MemorySourceFile& dup)
	:m_delete(dup.m_delete), m_vars(std::map<std::string, SourceFile*>())
{
	for (typename std::map<std::string, SourceFile*>::iterator it = dup.m_vars.begin(); it != dup.m_vars.end(); ++it) {
		m_vars.insert(std::pair< std::string, SourceFile*>(it->first, it->second));
	}
}

MemorySourceFile::~MemorySourceFile() {
	if (!m_delete)
		return;
	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		delete it->second;
	}
}

void MemorySourceFile::add(std::string name, SourceFile* o) {
	if (m_vars.find(name) != m_vars.end())
		throw "variable already exists";
	m_vars.insert(std::pair<std::string, SourceFile*>(name, o));
}

void MemorySourceFile::set(std::string name, SourceFile* o) {
	typename std::map<std::string, SourceFile*>::iterator it(m_vars.find(name));
	if (it == m_vars.end())
		throw "not found";
	if (it->second != nullptr)
		throw "??";
	it->second = o;
}

bool MemorySourceFile::containKey(std::string name) {
	if (m_vars.empty())
		return false;
	bool s{ false };
	size_t nameSize{ name.size() };

	for (typename std::map<std::string, SourceFile*>::iterator it = m_vars.begin(); it != m_vars.end(); ++it) {
		size_t itSize{ it->first.size() };

		if (itSize < nameSize)
			continue;

		if (0 == it->first.compare(it->first.length() - name.length(), name.length(), name) &&
			(itSize == nameSize || it->first.at(itSize - nameSize - 1) == '.')) {
			if (s)
				throw "multiple classes";
			s = true;
		}
	}

	if (!s && name.at(nameSize - 1) == '>') {
		myString ms{ &name };
		std::string genTypes{ ms.split(".").back() };
		SourceFile* gen{ get(myString{&genTypes}.extract2()) };
		std::queue<std::string> q{ myString{&myString{&genTypes}.extractFunc2() }.split2(',') };
		size_t size{ q.size() };
		SourceFile** genTypesArr{ new SourceFile * [size] };
		size_t i{ 0 };
		while (i < size) {
			genTypesArr[i] = get(q.front());
			q.pop();
			i++;
		}
		if (i < size || !q.empty())
			throw "??";
		add(name, nullptr);
		((Generic*)gen)->create(name, genTypesArr, size);
		delete[] genTypesArr;
		return true;
	}

	return s;
}

SourceFile* MemorySourceFile::get(std::string name) {
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

Interface* MemorySourceFile::getType(std::string name)
{
	SourceFile* s{ get(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

Interface* MemorySourceFile::getInterface(std::string name)
{
	SourceFile* s{ get(name) };
	Interface* i{ dynamic_cast<Interface*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not an Interface";
	return i;
}

Class* MemorySourceFile::getClass(std::string name)
{
	SourceFile* s{ get(name) };
	Class* i{ dynamic_cast<Class*>(s) };
	if (s != nullptr && i == nullptr)
		throw "not a Class";
	return i;
}

size_t MemorySourceFile::size() {
	return m_vars.size();
}
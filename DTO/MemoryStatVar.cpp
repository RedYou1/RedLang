#include "MemoryStatVar.h"
#include "StatVar.h"
#include "Command.h"
#include "Object.h"

DTO::MemoryStatVar::MemoryStatVar()
	: m_string{}
{
}

DTO::MemoryStatVar::~MemoryStatVar()
{
	for (typename std::map<std::wstring, StatVar*>::iterator it = m_string.begin(); it != m_string.end(); ++it) {
		delete it->second;
	}
}

void DTO::MemoryStatVar::add(std::wstring name, StatVar* var)
{
	m_string.insert(std::pair<std::wstring, StatVar*>(name, var));
}

DTO::StatVar* DTO::MemoryStatVar::get(std::wstring name)
{
	if (m_string.find(name) == m_string.end())
		throw "does not contain this name.";
	return m_string[name];
}

void DTO::MemoryStatVar::set(std::wstring name, IObject* value)
{
	StatVar* var{ get(name) };
	if (!value->getClass()->instanceOf(var->GetType()))
		throw "CastException";
	var->SetValue(value);
}

bool DTO::MemoryStatVar::containKey(std::wstring name)
{
	return m_string.find(name) != m_string.end();
}

size_t DTO::MemoryStatVar::size()
{
	return m_string.size();
}
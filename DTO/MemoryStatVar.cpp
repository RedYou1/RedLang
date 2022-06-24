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
	for (typename std::map<std::string, StatVar*>::iterator it = m_string.begin(); it != m_string.end(); ++it) {
		delete it->second;
	}
}

void DTO::MemoryStatVar::add(std::string name, StatVar* var)
{
	m_string.insert(std::pair<std::string, StatVar*>(name, var));
}

DTO::StatVar* DTO::MemoryStatVar::get(std::string name)
{
	if (m_string.find(name) == m_string.end())
		throw "does not contain this name.";
	return m_string[name];
}

void DTO::MemoryStatVar::set(std::string name, IObject* value)
{
	StatVar* var{ get(name) };
	if (!value->getClass()->instanceOf(var->GetType()))
		throw "CastException";
	var->SetValue(value);
}

bool DTO::MemoryStatVar::containKey(std::string name)
{
	return m_string.find(name) != m_string.end();
}

size_t DTO::MemoryStatVar::size()
{
	return m_string.size();
}
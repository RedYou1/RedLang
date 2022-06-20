#include "MemoryVar.h"
#include "Var.h"

MemoryVar::MemoryVar()
	:MemoryVar(nullptr) {}

MemoryVar::MemoryVar(MemoryVar* parent)
	: m_parent{ parent }, m_int{ new Var * [0] }, m_string{}, m_min{ m_parent ? m_parent->m_max : 0 }, m_max{ m_min }
{
}

MemoryVar::~MemoryVar()
{
	delete[] m_int;
	for (typename std::map<std::string, Var*>::iterator it = m_string.begin(); it != m_string.end(); ++it) {
		delete it->second;
	}
}

Var* MemoryVar::get(size_t i)
{
	if (m_parent != nullptr && i < m_min)
		return m_parent->get(i);
	if (!containKey(i))
		throw "does not contain this index";
	return m_int[i - m_min];
}

void MemoryVar::add(std::string name, Var* var)
{
	size_t _size{ thisSize() };
	m_string.insert(std::pair<std::string, Var*>(name, var));
	Var** t{ new Var * [_size + 1] };
	memcpy(t, m_int, _size * sizeof(Var*));
	t[_size] = var;
	delete[] m_int;
	m_int = t;
	if (var->m_index >= m_max)
		m_max = var->m_index + 1;
}

Var* MemoryVar::get(std::string name)
{
	if (m_string.find(name) == m_string.end()) {
		if (m_parent != nullptr)
			return m_parent->get(name);
		else
			throw "does not contain this name.";
	}
	return m_string[name];
}

bool MemoryVar::containKey(size_t i)
{
	return i < m_max;
}

bool MemoryVar::containKey(std::string name)
{
	return (m_parent != nullptr && m_parent->containKey(name)) || m_string.find(name) != m_string.end();
}

size_t MemoryVar::min()
{
	return m_min;
}

size_t MemoryVar::max()
{
	return m_max;
}

size_t MemoryVar::size()
{
	return (m_parent ? m_parent->size() : 0) + m_string.size();
}

size_t MemoryVar::thisSize()
{
	return m_string.size();
}
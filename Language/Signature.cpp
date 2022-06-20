#include "Signature.h"
#include "Interface.h"

Signature::Signature(std::string path, Interface* returnType, Interface** argsType, std::string* argsName, size_t argsLen)
	:m_path(path), m_returnType(returnType), m_argsType(argsType), m_argsName(argsName), m_argsLen(argsLen), m_infinite(false)
{}
Signature::Signature(std::string path, Interface* returnType, Interface** argsType, std::string* argsName, size_t argsLen, bool infinite)
	: m_path(path), m_returnType(returnType), m_argsType(argsType), m_argsName(argsName), m_argsLen(argsLen), m_infinite(infinite)
{}

Signature::~Signature() {
	delete[] m_argsType;
	delete[] m_argsName;
}

bool Signature::equalsI(Interface** argsType, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		if (m_argsLen == 0)
			return true;

		if (!m_argsType[0]->instanceOf(argsType[0]))
			return false;
		for (size_t c(1); c < m_argsLen; c++)
			if (argsType[c] != m_argsType[c])
				return false;
		return true;
	}
	return false;
}

bool Signature::similarI(Interface** argsType, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		for (size_t c(1); c < m_argsLen; c++)
			if (!argsType[c]->instanceOf(m_argsType[c]))
				return false;
		return true;
	}
	return false;
}

bool Signature::similar(Interface** argsType, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		for (size_t c(0); c < m_argsLen; c++)
			if (!argsType[c]->instanceOf(m_argsType[c]))
				return false;
		return true;
	}
	return false;
}

Signature* Signature::clone()
{
	Interface** argsType{ new Interface * [m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		argsType[c] = m_argsType[c];
	std::string* argsName{ new std::string[m_argsLen] };
	for (size_t c(0); c < m_argsLen; c++)
		argsName[c] = m_argsName[c];
	return new Signature(m_path, m_returnType, argsType, argsName, m_argsLen);
}

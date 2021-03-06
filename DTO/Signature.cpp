#include "Signature.h"
#include "Instanciable.h"
#include "Object.h"

DTO::Signature::Signature(std::filesystem::path path, Type returnType, Arg* args, size_t argsLen)
	:m_path(path), m_returnType(returnType), m_args(args), m_argsLen(argsLen), m_infinite(false)
{}
DTO::Signature::Signature(std::filesystem::path path, Type returnType, Arg* args, size_t argsLen, bool infinite)
	: m_path(path), m_returnType(returnType), m_args(args), m_argsLen(argsLen), m_infinite(infinite)
{}

DTO::Signature::~Signature() {
	delete[] m_args;
}

bool DTO::Signature::equalsI(Instanciable** argsType, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		if (m_argsLen == 0)
			return true;

		if (!m_args[0].type.type->instanceOf(argsType[0]))
			return false;
		for (size_t c(1); c < m_argsLen; c++)
			if (argsType[c] != m_args[c].type.type)
				return false;
		return true;
	}
	return false;
}

bool DTO::Signature::equalsI(Arg* args, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		if (m_argsLen == 0)
			return true;

		if (!m_args[0].type.type->instanceOf(args[0].type.type))
			return false;
		for (size_t c(1); c < m_argsLen; c++)
			if (args[c].type.type != m_args[c].type.type)
				return false;
		return true;
	}
	return false;
}

bool DTO::Signature::similarI(Instanciable** argsType, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		for (size_t c(1); c < m_argsLen; c++)
			if (!argsType[c]->instanceOf(m_args[c].type.type))
				return false;
		return true;
	}
	return false;
}

bool DTO::Signature::similar(Instanciable** argsType, size_t argsLen)
{
	if (m_argsLen == argsLen || (m_infinite && m_argsLen <= argsLen)) {
		for (size_t c(0); c < m_argsLen; c++)
			if (!argsType[c]->instanceOf(m_args[c].type.type))
				return false;
		return true;
	}
	return false;
}

bool DTO::Signature::similarI(IObject** args, size_t argsLen)
{
	if (m_argsLen < argsLen && !m_infinite)
		return false;
	if (m_argsLen > argsLen)
		for (size_t c(argsLen + 1); c < m_argsLen; c++)
			if (!m_args[c].type.nullable && m_args[c]._default == nullptr)
				return false;
	for (size_t c(1); c < argsLen && c < m_argsLen; c++)
		if (dynamic_cast<Object*>(args[c]) != nullptr && !((Object*)args[c])->getClass()->instanceOf(m_args[c].type.type))
			return false;
	return true;
}

bool DTO::Signature::similar(IObject** args, size_t argsLen)
{
	if (m_argsLen < argsLen && !m_infinite)
		return false;
	if (m_argsLen > argsLen)
		for (size_t c(argsLen + 1); c < m_argsLen; c++)
			if (!m_args[c].type.nullable && m_args[c]._default == nullptr)
				return false;
	for (size_t c(0); c < argsLen && c < m_argsLen; c++)
		if (dynamic_cast<Object*>(args[c]) != nullptr && !((Object*)args[c])->getClass()->instanceOf(m_args[c].type.type))
			return false;
	return true;
}

DTO::Signature* DTO::Signature::clone()
{
	Arg* args{ new Arg[m_argsLen] };
	memcpy(args, m_args, m_argsLen * sizeof(Arg));
	return new Signature(m_path, m_returnType, args, m_argsLen);
}

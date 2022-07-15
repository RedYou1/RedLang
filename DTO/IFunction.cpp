#include "IFunction.h"

DTO::Signature* DTO::IFunction::getSignature()
{
	return m_signature;
}

DTO::IFunction::IFunction(Signature* signature)
	:m_signature(signature)
{
}
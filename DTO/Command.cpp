#include "Command.h"
#include "Object.h"

DTO::CommandReturn::CommandReturn(IObject* object, bool _return, bool _throw)
	:m_object(object), m_return(_return), m_throw(_throw)
{
	if (m_object == nullptr)
		throw "??";
	m_object->addRef();
}

DTO::CommandReturn::CommandReturn(CommandReturn* value)
	: m_object(value->m_object), m_return(value->m_return), m_throw(value->m_throw)
{
	if (m_object == nullptr)
		throw "??";
	m_object->addRef();
}

DTO::CommandReturn::~CommandReturn()
{
	m_object->removeRef();
}

DTO::IObject* DTO::CommandReturn::getObject() { return m_object; }
bool DTO::CommandReturn::exitFunction()
{
	return m_return || m_throw;
}
bool DTO::CommandReturn::isReturn() { return m_return; }
void DTO::CommandReturn::setReturn(bool _return) { m_return = _return; }

bool DTO::CommandReturn::isThrow()
{
	return m_throw;
}

void DTO::CommandReturn::setThrow(bool _throw)
{
	m_throw = _throw;
}
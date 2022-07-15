#include "If.h"
#include "Boolean.h"
#include "CastException.h"

DTO::If::If(Command* cond, Command* block)
	:m_cond(cond), m_block(block)
{
}

DTO::If::~If()
{
	delete m_cond;
	delete m_block;
}

DTO::CommandReturn* DTO::If::exec(MemoryObject& pre_mem)
{
	MemoryObject mem{ &pre_mem };

	CommandReturn* t{ m_cond->exec(mem) };
	if (dynamic_cast<Object*>(t->getObject()) != nullptr &&
		!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"If", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
	bool v{ ((BooleanO*)t->getObject())->m_value };
	delete t;
	if (v)
		return m_block->exec(mem);
	return new CommandReturn(new NullObject(), false, false);
}

DTO::Command* DTO::If::clone()
{
	return new If(m_cond->clone(), m_block->clone());
}

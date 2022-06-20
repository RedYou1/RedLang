#include "If.h"
#include "Boolean.h"
#include "CastException.h"

If::If(Command* cond, Command* block)
	:m_cond(cond), m_block(block)
{
}

CommandReturn* If::exec(MemoryObject& pre_mem)
{
	MemoryObject mem{ &pre_mem };

	CommandReturn* t{ m_cond->exec(mem) };
	if (!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "If", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
	bool v{ ((BooleanO*)t->getObject())->m_value };
	delete t;
	if (v)
		return m_block->exec(mem);
	return new CommandReturn(nullptr, false, false);
}

Command* If::clone()
{
	return new If(m_cond->clone(), m_block->clone());
}

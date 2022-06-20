#include "While.h"
#include "Boolean.h"
#include "CastException.h"

While::While(Command* cond, Command* block)
	:m_cond(cond), m_block(block)
{
}

CommandReturn* While::exec(MemoryObject& pre_mem)
{
	MemoryObject mem{ &pre_mem };
	CommandReturn* t{ m_cond->exec(mem) };
	if (!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "While1", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
	bool v{ ((BooleanO*)t->getObject())->m_value };
	delete t;
	while (v) {
		CommandReturn* r;
		{
			MemoryObject bmem{ &mem };
			r = m_block->exec(bmem);
		}
		if (r->exitFunction())
			return r;
		delete r;
		t = m_cond->exec(mem);
		if (!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
			return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "While2", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
		v = ((BooleanO*)t->getObject())->m_value;
		delete t;
	}
	return new CommandReturn(nullptr, false, false);
}

Command* While::clone()
{
	return new While(m_cond->clone(), m_block->clone());
}


For::For(Command* init, Command* cond, Command* end, Command* block)
	:m_init(init), m_cond(cond), m_end(end), m_block(block)
{
}

CommandReturn* For::exec(MemoryObject& pre_mem)
{
	MemoryObject mem{ &pre_mem };

	delete m_init->exec(mem);

	CommandReturn* t{ m_cond->exec(mem) };
	if (!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "For1", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
	bool v{ ((BooleanO*)t->getObject())->m_value };
	delete t;
	while (v) {
		CommandReturn* r;
		{
			MemoryObject bmem{ &mem };
			r = m_block->exec(bmem);
		}
		if (r->exitFunction())
			return r;
		delete r;
		delete m_end->exec(mem);
		t = m_cond->exec(mem);
		if (!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
			return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), "For2", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
		v = ((BooleanO*)t->getObject())->m_value;
		delete t;
	}
	return new CommandReturn(nullptr, false, false);
}

Command* For::clone()
{
	return new For(m_init->clone(), m_cond->clone(), m_end->clone(), m_block->clone());
}

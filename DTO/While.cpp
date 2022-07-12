#include "While.h"
#include "Boolean.h"
#include "CastException.h"

DTO::While::While(Command* cond, Command* block)
	:m_cond(cond), m_block(block)
{
}

DTO::CommandReturn* DTO::While::exec(MemoryObject& pre_mem)
{
	MemoryObject mem{ &pre_mem };
	CommandReturn* t{ m_cond->exec(mem) };
	if (t->isThrow())
		return t;
	if (dynamic_cast<Object*>(t->getObject()) == nullptr ||
		!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"While1", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
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
		if (t->isThrow())
			return t;
		if (dynamic_cast<Object*>(t->getObject()) == nullptr ||
			!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
			return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"While2", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
		v = ((BooleanO*)t->getObject())->m_value;
		delete t;
	}
	return new CommandReturn(new NullObject(), false, false);
}

DTO::Command* DTO::While::clone()
{
	return new While(m_cond->clone(), m_block->clone());
}


DTO::For::For(Command* init, Command* cond, Command* end, Command* block)
	:m_init(init), m_cond(cond), m_end(end), m_block(block)
{
}

DTO::CommandReturn* DTO::For::exec(MemoryObject& pre_mem)
{
	MemoryObject mem{ &pre_mem };

	CommandReturn* t{ m_init->exec(mem) };
	if (t->isThrow())
		return t;
	delete t;

	t = m_cond->exec(mem);
	if (t->isThrow())
		return t;
	if (dynamic_cast<Object*>(t->getObject()) == nullptr ||
		!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
		return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"For1", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
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

		t = m_end->exec(mem);
		if (t->isThrow())
			return t;
		delete t;

		t = m_cond->exec(mem);
		if (t->isThrow())
			return t;
		if (dynamic_cast<Object*>(t->getObject()) == nullptr ||
			!t->getObject()->getClass()->instanceOf(GLOBAL::getClasses()->getClass(Paths::Boolean)))
			return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), L"For2", t, GLOBAL::getClasses()->getClass(Paths::Boolean)), false, true);
		v = ((BooleanO*)t->getObject())->m_value;
		delete t;
	}
	return new CommandReturn(new NullObject(), false, false);
}

DTO::Command* DTO::For::clone()
{
	return new For(m_init->clone(), m_cond->clone(), m_end->clone(), m_block->clone());
}

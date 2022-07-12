#pragma once
#include <string>
#include "Command.h"
#include "Class.h"
#include "Object.h"

#include "Function.h"
#include "FunctionClass.h"

namespace DTO {
	class While : public Command {
	private:
		Command* m_cond;
		Command* m_block;
	public:
		While(Command* cond, Command* block);
		virtual ~While() override { delete m_cond; delete m_block; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};

	class For : public Command {
	private:
		Command* m_init;
		Command* m_cond;
		Command* m_end;
		Command* m_block;
	public:
		For(Command* init, Command* cond, Command* end, Command* block);
		virtual ~For() override { delete m_init; delete m_cond; delete m_end; delete m_block; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};

	class ForEach :public Command {
	private:
		Instanciable* m_type;
		std::wstring m_name;
		Command* m_iterable;
		Function* m_func;
	public:
		ForEach(Instanciable* type, std::wstring name, Command* iterable, Function* func) :
			m_type(type), m_name(name), m_iterable(iterable), m_func(func) {}
		~ForEach() { delete m_iterable; delete m_func; }
		CommandReturn* exec(MemoryObject& mem) override {
			CommandReturn* q{ m_iterable->exec(mem) };

			FunctionO* func{ new FunctionO(GLOBAL::getClasses()->getClass(Paths::Function),m_func) };

			IObject** args{ new IObject * [2]{q->getObject(),func} };
			delete q->getObject()->exec(L"forEach", args, 2);

			delete[] args;
			delete q;
			return new CommandReturn(new NullObject(), false, false);
		}
		Command* clone()override { return new ForEach(m_type, m_name, m_iterable->clone(), (Function*)m_func->clone()); }
	};
}
#pragma once
#include "Command.h"
#include "Type.h"
#include "Function.h"

namespace DTO {
	class ForEach :public Command {
	private:
		Type m_type;
		std::wstring m_name;
		Command* m_iterable;
		Function* m_func;
	public:
		ForEach(Type type, std::wstring name, Command* iterable, Function* func);
		~ForEach() override;
		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
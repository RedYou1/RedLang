#pragma once
#include <string>
#include "Command.h"
#include "Class.h"
#include "Object.h"

#include "Function.h"

namespace DTO {
	class Try : public Command {
	private:
		FunctionBlock* m_try;
		Interface** m_catchKeys;
		std::string* m_names;
		FunctionBlock** m_catchs;
		size_t m_catchLen;
	public:
		Try(FunctionBlock* t_try, Interface** catchKeys, std::string* names, FunctionBlock** catchs, size_t catchLen);
		virtual ~Try() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
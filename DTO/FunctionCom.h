#pragma once
#include "Command.h"
#include "Class.h"

namespace DTO {
	class FunctionCom :public Command {
	private:
		Class* m_class;
		std::wstring m_name;
		Command** m_args;
		size_t m_argsLen;
	public:
		FunctionCom(Class* _class, std::wstring name, Command** args, size_t argsLen);
		virtual ~FunctionCom() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone() override;
	};
}
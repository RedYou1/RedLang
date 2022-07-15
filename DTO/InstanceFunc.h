#pragma once
#include "Command.h"
#include "Class.h"

namespace DTO {
	class InstanceFunc :public Command {
	private:
		Class* m_class;
		Command** m_args;
		size_t m_argsLen;
	public:
		InstanceFunc(Class* _class, Command** args, size_t argsLen);
		virtual ~InstanceFunc()override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone()override;
	};
}
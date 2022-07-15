#pragma once
#include "Command.h"
#include "Type.h"

namespace DTO {
	class ReplaceVar : public Command {
	private:
		Type m_type;
		std::wstring m_name;
		Command* m_cmd;
	public:
		ReplaceVar(Type type, std::wstring name, Command* cmd);
		virtual ~ReplaceVar() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone() override;
	};
}
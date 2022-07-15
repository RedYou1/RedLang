#pragma once
#include <string>
#include "Command.h"
#include "Type.h"

namespace DTO {
	class Declaration : public Command {
	private:
		Type m_type;
		std::wstring m_name;
		Command* m_cmd;
	public:
		Declaration(Type type, std::wstring name, Command* cmd);
		virtual ~Declaration() override;

		CommandReturn* exec(MemoryObject& mem) override;

		Command* clone() override;
	};
}
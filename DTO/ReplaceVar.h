#pragma once
#include "Command.h"
#include "Instanciable.h"

namespace DTO {
	class ReplaceVar : public Command {
	private:
		Instanciable* m_type;
		std::wstring m_name;
		Command* m_cmd;
	public:
		ReplaceVar(Instanciable* type, std::wstring name, Command* cmd);
		virtual ~ReplaceVar() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone() override;
	};
}
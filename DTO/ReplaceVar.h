#pragma once
#include <string>
#include "MemoryObject.h"
#include "Object.h"
#include "Command.h"
#include "Class.h"

namespace DTO {
	class ReplaceVar : public Command {
	private:
		Instanciable* m_type;
		std::wstring m_name;
		Command* m_cmd;
	public:
		ReplaceVar(Instanciable* type, std::wstring name, Command* cmd);
		virtual ~ReplaceVar() override { delete m_cmd; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone() override;
	};
}
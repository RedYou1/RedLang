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
		std::string m_name;
		Command* m_cmd;
	public:
		ReplaceVar(Instanciable* type, std::string name, Command* cmd);
		virtual ~ReplaceVar() override { delete m_cmd; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone() override;
	};
}
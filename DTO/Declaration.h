#pragma once
#include <string>
#include "Command.h"
#include "Class.h"
#include "Object.h"
#include "MemoryObject.h"

namespace DTO {
	class Declaration : public Command {
	private:
		Instanciable* m_type;
		std::wstring m_name;
		Command* m_cmd;
	public:
		Declaration(Instanciable* type, std::wstring name, Command* cmd);
		virtual ~Declaration() override { delete m_cmd; }

		CommandReturn* exec(MemoryObject& mem) override;

		Command* clone() override;
	};
}
#pragma once
#include <string>
#include "Command.h"
#include "Instanciable.h"

namespace DTO {
	class Declaration : public Command {
	private:
		Instanciable* m_type;
		std::wstring m_name;
		Command* m_cmd;
		bool m_nullable;
	public:
		Declaration(Instanciable* type, std::wstring name, Command* cmd, bool nullable);
		virtual ~Declaration() override;

		CommandReturn* exec(MemoryObject& mem) override;

		Command* clone() override;
	};
}
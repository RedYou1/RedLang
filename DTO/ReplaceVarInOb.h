#pragma once

#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Var.h"

namespace DTO {
	class ReplaceVarInOb : public Command {
	private:
		bool m_delete;
		Command* m_ob;
		std::string m_name;
		Command* m_value;
	public:
		ReplaceVarInOb(Command* ob, bool _delete, std::string name, Command* value);
		virtual ~ReplaceVarInOb() override { delete m_ob; delete m_value; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
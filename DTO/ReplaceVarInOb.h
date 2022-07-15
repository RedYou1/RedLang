#pragma once
#include "Command.h"

namespace DTO {
	class ReplaceVarInOb : public Command {
	private:
		bool m_delete;
		Command* m_ob;
		std::wstring m_name;
		Command* m_value;
	public:
		ReplaceVarInOb(Command* ob, bool _delete, std::wstring name, Command* value);
		virtual ~ReplaceVarInOb() override { delete m_ob; delete m_value; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
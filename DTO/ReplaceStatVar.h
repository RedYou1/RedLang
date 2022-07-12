#pragma once
#include <string>
#include "MemoryObject.h"
#include "Object.h"
#include "Command.h"
#include "Class.h"

namespace DTO {
	class ReplaceStatVar : public Command {
	private:
		Class* m_class;
		std::wstring m_name;
		Command* m_value;
	public:
		ReplaceStatVar(Class* _class, std::wstring name, Command* value);
		virtual ~ReplaceStatVar() override { delete m_value; }

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone() override;
	};
}
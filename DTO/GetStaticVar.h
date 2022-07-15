#pragma once
#include "Command.h"
#include "Class.h"

namespace DTO {
	class GetStaticVar :public Command {
	private:
		Class* m_c;
		std::wstring m_name;
	public:
		GetStaticVar(Class* c, std::wstring name);
		virtual ~GetStaticVar() override = default;

		CommandReturn* exec(MemoryObject& mem) override;

		Command* clone()override;
	};
}
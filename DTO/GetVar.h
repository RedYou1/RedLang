#pragma once
#include "Command.h"
#include "Object.h"

namespace DTO {
	class GetVar :public Command {
	private:
		Object* m_o;
		std::wstring m_name;
	public:
		GetVar(Object* o, std::wstring name);
		virtual ~GetVar() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
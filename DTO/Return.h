#pragma once
#include "Command.h"
#include "Instanciable.h"

namespace DTO {
	class Return :public Command {
	private:
		std::wstring m_name;
		Instanciable* m_returnType;
	public:
		Return(std::wstring name, Instanciable* returnType);
		virtual ~Return() override = default;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
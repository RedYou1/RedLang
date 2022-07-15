#pragma once
#include "Command.h"
#include "Instanciable.h"

namespace DTO {
	class Return :public Command {
	private:
		std::wstring m_name;
	public:
		Return(std::wstring name);
		virtual ~Return() override = default;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
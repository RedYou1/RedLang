#pragma once
#include "Command.h"
#include "Instanciable.h"
#include "FunctionBlock.h"

namespace DTO {
	class Try : public Command {
	private:
		FunctionBlock* m_try;
		Instanciable** m_catchKeys;
		std::wstring* m_names;
		FunctionBlock** m_catchs;
		FunctionBlock* m_finally;
		size_t m_catchLen;
	public:
		Try(FunctionBlock* t_try, Instanciable** catchKeys, std::wstring* names, FunctionBlock** catchs, size_t catchLen, FunctionBlock* finally);
		virtual ~Try() override;

		CommandReturn* exec(MemoryObject& mem) override;
		Command* clone()override;
	};
}
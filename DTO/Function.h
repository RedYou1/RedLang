#pragma once
#include <mutex>
#include "IFunction.h"

namespace DTO {
	class Function {
	protected:
		Signature* m_signature;
		IFunction* m_function;
		std::mutex m_lock{};
	public:
		Function(Signature* signature, IFunction* function);
		Function(Signature* signature, Command** cmds, size_t cmdsLen);
		virtual ~Function();

		Signature* getSignature();

		void swap();

		CommandReturn* exec(MemoryObject& pre_mem, Command** args);
		CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size);
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args);
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size);
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args);
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size);

		Function* clone();
	};
}
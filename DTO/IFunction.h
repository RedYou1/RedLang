#pragma once
#include "Signature.h"
#include "Command.h"
#include "MemoryObject.h"
#include "Object.h"

namespace DTO {
	class IFunction {
	protected:
		Signature* m_signature;
	public:
		IFunction(Signature* signature);
		virtual ~IFunction() = default;

		virtual CommandReturn* exec(MemoryObject& pre_mem, Command** args) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, IObject** args) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size) = 0;

		Signature* getSignature();

		virtual IFunction* clone() = 0;
	};
}
#pragma once
#include "PostFunction.h"
#include "Function.h"

namespace DTO {
	class PreFunction :public IFunction {
	public:
		PreFunction(Signature* signature)
			:IFunction(signature) {}

		CommandReturn* exec(MemoryObject& pre_mem, Command** args) override { throw "not converted"; }
		CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size) override { throw "not converted"; }
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args) override { throw "not converted"; }
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size) override { throw "not converted"; }
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args) override { throw "not converted"; }
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size) override { throw "not converted"; }

		IFunction* clone() override { throw "not converted"; }

		virtual PostFunction* convert(Function* func) = 0;
	};
}
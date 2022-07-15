#include "Function.h"
#include "PostFunction.h"
#include "PreFunction.h"

DTO::Function::Function(Signature* signature, IFunction* function)
	:m_signature(signature), m_function(function)
{
}

DTO::Function::Function(Signature* signature, Command** cmds, size_t cmdsLen)
	: m_signature(signature), m_function(new PostFunction(m_signature, cmds, cmdsLen))
{
}

DTO::Function::~Function()
{
	delete m_signature; delete m_function;
}

DTO::Signature* DTO::Function::getSignature()
{
	return m_signature;
}

void DTO::Function::swap()
{
	std::lock_guard<std::mutex>lock(m_lock);
	if (dynamic_cast<PreFunction*>(m_function)) {
		PostFunction* f{ ((PreFunction*)m_function)->convert(this) };
		delete m_function;
		m_function = f;
	}
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, Command** args)
{
	swap();
	return m_function->exec(pre_mem, args);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, Command** args, size_t size)
{
	swap();
	return m_function->exec(pre_mem, args, size);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, IObject** args)
{
	swap();
	return m_function->exec(pre_mem, args);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, IObject** args, size_t size)
{
	swap();
	return m_function->exec(pre_mem, args, size);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, CommandReturn** args)
{
	swap();
	return m_function->exec(pre_mem, args);
}

DTO::CommandReturn* DTO::Function::exec(MemoryObject& pre_mem, CommandReturn** args, size_t size)
{
	swap();
	return m_function->exec(pre_mem, args, size);
}

DTO::Function* DTO::Function::clone()
{
	throw "??";
}

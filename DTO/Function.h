#pragma once
#include <string>
#include <queue>
#include "Class.h"
#include "Command.h"
#include "Object.h"
#include "MemoryObject.h"
#include "MemorySourceFile.h"
#include "MemoryVariable.h"
#include "Signature.h"
#include <mutex>

namespace DTO {
	class IFunction {
	protected:
		Signature* m_signature;
	public:
		IFunction(Signature* signature) :m_signature(signature) {}
		virtual ~IFunction() {}

		virtual CommandReturn* exec(MemoryObject& pre_mem, Command** args) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, IObject** args) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args) = 0;
		virtual CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size) = 0;

		Signature* getSignature() { return m_signature; }

		virtual IFunction* clone() = 0;
	};

	class PostFunction :public IFunction {
	private:
		Command** m_commands;
		size_t m_commandLen;

		CommandReturn* exec(MemoryObject& mem);
	public:
		PostFunction(Signature* signature, Command** commands, size_t commandLen);
		virtual ~PostFunction() override;

		CommandReturn* exec(MemoryObject& pre_mem, Command** args) override;
		CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size) override;
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args) override;
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size) override;
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args) override;
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size) override;

		Command** getCommands() { return m_commands; }
		size_t getcommandLen() { return m_commandLen; }

		IFunction* clone() override;
	};

	class Function {
	protected:
		Signature* m_signature;
		IFunction* m_function;
		std::mutex m_lock{};
	public:
		Function(Signature* signature, IFunction* function) :m_signature(signature), m_function(function) {}
		Function(Signature* signature, Command** cmds, size_t cmdsLen) :m_signature(signature), m_function(new PostFunction(m_signature, cmds, cmdsLen)) {}
		virtual ~Function() { delete m_signature; delete m_function; }

		Signature* getSignature() { return m_signature; }

		void swap();

		CommandReturn* exec(MemoryObject& pre_mem, Command** args);
		CommandReturn* exec(MemoryObject& pre_mem, Command** args, size_t size);
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args);
		CommandReturn* exec(MemoryObject& pre_mem, IObject** args, size_t size);
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args);
		CommandReturn* exec(MemoryObject& pre_mem, CommandReturn** args, size_t size);

		Function* clone();
	};

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

	class FunctionBlock : public Command {
	private:
		Command** m_commands;
		size_t m_commandLen;
	public:
		FunctionBlock(Command** commands, size_t commandLen);
		virtual ~FunctionBlock();

		CommandReturn* exec(MemoryObject& pre_mem) override;
		CommandReturn* exec2(MemoryObject& pre_mem, std::string* name, IObject** args, size_t argsLen);

		Command** getCommands() { return m_commands; }
		size_t getcommandLen() { return m_commandLen; }

		Command* clone() override;
	};

	class FunctionKnownCom :public Command {
	private:
		Function* m_func;
		Command** m_args;
		size_t m_argsLen;
	public:
		FunctionKnownCom(Function* func, Command** args, size_t argsLen);
		virtual ~FunctionKnownCom() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone() override;
	};

	class FunctionCom :public Command {
	private:
		Class* m_class;
		std::string m_name;
		Command** m_args;
		size_t m_argsLen;
	public:
		FunctionCom(Class* _class, std::string name, Command** args, size_t argsLen);
		virtual ~FunctionCom() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone() override;
	};

	class FunctionDynCom :public Command {
	private:
		Command* m_ob;
		std::string m_name;
		Command** m_args;
		size_t m_argsLen;
	public:
		FunctionDynCom(Command* ob, std::string name, Command** args, size_t argsLen);
		virtual ~FunctionDynCom() override;
		CommandReturn* exec(MemoryObject& pre_mem) override;
		Command* clone() override;
	};
}
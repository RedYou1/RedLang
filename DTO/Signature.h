#pragma once
#include <string>
#include "Arg.h"

namespace DTO {
	class Instanciable;

	class Signature {
	private:
		Instanciable* m_returnType;

		std::filesystem::path m_path;
		Arg* m_args;
		size_t m_argsLen;
		bool m_infinite;
	public:
		Signature(std::filesystem::path path, Instanciable* returnType, Arg* args, size_t argsLen);
		Signature(std::filesystem::path path, Instanciable* returnType, Arg* args, size_t argsLen, bool infinity);
		~Signature();

		bool equalsI(Instanciable** argsType, size_t argsLen);
		bool equalsI(Arg* args, size_t argsLen);
		bool similarI(Instanciable** argsType, size_t argsLen);
		bool similar(Instanciable** argsType, size_t argsLen);

		std::filesystem::path getPath() { return m_path; }
		Instanciable* getReturnType() { return m_returnType; }
		Arg* getArgs() { return m_args; }
		size_t getArgsLen() { return m_argsLen; }

		Signature* clone();
	};
}
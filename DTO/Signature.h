#pragma once
#include <string>
#include "Arg.h"

namespace DTO {
	class Interface;

	class Signature {
	private:
		Interface* m_returnType;

		std::string m_path;
		Arg* m_args;
		size_t m_argsLen;
		bool m_infinite;
	public:
		Signature(std::string path, Interface* returnType, Arg* args, size_t argsLen);
		Signature(std::string path, Interface* returnType, Arg* args, size_t argsLen, bool infinity);
		~Signature();

		bool equalsI(Interface** argsType, size_t argsLen);
		bool equalsI(Arg* args, size_t argsLen);
		bool similarI(Interface** argsType, size_t argsLen);
		bool similar(Interface** argsType, size_t argsLen);

		std::string getPath() { return m_path; }
		Interface* getReturnType() { return m_returnType; }
		Arg* getArgs() { return m_args; }
		size_t getArgsLen() { return m_argsLen; }

		Signature* clone();
	};
}
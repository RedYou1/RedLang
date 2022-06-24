#pragma once
#include <string>

namespace DTO {
	class Interface;

	class Signature {
	private:
		Interface* m_returnType;

		std::string m_path;
		Interface** m_argsType;
		std::string* m_argsName;
		size_t m_argsLen;
		bool m_infinite;
	public:
		Signature(std::string path, Interface* returnType, Interface** argsType, std::string* argsName, size_t argsLen);
		Signature(std::string path, Interface* returnType, Interface** argsType, std::string* argsName, size_t argsLen, bool infinity);
		~Signature();

		bool equalsI(Interface** argsType, size_t argsLen);
		bool similarI(Interface** argsType, size_t argsLen);
		bool similar(Interface** argsType, size_t argsLen);

		std::string getPath() { return m_path; }
		Interface* getReturnType() { return m_returnType; }
		Interface** getArgsType() { return m_argsType; }
		std::string* getArgsName() { return m_argsName; }
		size_t getArgsLen() { return m_argsLen; }

		Signature* clone();
	};
}
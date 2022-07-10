#pragma once
#include <map>
#include <string>
#include <list>
#include "Instanciable.h"

namespace DTO {
	class MemorySourceFile;
	class Signature;
	class Class;

	class Interface : public Instanciable {
	protected:
		Interface** m_interfaces;
		size_t m_interfaceLen;
		std::map<std::string, std::list<Signature*>> m_signs;
		MemorySourceFile* m_genTypes;//normaly it contains nothing.
	public:
		Interface(std::string name, std::string path);
		Interface(std::string name, std::string path, MemorySourceFile* genTypes);
		Interface(std::string name, std::string path, Interface** implements, size_t implementsLen);
		Interface(std::string name, std::string path, MemorySourceFile* genTypes, Interface** implements, size_t implementsLen);
		virtual ~Interface() override;

		Interface** getInterfaces() { return m_interfaces; }
		size_t getInterfaceLen() { return m_interfaceLen; }
		std::map<std::string, std::list<Signature*>>* getSignatures() { return &m_signs; }

		void add(std::string name, Signature* o);

		std::list<Signature*> get(std::string name);
		Signature* get(std::string name, Instanciable** argsType, size_t argsLen);

		bool containsAll(Class* _class);

		virtual bool instanceOf(Instanciable* other) override;
	};
}
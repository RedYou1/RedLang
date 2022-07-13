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
		std::map<std::wstring, std::list<Signature*>> m_signs;
		MemorySourceFile* m_genTypes;//normaly it contains nothing.
	public:
		Interface(std::wstring name, std::filesystem::path path);
		Interface(std::wstring name, std::filesystem::path path, MemorySourceFile* genTypes);
		Interface(std::wstring name, std::filesystem::path path, Interface** implements, size_t implementsLen);
		Interface(std::wstring name, std::filesystem::path path, MemorySourceFile* genTypes, Interface** implements, size_t implementsLen);
		virtual ~Interface() override;

		Interface** getInterfaces() { return m_interfaces; }
		size_t getInterfaceLen() { return m_interfaceLen; }
		MemorySourceFile* getGenTypes() { return m_genTypes; }
		std::map<std::wstring, std::list<Signature*>>* getSignatures() { return &m_signs; }

		void add(std::wstring name, Signature* o);

		std::list<Signature*> get(std::wstring name);
		Signature* get(std::wstring name, Instanciable** argsType, size_t argsLen);

		bool containsAll(Class* _class);

		virtual bool instanceOf(Instanciable* other) override;
	};
}
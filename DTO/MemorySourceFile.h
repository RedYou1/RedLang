#pragma once
#include <string>
#include <map>
#include "SourceFile.h"
#include "Interface.h"
#include "Class.h"

namespace DTO {
	class MemorySourceFile {
	private:
		std::map<std::wstring, SourceFile*> m_vars;
		bool m_delete;
	public:
		MemorySourceFile(bool _delete) : m_delete(_delete), m_vars(std::map<std::wstring, SourceFile*>()) {}
		~MemorySourceFile();

		void add(std::wstring name, SourceFile* o);
		void set(std::wstring name, SourceFile* o);

		bool containKey(std::wstring* name, MemorySourceFile* _genTypes = nullptr);

		SourceFile* get(std::wstring name);
		Instanciable* getType(std::wstring name);
		Interface* getInterface(std::wstring name);
		Class* getClass(std::wstring name);

		SourceFile* checkGet(std::wstring name);
		Instanciable* checkGetType(std::wstring name);
		Interface* checkGetInterface(std::wstring name);
		Class* checkGetClass(std::wstring name);

		size_t size();
	};
}
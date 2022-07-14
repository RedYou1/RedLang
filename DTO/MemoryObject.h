#pragma once
#include <string>
#include <map>
#include "Object.h"
#include "Interface.h"

namespace DTO {
	class MemoryObject {
	private:

		class Memory {
		private:
			IObject* m_object;
			Instanciable* m_type;
			bool m_nullable;
		public:
			Memory(IObject* object, Instanciable* type, bool nullable);
			~Memory();

			IObject* getObject();
			Instanciable* getType();
			bool isNullable();

			void setObject(IObject* object);
		};

		MemoryObject* m_parent;
		std::map<std::wstring, Memory*> m_vars;
	public:
		MemoryObject() : m_parent{ nullptr }, m_vars(std::map<std::wstring, Memory*>()) {}
		MemoryObject(MemoryObject* parent);
		~MemoryObject();

		void add(std::wstring name, IObject* object, Instanciable* type, bool nullable);
		void set(std::wstring name, IObject* object);
		bool isNullable(std::wstring name);

		bool containKey(std::wstring name);

		IObject* get(std::wstring name);

		size_t size();
	};
}
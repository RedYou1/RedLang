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
		public:
			Memory(IObject* object, Instanciable* type);
			~Memory();

			IObject* getObject();
			Instanciable* getType();

			void setObject(IObject* object);
		};

		MemoryObject* m_parent;
		std::map<std::wstring, Memory*> m_vars;
	public:
		MemoryObject() : m_parent{ nullptr }, m_vars(std::map<std::wstring, Memory*>()) {}
		MemoryObject(MemoryObject* parent);
		~MemoryObject();

		void add(std::wstring name, IObject* object, Instanciable* type);
		void set(std::wstring name, IObject* object);

		bool containKey(std::wstring name);

		IObject* get(std::wstring name);

		size_t size();
	};
}
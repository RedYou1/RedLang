#pragma once
#include <string>
#include <map>
#include "Object.h"
#include "Type.h"

namespace DTO {
	class MemoryObject {
	private:

		class Memory {
		private:
			IObject* m_object;
			Type m_type;
		public:
			Memory(IObject* object, Type type);
			~Memory();

			IObject* getObject();
			Type getType();

			void setObject(IObject* object);
		};

		MemoryObject* m_parent;
		std::map<std::wstring, Memory*> m_vars;
	public:
		MemoryObject() : m_parent{ nullptr }, m_vars(std::map<std::wstring, Memory*>()) {}
		MemoryObject(MemoryObject* parent);
		~MemoryObject();

		void add(std::wstring name, IObject* object, Type type);
		void set(std::wstring name, IObject* object);
		bool isNullable(std::wstring name);

		bool containKey(std::wstring name);

		IObject* get(std::wstring name);

		size_t size();
	};
}
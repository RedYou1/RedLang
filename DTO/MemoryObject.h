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
			Interface* m_type;
		public:
			Memory(IObject* object, Interface* type);
			~Memory();

			IObject* getObject();
			Interface* getType();

			void setObject(IObject* object);
		};

		MemoryObject* m_parent;
		std::map<std::string, Memory*> m_vars;
	public:
		MemoryObject() : m_parent{ nullptr }, m_vars(std::map<std::string, Memory*>()) {}
		MemoryObject(MemoryObject* parent);
		~MemoryObject();

		void add(std::string name, IObject* object, Interface* type);
		void set(std::string name, IObject* object);

		bool containKey(std::string name);

		IObject* get(std::string name);

		size_t size();
	};
}
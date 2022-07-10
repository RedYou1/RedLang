#pragma once
#include <string>
#include "Object.h"
#include "Command.h"
#include "Generic.h"
#include "Return.h"
#include "Boolean.h"
#include "Long.h"
#include "Global.h"
#include "MemoryFunction.h"
#include "Class.h"
#include "CastException.h"
#include "GarbageCollector.h"
#include "CastException.h"

namespace DTO {
	class List : public GenericStatic {
	private:
		class ListI : public Interface {
		public:
			Instanciable* m_type;
			ListI(std::string name, Instanciable* type);
		};
	public:
		List() : GenericStatic("List", Paths::List, 1) {
		}

		SourceFile* create(std::string newName, Instanciable** gens, size_t genSize)override;
	};
}
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
	class Iterable : public GenericStatic {
	private:
		class IterableI : public Interface {
		public:
			Instanciable* m_type;
			IterableI(std::string name, Instanciable* type);
		};
	public:
		Iterable() : GenericStatic("Iterable", Paths::Iterable, 1) {
		}

		SourceFile* create(std::string newName, Instanciable** gens, size_t genSize)override;
	};
}
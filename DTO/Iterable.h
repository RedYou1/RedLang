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
	class Iterable : public Generic {
	private:
		class IterableI : public Interface {
		public:
			Interface* m_type;
			IterableI(std::string name, Interface* type);
		};
	public:
		Iterable() : Generic("Iterable", Paths::Iterable) {
		}

		SourceFile* create(std::string newName, SourceFile** gens, size_t genSize)override;
	};
}
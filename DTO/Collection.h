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
	class Collection : public GenericStatic {
	private:
		class CollectionI : public Interface {
		public:
			Interface* m_type;
			CollectionI(std::string name, Interface* type);
		};
	public:
		Collection() : GenericStatic("Collection", Paths::Collection, 1) {
		}

		SourceFile* create(std::string newName, Interface** gens, size_t genSize)override;
	};
}
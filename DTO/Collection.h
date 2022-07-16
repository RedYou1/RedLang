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
#include "CastException.h"

namespace DTO {
	class Collection : public GenericStatic {
	private:
		class CollectionI : public Interface {
		public:
			Instanciable* m_type;
			CollectionI(std::wstring name, Instanciable* type);
		};
	public:
		Collection() : GenericStatic(L"Collection", Paths::Collection, 1) {
		}

		SourceFile* create(std::wstring newName, Instanciable** gens, size_t genSize)override;
	};
}
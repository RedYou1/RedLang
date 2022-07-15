#pragma once
#include "Instanciable.h"

namespace DTO {
	struct Type {
		Instanciable* type;
		bool nullable{ false };
	};
}
#pragma once
#include <string>
#include "Interface.h"

namespace DTO {
	struct Arg {
		Instanciable* type = nullptr;
		std::string name = "";
	};
}
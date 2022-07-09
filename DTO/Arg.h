#pragma once
#include <string>
#include "Interface.h"

namespace DTO {
	struct Arg {
		Interface* type = nullptr;
		std::string name = "";
	};
}
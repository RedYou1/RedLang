#pragma once
#include <string>
#include "Type.h"
#include "Command.h"

namespace DTO {
	class Arg {
	public:
		Type type;
		std::wstring name;
		Command* _default{ nullptr };
		~Arg() { delete _default; }
	};
}
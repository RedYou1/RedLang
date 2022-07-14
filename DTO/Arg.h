#pragma once
#include <string>
#include "Interface.h"
#include "Command.h"

namespace DTO {
	class Arg {
	public:
		Instanciable* type;
		std::wstring name;
		Command* _default{ nullptr };
		bool nullable{ false };
		~Arg() { delete _default; }
	};
}
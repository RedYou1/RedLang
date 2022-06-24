#pragma once
#include "Class.h"
#include "Command.h"

namespace DTO {
	class Var {
	public:
		size_t m_index;
		Interface* m_type;
		Command* m_default;

		Var(size_t index, Interface* type, Command* _default) :m_index(index), m_type(type), m_default(_default) {	}
		~Var() { delete m_default; }
	};
}
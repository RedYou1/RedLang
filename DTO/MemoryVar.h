#pragma once
#include <string>
#include <map>

namespace DTO {
	class Var;

	class MemoryVar {
	private:
		MemoryVar* m_parent;

		size_t m_min;
		size_t m_max;

		Var** m_int;
		std::map<std::wstring, Var*> m_string;
	public:
		MemoryVar();
		MemoryVar(MemoryVar* parent);
		~MemoryVar();

		void add(std::wstring name, Var* var);

		Var* get(size_t i);
		Var* get(std::wstring name);

		bool containKey(size_t i);
		bool containKey(std::wstring name);

		size_t min();
		size_t max();
		size_t size();
		size_t thisSize();
	};
}
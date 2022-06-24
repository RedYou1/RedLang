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
		std::map<std::string, Var*> m_string;
	public:
		MemoryVar();
		MemoryVar(MemoryVar* parent);
		~MemoryVar();

		void add(std::string name, Var* var);

		Var* get(size_t i);
		Var* get(std::string name);

		bool containKey(size_t i);
		bool containKey(std::string name);

		size_t min();
		size_t max();
		size_t size();
		size_t thisSize();
	};
}
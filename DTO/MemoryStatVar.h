#pragma once
#include <string>
#include <map>

namespace DTO {
	class StatVar;
	class Object;
	class IObject;

	class MemoryStatVar {
	private:
		std::map<std::string, StatVar*> m_string;
	public:
		MemoryStatVar();
		~MemoryStatVar();

		void add(std::string name, StatVar* var);

		StatVar* get(std::string name);
		void set(std::string name, IObject* value);

		bool containKey(std::string name);

		size_t size();
	};
}
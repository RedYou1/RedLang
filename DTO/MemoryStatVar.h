#pragma once
#include <string>
#include <map>

namespace DTO {
	class StatVar;
	class Object;
	class IObject;

	class MemoryStatVar {
	private:
		std::map<std::wstring, StatVar*> m_string;
	public:
		MemoryStatVar();
		~MemoryStatVar();

		void add(std::wstring name, StatVar* var);

		StatVar* get(std::wstring name);
		void set(std::wstring name, IObject* value);

		bool containKey(std::wstring name);

		size_t size();
	};
}
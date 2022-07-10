#pragma once
#include <string>
#include <map>
#include <queue>
#include <vector>
#include "../DTO/Generic.h"

namespace Parser {
	class GenericDef : public DTO::GenericDynamic {
	private:
		std::string m_type;
		std::string m_content;
		std::vector<std::string> m_genTypes;
		size_t m_genSize;
	public:
		GenericDef(std::string name, std::string path, std::string type, std::queue<std::string> genTypes, std::string content);
		virtual ~GenericDef() override = default;

		DTO::SourceFile* create(std::string newName, DTO::Interface** gens, size_t genSize) override;
	};
}
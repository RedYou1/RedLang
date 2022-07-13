#pragma once
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <filesystem>
#include "../DTO/Generic.h"

namespace Parser {
	class GenericDef : public DTO::GenericStatic {
	private:
		std::wstring m_type;
		std::wstring m_content;
		std::vector<std::wstring> m_genTypes;
	public:
		GenericDef(std::wstring name, std::filesystem::path path, std::wstring type, std::queue<std::wstring> genTypes, std::wstring content);
		virtual ~GenericDef() override = default;

		DTO::SourceFile* create(std::wstring newName, DTO::Instanciable** gens, size_t genSize) override;
	};
}
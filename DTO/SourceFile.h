#pragma once
#include <string>
#include <filesystem>

namespace DTO {
	class SourceFile {
	private:
		std::wstring m_name;
		std::filesystem::path m_path;
	public:
		SourceFile(std::wstring name, std::filesystem::path path);
		virtual ~SourceFile() = default;

		std::wstring getPath() { return m_path; }
		std::wstring getName() { return m_name; }
	};
}
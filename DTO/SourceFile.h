#pragma once
#include <string>

namespace DTO {
	class SourceFile {
	private:
		std::wstring m_name;
		std::wstring	m_path;
	public:
		SourceFile(std::wstring name, std::wstring path);
		virtual ~SourceFile() = default;

		std::wstring getPath() { return m_path; }
		std::wstring getName() { return m_name; }
	};
}
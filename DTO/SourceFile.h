#pragma once
#include <string>

namespace DTO {
	class SourceFile {
	private:
		std::string m_name;
		std::string	m_path;
	public:
		SourceFile(std::string name, std::string path);
		virtual ~SourceFile() = default;

		std::string getPath() { return m_path; }
		std::string getName() { return m_name; }

		static SourceFile* loadFile(std::string path);
	};
}
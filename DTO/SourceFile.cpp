#include "SourceFile.h"

DTO::SourceFile::SourceFile(std::wstring name, std::filesystem::path path)
	:m_name(name), m_path(path)
{
}

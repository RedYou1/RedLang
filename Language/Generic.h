#pragma once
#include <string>
#include <map>
#include <queue>
#include <vector>
#include "SourceFile.h"

class Generic : public SourceFile {
private:
	std::string m_path;
	std::string m_type;
	std::string m_content;
	std::vector<std::string> m_genTypes;
	size_t m_genSize;
	Generic(std::string name, std::string path, std::string type, std::queue<std::string> genTypes, std::string content);
public:
	virtual ~Generic() = default;

	SourceFile* create(std::string newName, SourceFile** gens, size_t genSize);
	static Generic* parse(std::string path, std::string type, std::string name, std::queue<std::string> genTypes, std::string content);
};
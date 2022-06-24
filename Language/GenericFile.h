#pragma once
#include <string>
#include <map>
#include <queue>
#include <vector>
#include "Generic.h"

class GenericFile : public Generic {
private:
	std::string m_type;
	std::string m_content;
	std::vector<std::string> m_genTypes;
	size_t m_genSize;
	GenericFile(std::string name, std::string path, std::string type, std::queue<std::string> genTypes, std::string content);
public:
	virtual ~GenericFile() override = default;

	SourceFile* create(std::string newName, SourceFile** gens, size_t genSize) override;
	static GenericFile* parse(std::string path, std::string type, std::string name, std::queue<std::string> genTypes, std::string content);
};
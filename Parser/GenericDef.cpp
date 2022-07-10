#include "GenericDef.h"
#include "Parser.h"
#include "../DTO/myString.h"
#include "../DTO/MemorySourceFile.h"
#include "../DTO/Class.h"
#include "../DTO/Global.h"

Parser::GenericDef::GenericDef(std::string name, std::string path, std::string type, std::queue<std::string> genTypes, std::string content)
	:DTO::GenericDynamic(name, path),
	m_type(type),
	m_genSize(genTypes.size()),
	m_genTypes(),
	m_content(content)
{
	m_genTypes.resize(m_genSize);
	for (size_t i{ 0 }; i < m_genSize; i++)
	{
		m_genTypes[i] = genTypes.front();
		genTypes.pop();
	}
}

DTO::SourceFile* Parser::GenericDef::create(std::string newName, DTO::Interface** gens, size_t genSize)
{
	if (genSize != m_genSize)
		throw "??";

	DTO::MemorySourceFile* genTypes{ new DTO::MemorySourceFile{false} };
	for (size_t i{ 0 }; i < m_genSize; i++) {
		genTypes->add(m_genTypes[i], gens[i]);
	}

	std::string str{ newName + m_content };

	SourceFile* r{ nullptr };
	if (m_type == "class") {
		r = Parser::parseClass(getPath(), str, *genTypes);
	}
	else if (m_type == "interface") {
		r = Parser::parseInterface(getPath(), str, *genTypes);
	}
	else {
		throw "??";
	}
	if (r == nullptr)
		throw "??";

	DTO::GenPossibility* poss{ new DTO::GenPossibility[genSize] };

	for (size_t i{ 0 }; i < genSize; i++) {
		poss[i] = DTO::GenPossibility{ gens[i] };
	}

	add(poss, genSize, r);

	return r;
}

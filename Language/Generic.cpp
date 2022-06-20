#include "Generic.h"
#include "myString.h"
#include "MemorySourceFile.h"
#include "Class.h"
#include "Global.h"

Generic::Generic(std::string name, std::string path, std::string type, std::queue<std::string> genTypes, std::string content)
	:SourceFile(name, path),
	m_path(path),
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

SourceFile* Generic::create(std::string newName, SourceFile** gens, size_t genSize)
{
	if (genSize != m_genSize)
		throw "??";

	MemorySourceFile* genTypes{ new MemorySourceFile{false} };
	for (size_t i{ 0 }; i < m_genSize; i++) {
		genTypes->add(m_genTypes[i], gens[i]);
	}

	std::string str{ newName + m_content };

	SourceFile* r{ nullptr };
	if (m_type == "class") {
		r = Class::parse(m_path, str, *genTypes);
	}
	else if (m_type == "interface") {
		r = Interface::parse(m_path, str, *genTypes);
	}
	else {
		throw "??";
	}
	if (r == nullptr)
		throw "??";

	return r;
}

Generic* Generic::parse(std::string path, std::string type, std::string name, std::queue<std::string> genTypes, std::string content)
{
	Generic* g{ new Generic(name,path, type, genTypes, content) };
	GLOBAL::getClasses()->set(name, g);
	return g;
}

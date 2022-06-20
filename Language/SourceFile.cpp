#include <fstream>
#include "SourceFile.h"
#include "myString.h"
#include "Global.h"
#include "Generic.h"

SourceFile::SourceFile(std::string name, std::string path)
	:m_name(name), m_path(path)
{
}

SourceFile* SourceFile::loadFile(std::string filePath) {
	std::ifstream file(filePath);
	std::string tempPath{ filePath };
	std::queue<std::string> pathqueue{ myString{ &tempPath }.split("\\") };
	std::string path{ pathqueue.front() };
	pathqueue.pop();
	while (pathqueue.size() > 1) {
		path += '\\' + pathqueue.front();
		pathqueue.pop();
	}
	if (file.is_open())
	{
		{
			std::string p{ filePath };
			p = myString{ &p }.split("\\").back();
			p = myString{ &p }.extract(".");
			if (GLOBAL::getClasses()->containKey(p)) {
				file.close();
				return GLOBAL::getClasses()->get(p);
			}
			else {
				GLOBAL::getClasses()->add(p, nullptr);
			}
		}
		std::string line;
		std::string content{ "" };
		while (std::getline(file, line)) {
			content += line;
		}
		file.close();

		myString s{ &content };
		std::string instru{ "" };
		do {
			s.removeUseless();
			instru = s.extract(" ");
			if (instru == "include") {
				s.removeUseless();
				s.extract(1);
				std::string v{ path + '\\' };
				char temp;
				while ((temp = s.extract(1).at(0)) != '\"') {
					if (temp == '\\') {
						v += myString::backSlachChar(s.extract(1).at(0));
					}
					else {
						v += temp;
					}
				}
				loadFile(v);
			}
		} while (instru != "class" && instru != "interface");

		s.removeUseless();
		std::string name{ s.extract2() };
		s.removeUseless();
		if (content.at(0) == '<') {
			std::queue<std::string> q{ myString(&s.extractFunc2()).split2(',') };
			return Generic::parse(path, instru, name, q, content);
		}
		else {
			MemorySourceFile* g{ new MemorySourceFile {false} };
			if (instru == "class") {
				return Class::parse(path, name + ' ' + content, *g);
			}
			else {
				return Interface::parse(path, name + ' ' + content, *g);
			}
		}
	}
	else throw "Unable to open file";
}

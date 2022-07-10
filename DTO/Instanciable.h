#pragma once
#include "SourceFile.h"

namespace DTO {
	class Instanciable : public SourceFile {
	public:
		Instanciable(std::string name, std::string path) :SourceFile(name, path) {}
		virtual bool instanceOf(Instanciable* other) = 0;
	};
}
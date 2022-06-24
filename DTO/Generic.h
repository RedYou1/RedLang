#pragma once
#include <string>
#include "SourceFile.h"

namespace DTO {
	class Generic : public SourceFile {
	public:
		Generic(std::string name, std::string path) :SourceFile(name, path) {}
		virtual ~Generic() override = default;
		virtual SourceFile* create(std::string newName, SourceFile** gens, size_t genSize) = 0;
	};
}
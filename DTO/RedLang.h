#pragma once
#include "SourceFile.h"

namespace DTO {
	class RedLang {
	public:
		static void importRedLang(SourceFile* (*parser)(std::string));
	};
}
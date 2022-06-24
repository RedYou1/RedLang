#include "Global.h"

DTO::MemorySourceFile* DTO::GLOBAL::mem{ new MemorySourceFile{true} };
DTO::MemorySourceFile* DTO::GLOBAL::getClasses() {
	return mem;
}

void DTO::GLOBAL::Delete()
{
	delete mem;
}

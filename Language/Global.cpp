#include "Global.h"

MemorySourceFile* GLOBAL::mem{ new MemorySourceFile{true} };
MemorySourceFile* GLOBAL::getClasses() {
	return mem;
}

void GLOBAL::Delete()
{
	delete mem;
}

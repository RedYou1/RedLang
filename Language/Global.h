#pragma once
#include "MemorySourceFile.h"

class GLOBAL {
private:
	static MemorySourceFile* mem;
public:
	static MemorySourceFile* getClasses();
	static void Delete();
};

namespace Paths {
	constexpr char Function[] = "RedLang.Function";
	constexpr char Class[] = "RedLang.Class";
	constexpr char Object[] = "RedLang.Object";
	constexpr char Number[] = "RedLang.Numbers.Number";
	constexpr char Byte[] = "RedLang.Numbers.Byte";
	constexpr char Short[] = "RedLang.Numbers.Short";
	constexpr char Interger[] = "RedLang.Numbers.Integer";
	constexpr char Long[] = "RedLang.Numbers.Long";
	constexpr char Float[] = "RedLang.Numbers.Float";
	constexpr char Double[] = "RedLang.Numbers.Double";
	constexpr char Char[] = "RedLang.Char";
	constexpr char String[] = "RedLang.String";
	constexpr char Boolean[] = "RedLang.Boolean";
	constexpr char Lock[] = "RedLang.Threading.Lock";
	constexpr char Thread[] = "RedLang.Threading.Thread";
	constexpr char System[] = "RedLang.System";
	constexpr char Exception[] = "RedLang.Exceptions.Exception";
	constexpr char CastException[] = "RedLang.Exceptions.CastException";
	constexpr char IllegalArgumentException[] = "RedLang.Exceptions.IllegalArgumentException";
	constexpr char NullException[] = "RedLang.Exceptions.NullException";
}
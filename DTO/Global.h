#pragma once
#include "MemorySourceFile.h"

namespace DTO {
	class GLOBAL {
	private:
		static MemorySourceFile* mem;
	public:
		static MemorySourceFile* getClasses();
		static void Delete();
	};

	namespace Paths {
		constexpr wchar_t Function[] = L"RedLang.Function";
		constexpr wchar_t Class[] = L"RedLang.Class";
		constexpr wchar_t Object[] = L"RedLang.Object";
		constexpr wchar_t Char[] = L"RedLang.Char";
		constexpr wchar_t String[] = L"RedLang.String";
		constexpr wchar_t Boolean[] = L"RedLang.Boolean";
		constexpr wchar_t Lock[] = L"RedLang.Threading.Lock";
		constexpr wchar_t Thread[] = L"RedLang.Threading.Thread";
		constexpr wchar_t System[] = L"RedLang.System";
		constexpr wchar_t File[] = L"RedLang.File";

		constexpr wchar_t Number[] = L"RedLang.Numbers.Number";
		constexpr wchar_t Byte[] = L"RedLang.Numbers.Byte";
		constexpr wchar_t Short[] = L"RedLang.Numbers.Short";
		constexpr wchar_t Integer[] = L"RedLang.Numbers.Integer";
		constexpr wchar_t Long[] = L"RedLang.Numbers.Long";
		constexpr wchar_t Float[] = L"RedLang.Numbers.Float";
		constexpr wchar_t Double[] = L"RedLang.Numbers.Double";

		constexpr wchar_t Exception[] = L"RedLang.Exceptions.Exception";
		constexpr wchar_t CastException[] = L"RedLang.Exceptions.CastException";
		constexpr wchar_t IllegalArgumentException[] = L"RedLang.Exceptions.IllegalArgumentException";
		constexpr wchar_t NullException[] = L"RedLang.Exceptions.NullException";

		constexpr wchar_t Iterable[] = L"RedLang.Iterable";
		constexpr wchar_t Collection[] = L"RedLang.Iterable.Collection";
		constexpr wchar_t List[] = L"RedLang.Iterable.Collection.List";
		constexpr wchar_t Array[] = L"RedLang.Iterable.Array";
		constexpr wchar_t ArrayList[] = L"RedLang.Iterable.ArrayList";
	}
}
#pragma once
#include <string>
#include <queue>

namespace DTO {
	class myString {
	private:
		static constexpr const wchar_t stackable[12]{ '(',')','{','}','[',']','<','>','\"','\"','\'','\'' };
		static constexpr const int stackableLen{ 12 };

		static constexpr const wchar_t end[8]{ ' ', '.', '{', '(', '<', ';','=',',' };
		static constexpr const int endLen{ 8 };

		std::wstring* m_str;

		int isInStack(wchar_t b);

		static bool isUseless(wchar_t c);
	public:
		myString(std::wstring* str);

		bool startWith(std::wstring s);
		bool endWith(std::wstring s);

		std::wstring extract(size_t amount);

		std::wstring extract(std::wstring until);

		std::wstring extract(std::queue<std::wstring> untils);

		std::wstring extract2();

		std::wstring extractName();

		std::wstring extractFunc();
		std::wstring extractFunc2();

		std::queue<std::wstring> split(std::wstring s);
		std::queue<std::wstring> split2(wchar_t split);

		void removeUseless();

		static wchar_t backSlachChar(wchar_t c);

		static bool is_number(const std::wstring& s)
		{
			if (s.size() <= 1)
				return false;

			if (s.back() != L'b' && s.back() != L's' && s.back() != L'i' && s.back() != L'f' && s.back() != L'l' && s.back() != L'd')
				return false;

			bool dot{ s.back() == L'f' || s.back() == L'd' };

			for (std::wstring::const_iterator it{ s.begin() + (s.at(0) == L'-' ? 1 : 0) }; it != s.end() - 1; ++it) {
				if (!std::isdigit(*it))
					if (*it == L'.' && dot) {
						dot = false;
					}
					else
						return false;
			}
			return true;
		}
	};
}
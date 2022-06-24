#pragma once
#include <string>
#include <queue>

namespace DTO {
	class myString {
	private:
		static constexpr const char stackable[12]{ '(',')','{','}','[',']','<','>','\"','\"','\'','\'' };
		static constexpr const int stackableLen{ 12 };

		static constexpr const char end[8]{ ' ', '.', '{', '(', '<', ';','=',',' };
		static constexpr const int endLen{ 8 };

		std::string* m_str;

		int isInStack(char b);

		static bool isUseless(char c);
	public:
		myString(std::string* str);

		bool startWith(std::string s);
		bool endWith(std::string s);

		std::string extract(size_t amount);

		std::string extract(std::string until);

		std::string extract(std::queue<std::string> untils);

		std::string extract2();

		std::string extractName();

		std::string extractFunc();
		std::string extractFunc2();

		std::queue<std::string> split(std::string s);
		std::queue<std::string> split2(char split);

		void removeUseless();

		static char backSlachChar(char c);

		static bool is_number(const std::string& s)
		{
			if (s.size() <= 1)
				return false;

			if (s.back() != 'b' && s.back() != 's' && s.back() != 'i' && s.back() != 'f' && s.back() != 'l' && s.back() != 'd')
				return false;

			bool dot{ s.back() == 'f' || s.back() == 'd' };

			for (std::string::const_iterator it{ s.begin() + (s.at(0) == '-' ? 1 : 0) }; it != s.end() - 1; ++it) {
				if (!std::isdigit(*it))
					if (*it == '.' && dot) {
						dot = false;
					}
					else
						return false;
			}
			return true;
		}
	};
}
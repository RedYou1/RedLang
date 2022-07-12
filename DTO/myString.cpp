#include <stack>
#include "myString.h"

DTO::myString::myString(std::wstring* str) :m_str{ str } {}

bool DTO::myString::startWith(std::wstring s)
{
	return m_str->find(s) == 0;
}

bool DTO::myString::endWith(std::wstring s)
{
	if (m_str->length() < s.length())
		return false;
	return (0 == m_str->compare(m_str->length() - s.length(), s.length(), s));
}

std::wstring DTO::myString::extract2() {
	size_t a{ m_str->size() };
	for (int c(0); c < endLen; c++) {
		size_t t{ m_str->find(end[c]) };
		if (a > t && (end[c] != L'.' || !std::isdigit(m_str->at(t + 1))))
			a = t;
	}
	return extract(a);
}

std::wstring DTO::myString::extractName() {
	size_t a{ m_str->size() };
	for (int c(0); c < endLen; c++) {
		if (end[c] == L',' || end[c] == L'<')
			continue;
		size_t t{ m_str->find(end[c]) };
		if (a > t && (end[c] != L'.' || !std::isdigit(m_str->at(t + 1))))
			a = t;
	}
	return extract(a);
}

std::wstring DTO::myString::extract(size_t amount) {
	std::wstring n{ m_str->substr(0, amount) };
	m_str->erase(0, amount);
	return n;
}

std::wstring DTO::myString::extract(std::wstring until) {
	return extract(m_str->find(until));
}

std::wstring DTO::myString::extract(std::queue<std::wstring> untils) {
	size_t a{ m_str->size() };
	while (!untils.empty()) {
		size_t t{ m_str->find(untils.front()) };
		if (a > t)
			a = t;
		untils.pop();
	}
	return extract(a);
}

int DTO::myString::isInStack(wchar_t b) {
	for (int c(0); c < stackableLen; c++)
		if (stackable[c] == b)
			return c;
	return -1;
}

std::wstring DTO::myString::extractFunc() {
	if (isInStack(m_str->at(0)) == -1)
		throw "need to start with a stack";

	std::stack<wchar_t> a{};
	a.push(extract(1).at(0));
	int ib{ isInStack(a.top()) };
	std::wstring b{ L"" };
	while (!m_str->empty()) {
		wchar_t c{ extract(1).at(0) };
		int ia{ isInStack(c) };


		if (ia > -1) {
			if ((ia == ib && (stackable[ia] == L'"' || stackable[ia] == L'\'')) || (ib > -1 && ia - 1 == ib)) {
				a.pop();
				if (a.empty())
					break;
				ib = isInStack(a.top());
			}
			else if (ia % 2 == 0 && (!(a.top() == L'"' || a.top() == L'\'') || (c == L'"' || c == L'\''))) {
				a.push(c);
				ib = isInStack(a.top());
			}
			else if (a.top() != L'"' && a.top() != L'\'') {
				throw "none stackable";
			}
		}
		b += c;
	}
	return b;
}

std::wstring DTO::myString::extractFunc2() {
	if (isInStack(m_str->at(0)) == -1)
		throw "need to start with a stack";

	std::stack<wchar_t> a{};
	a.push(extract(1).at(0));
	int ib{ isInStack(a.top()) };
	std::wstring b{ a.top() };
	while (!m_str->empty()) {
		wchar_t c{ extract(1).at(0) };
		int ia{ isInStack(c) };


		if (ia > -1) {
			if ((ia == ib && (stackable[ia] == L'"' || stackable[ia] == L'\'')) || (ib > -1 && ia - 1 == ib)) {
				a.pop();
				if (a.empty()) {
					b += c;
					break;
				}
				ib = isInStack(a.top());
			}
			else if (ia % 2 == 0 && (!(a.top() == L'"' || a.top() == L'\'') || (c == L'"' || c == L'\''))) {
				a.push(c);
				ib = isInStack(a.top());
			}
			else if (a.top() != L'"' && a.top() != L'\'') {
				throw "none stackable";
			}
		}
		b += c;
	}
	return b;
}

std::queue<std::wstring> DTO::myString::split2(wchar_t split) {
	if (isInStack(m_str->at(0)) == -1)
		throw "need to start with a stack";
	int last{ 0 };
	std::wstring a{ *m_str };
	std::queue<std::wstring> result{};
	std::stack<wchar_t> stack{};
	stack.push(extract(1).at(0));
	int ib{ isInStack(stack.top()) };
	std::wstring temp{ L"" };
	while (!m_str->empty()) {
		wchar_t c{ extract(1).at(0) };
		int ia{ isInStack(c) };

		if ((c != L'\'' && stack.top() == L'\'') || (c != L'"' && stack.top() == L'"')) {
			temp += c;
			continue;
		}

		if ((c == L'\'' && stack.top() == L'\'') || (c == L'"' && stack.top() == L'"')) {
			temp += c;
			stack.pop();
			if (stack.empty())
				break;
			ib = isInStack(stack.top());
			continue;
		}

		if (stack.size() == 1 && c == split) {
			result.push(temp);
			temp = L"";
			continue;
		}

		if (ia > -1) {
			if (ib > -1 && ia - 1 == ib) {
				stack.pop();
				if (stack.empty())
					break;
				ib = isInStack(stack.top());
				if (stack.size() == 1 && c == L'}') {
					result.push(temp + c);
					temp = L"";
					continue;
				}
			}
			else if (ia % 2 == 0 && (!(stack.top() == L'"' || stack.top() == L'\'') || (c == L'"' || c == L'\''))) {
				stack.push(c);
				ib = isInStack(stack.top());
			}
			else {
				throw "none stackable";
			}
		}
		temp += c;
	}
	if (!result.empty() || temp != L"")
		result.push(temp);
	return result;
}

std::queue<std::wstring> DTO::myString::split(std::wstring s) {
	int last{ 0 };
	std::wstring a{ *m_str };
	std::queue<std::wstring> result{};
	while (a.size() > 0) {
		size_t f{ a.find(s) };
		if (f == std::wstring::npos) {
			result.push(a);
			break;
		}
		else {
			myString t{ &a };
			std::wstring b{ t.extract(f) };
			t.extract(s.size());
			result.push(b);
		}
	}
	return result;
}

wchar_t DTO::myString::backSlachChar(wchar_t c)
{
	switch (c) {
	case L'\\':
		return L'\\';
	case L'\"':
		return L'\"';
	case L'\'':
		return L'\'';
	case L'a':
		return L'\a';
	case L'b':
		return L'\b';
	case L't':
		return L'\t';
	case L'n':
		return L'\n';
	case L'v':
		return L'\v';
	case L'f':
		return L'\f';
	case L'r':
		return L'\r';
	default:
		return c;
	}
}


bool DTO::myString::isUseless(wchar_t c) {
	return c == L' ' || c == L'\r' || c == L'\n' || c == L'	';
}

void DTO::myString::removeUseless() {
	while (!m_str->empty() && isUseless(m_str->at(0))) {
		extract(1);
	}
}
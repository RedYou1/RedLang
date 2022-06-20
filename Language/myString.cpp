#include <stack>
#include "myString.h"

myString::myString(std::string* str) :m_str{ str } {}

bool myString::startWith(std::string s)
{
	return m_str->find(s) == 0;
}

bool myString::endWith(std::string s)
{
	if (m_str->length() < s.length())
		return false;
	return (0 == m_str->compare(m_str->length() - s.length(), s.length(), s));
}

std::string myString::extract2() {
	size_t a{ m_str->size() };
	for (int c(0); c < endLen; c++) {
		size_t t{ m_str->find(end[c]) };
		if (a > t && (end[c] != '.' || !std::isdigit(m_str->at(t + 1))))
			a = t;
	}
	return extract(a);
}

std::string myString::extractName() {
	size_t a{ m_str->size() };
	for (int c(0); c < endLen; c++) {
		if (end[c] == ',' || end[c] == '<')
			continue;
		size_t t{ m_str->find(end[c]) };
		if (a > t && (end[c] != '.' || !std::isdigit(m_str->at(t + 1))))
			a = t;
	}
	return extract(a);
}

std::string myString::extract(size_t amount) {
	std::string n{ m_str->substr(0, amount) };
	m_str->erase(0, amount);
	return n;
}

std::string myString::extract(std::string until) {
	return extract(m_str->find(until));
}

std::string myString::extract(std::queue<std::string> untils) {
	size_t a{ m_str->size() };
	while (!untils.empty()) {
		size_t t{ m_str->find(untils.front()) };
		if (a > t)
			a = t;
		untils.pop();
	}
	return extract(a);
}

int myString::isInStack(char b) {
	for (int c(0); c < stackableLen; c++)
		if (stackable[c] == b)
			return c;
	return -1;
}

std::string myString::extractFunc() {
	if (isInStack(m_str->at(0)) == -1)
		throw "need to start with a stack";

	std::stack<char> a{};
	a.push(extract(1).at(0));
	int ib{ isInStack(a.top()) };
	std::string b{ "" };
	while (!m_str->empty()) {
		char c{ extract(1).at(0) };
		int ia{ isInStack(c) };


		if (ia > -1) {
			if ((ia == ib && (stackable[ia] == '"' || stackable[ia] == '\'')) || (ib > -1 && ia - 1 == ib)) {
				a.pop();
				if (a.empty())
					break;
				ib = isInStack(a.top());
			}
			else if (ia % 2 == 0 && (!(a.top() == '"' || a.top() == '\'') || (c == '"' || c == '\''))) {
				a.push(c);
				ib = isInStack(a.top());
			}
			else if (a.top() != '"' && a.top() != '\'') {
				throw "none stackable";
			}
		}
		b += c;
	}
	return b;
}

std::string myString::extractFunc2() {
	if (isInStack(m_str->at(0)) == -1)
		throw "need to start with a stack";

	std::stack<char> a{};
	a.push(extract(1).at(0));
	int ib{ isInStack(a.top()) };
	std::string b{ a.top() };
	while (!m_str->empty()) {
		char c{ extract(1).at(0) };
		int ia{ isInStack(c) };


		if (ia > -1) {
			if ((ia == ib && (stackable[ia] == '"' || stackable[ia] == '\'')) || (ib > -1 && ia - 1 == ib)) {
				a.pop();
				if (a.empty()) {
					b += c;
					break;
				}
				ib = isInStack(a.top());
			}
			else if (ia % 2 == 0 && (!(a.top() == '"' || a.top() == '\'') || (c == '"' || c == '\''))) {
				a.push(c);
				ib = isInStack(a.top());
			}
			else if (a.top() != '"' && a.top() != '\'') {
				throw "none stackable";
			}
		}
		b += c;
	}
	return b;
}

std::queue<std::string> myString::split2(char split) {
	if (isInStack(m_str->at(0)) == -1)
		throw "need to start with a stack";
	int last{ 0 };
	std::string a{ *m_str };
	std::queue<std::string> result{};
	std::stack<char> stack{};
	stack.push(extract(1).at(0));
	int ib{ isInStack(stack.top()) };
	std::string temp{ "" };
	while (!m_str->empty()) {
		char c{ extract(1).at(0) };
		int ia{ isInStack(c) };

		if ((c != '\'' && stack.top() == '\'') || (c != '"' && stack.top() == '"')) {
			temp += c;
			continue;
		}

		if ((c == '\'' && stack.top() == '\'') || (c == '"' && stack.top() == '"')) {
			temp += c;
			stack.pop();
			if (stack.empty())
				break;
			ib = isInStack(stack.top());
			continue;
		}

		if (stack.size() == 1 && c == split) {
			result.push(temp);
			temp = "";
			continue;
		}

		if (ia > -1) {
			if (ib > -1 && ia - 1 == ib) {
				stack.pop();
				if (stack.empty())
					break;
				ib = isInStack(stack.top());
				if (stack.size() == 1 && c == '}') {
					result.push(temp + c);
					temp = "";
					continue;
				}
			}
			else if (ia % 2 == 0 && (!(stack.top() == '"' || stack.top() == '\'') || (c == '"' || c == '\''))) {
				stack.push(c);
				ib = isInStack(stack.top());
			}
			else {
				throw "none stackable";
			}
		}
		temp += c;
	}
	if (!result.empty() || temp != "")
		result.push(temp);
	return result;
}

std::queue<std::string> myString::split(std::string s) {
	int last{ 0 };
	std::string a{ *m_str };
	std::queue<std::string> result{};
	while (a.size() > 0) {
		size_t f{ a.find(s) };
		if (f == std::string::npos) {
			result.push(a);
			break;
		}
		else {
			myString t{ &a };
			std::string b{ t.extract(f) };
			t.extract(s.size());
			result.push(b);
		}
	}
	return result;
}

char myString::backSlachChar(char c)
{
	switch (c) {
	case '\\':
		return '\\';
	case '\"':
		return '\"';
	case '\'':
		return '\'';
	case 'a':
		return '\a';
	case 'b':
		return '\b';
	case 't':
		return '\t';
	case 'n':
		return '\n';
	case 'v':
		return '\v';
	case 'f':
		return '\f';
	case 'r':
		return '\r';
	default:
		return c;
	}
}


bool myString::isUseless(char c) {
	return c == ' ' || c == '\r' || c == '\n' || c == '	';
}

void myString::removeUseless() {
	while (!m_str->empty() && isUseless(m_str->at(0))) {
		extract(1);
	}
}
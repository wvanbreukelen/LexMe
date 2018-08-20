#pragma once

#include <string>

struct Literal {
	std::string text;

	Literal(const char* text) : text(text) {};
	Literal(const std::string& text) : text(text) {};
	Literal() {};
};
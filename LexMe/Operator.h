#pragma once

#include <string>

struct Operator {
	std::string text;

	Operator(const char* text) : text(text) {};
	Operator(const std::string& text) : text(text) {};
	Operator() {};
};
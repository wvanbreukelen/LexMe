#pragma once

#include <string>

class Operator {
public:
	std::string text;

	Operator(const char* text) : text(text) {};
	Operator(const std::string& text) : text(text) {};
	Operator() {};
};
#pragma once

#include <string>

class Operator {
public:
	std::string text;
	int percedence = 0;

	Operator(const char* text, int percendence) : text(text), percedence(percendence) {};
	Operator(const std::string& text, int percendence) : text(text), percedence(percendence) {};
	Operator() {};
};
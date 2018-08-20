#include "Lexer.h"


Lexer::Lexer()
{
}

TokenList Lexer::process(const std::string &text) {
	return lexerAnalysis.lexString(text);
}

TokenList Lexer::process(const char* text) {
	return lexerAnalysis.lexString(text);
}

Lexer::~Lexer()
{
}

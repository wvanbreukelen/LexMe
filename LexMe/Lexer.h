#pragma once

#include "TokenList.h"
#include "LexeralAnalysis.h"
#include "LanguageDefinition.h"

class Lexer
{
private:
	LexeralAnalysis lexerAnalysis;

public:
	Lexer();
	
	TokenList process(const std::string &text);
	TokenList process(const char* text);

	~Lexer();
};


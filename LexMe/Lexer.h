#pragma once

#include "TokenList.h"
#include "LexeralAnalysis.h"
#include "LanguageDefinition.h"

class Lexer
{
public:
	Lexer();
	
	TokenList process(const char* text);

	~Lexer();
};


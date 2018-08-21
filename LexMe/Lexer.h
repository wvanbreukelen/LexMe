#pragma once

#include "TokenList.h"
#include "LexeralAnalysis.h"
#include "LanguageDefinition.h"
#include <iostream>
#include <fstream>
#include <string>

class Lexer
{
private:
	LexeralAnalysis lexerAnalysis;

public:
	Lexer();
	
	TokenList process(const std::string &text);
	TokenList process(const char* text);
	TokenList processFile(const char* filename);
	TokenList processFile(std::string filename);

	~Lexer();
};


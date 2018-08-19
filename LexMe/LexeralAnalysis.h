#pragma once

#include <string>
#include "LanguageDefinition.h"
#include "TokenList.h"

class LexeralAnalysis
{
public:
	LexeralAnalysis();
	~LexeralAnalysis();

	TokenList processString(std::string str);
};


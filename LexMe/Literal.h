#pragma once

#include "TypeDefinition.h"

using LanguageDefinition::TokenType;

class Literal {
public:
	const char *text = "";
	TokenType tokenType = TokenType::UNKNOWN;

	Literal(const char* text, const TokenType tokenType) : text(text), tokenType(tokenType) {};
	Literal() {};
};


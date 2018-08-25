#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "LanguageDefinition.h"
#include "TokenList.h"
#include "LexicalException.h"

class LexeralAnalysis
{
private:
	typedef LanguageDefinition::TokenType TokenType;
	typedef LanguageDefinition::CharacterType CharacterType;
	typedef Operator Operator;

	LanguageDefinition::CharacterType classifyCharacter(const std::string& str, const unsigned int chIndex);
	LanguageDefinition::TokenType resolveTokenType(LanguageDefinition::CharacterType charType, LanguageDefinition::TokenType prevTokenType);
	TokenType classifyLiteralNumber(const std::string &tokenValue);
	std::vector<Operator> findOperatorMatches(const TokenType prevTokenType, const std::string &tokenValue, const int linePos, const int charPos);

	// Hashmap with character classifier, currently stored in RAM, but may be stored in ROM for performance dependend devices.
	std::unordered_map<uint8_t, LanguageDefinition::CharacterType> charClassifiers;

public:
	LexeralAnalysis();

	TokenList lexString(const std::string& str);
	TokenList lexString(const char* str);
};


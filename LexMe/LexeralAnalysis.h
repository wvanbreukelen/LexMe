#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "LanguageDefinition.h"
#include "TokenList.h"

class LexeralAnalysis
{
private:
	typedef LanguageDefinition::TokenType TokenType;
	typedef LanguageDefinition::CharacterType CharacterType;
	typedef LanguageDefinition::Operator Operator;

	LanguageDefinition::CharacterType classifyCharacter(const uint8_t strEntry);
	LanguageDefinition::TokenType resolveTokenType(LanguageDefinition::CharacterType charType, LanguageDefinition::TokenType prevTokenType);
	//LanguageDefinition::OperatorType classifyOperator(const uint8_t strEntry);

	// Hashmap with character classifier, currently stored in RAM, but may be stored in ROM for performance dependend devices.
	std::unordered_map<uint8_t, LanguageDefinition::CharacterType> charClassifiers;

public:
	LexeralAnalysis();

	TokenList lexString(const std::string& str);
	TokenList lexString(const char* str);
};


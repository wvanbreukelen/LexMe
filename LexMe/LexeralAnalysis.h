#pragma once

#include <string>
#include <unordered_map>
#include "LanguageDefinition.h"
#include "TokenList.h"

class LexeralAnalysis
{
private:
	LanguageDefinition::CharacterType classifyCharacter(const uint8_t strEntry);
	LanguageDefinition::OperatorType classifyOperator(const uint8_t strEntry);

	// Hashmap with character classifier, currently stored in RAM, but may be stored in ROM for performance dependend devices.
	std::unordered_map<uint8_t, LanguageDefinition::CharacterType> charClassifiers;

public:
	LexeralAnalysis();
	~LexeralAnalysis();

	TokenList lexString(const std::string& ch);
};


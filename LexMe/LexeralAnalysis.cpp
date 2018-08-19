#include "stdafx.h"
#include "LexeralAnalysis.h"

LexeralAnalysis::LexeralAnalysis() {
	// Create our classifier map.
	// First, do all alpha characters.
	for (uint8_t c = 'a'; c <= 'z'; c++) {
		charClassifiers[c] = LanguageDefinition::CharacterType::LETTER;
	}

	for (uint8_t c = 'A'; c <= 'Z'; c++) {
		charClassifiers[c] = LanguageDefinition::CharacterType::LETTER;
	}

	for (uint8_t c = '0'; c <= '9'; c++) {
		charClassifiers[c] = LanguageDefinition::CharacterType::DIGIT;
	}
	
	// Then, do other special characters.
	charClassifiers['#'] = LanguageDefinition::CharacterType::COMMENT_SINGLE_LINE;
	charClassifiers['\n'] = LanguageDefinition::CharacterType::NEWLINE;
	charClassifiers['"'] = LanguageDefinition::CharacterType::STR_QUOTE;
	charClassifiers[';'] = LanguageDefinition::CharacterType::LINE_BREAK;
}

TokenList LexeralAnalysis::lexString(const std::string& str) {
	// First, we need to check with with kind of character we are dealing with.


	// Then, we need to extract the data required for this token, if required.

	// Finally, we will create a new Token and add it to our token list

	// We repeat this process to the end of the string. We throw exceptions if things go wrong.
}

LanguageDefinition::CharacterType LexeralAnalysis::classifyCharacter(const uint8_t ch) {
	try {
		return charClassifiers.at(ch);
	}
	catch (std::out_of_range) {
		throw std::exception("Unable to classify character!"); // TODO: print ch
	}
}
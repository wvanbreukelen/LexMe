#include "stdafx.h"
#include "LexeralAnalysis.h"

LexeralAnalysis::LexeralAnalysis() {
	// Create our classifier map.
	// First, do all alpha characters.
	for (uint8_t c = 'a'; c <= 'z'; c++) {
		charClassifiers[c] = CharacterType::LETTER;
	}

	for (uint8_t c = 'A'; c <= 'Z'; c++) {
		charClassifiers[c] = CharacterType::LETTER;
	}

	for (uint8_t c = '0'; c <= '9'; c++) {
		charClassifiers[c] = CharacterType::DIGIT;
	}
	
	// Then, do other special characters.
	charClassifiers['#']  = CharacterType::COMMENT_SINGLE_LINE;
	charClassifiers['\n'] = CharacterType::NEWLINE;
	charClassifiers['"']  = CharacterType::STR_QUOTE;
	charClassifiers[';']  = CharacterType::LINE_BREAK;
	charClassifiers[' ']  = CharacterType::WHITESPACE;

	for (const auto &op : LanguageDefinition::operators) {
		std::string opText = op.text;

		for (unsigned int i = 0; i < opText.length(); i++) {
			charClassifiers[opText[i]] = CharacterType::OPERATOR;
		}
	}
}

TokenList LexeralAnalysis::lexString(const char* str) {
	return lexString(std::string(str));
}

TokenList LexeralAnalysis::lexString(const std::string& str) {
	std::string tokenValue;
	LanguageDefinition::TokenType prevTokenType = TokenType::WHITESPACE;

	for (auto const &ch : str) {

		// First, we need to check with with kind of character we are dealing with.
		LanguageDefinition::CharacterType charType = classifyCharacter(ch);
		LanguageDefinition::TokenType newTokenType = resolveTokenType(charType, prevTokenType);

		if (newTokenType != prevTokenType) {
			if (!tokenValue.empty()) {
				// Finish token, process it.
				if (newTokenType == TokenType::OPERATOR) {
					std::vector<Operator> opMatches;

				}
			}
		}

		if (newTokenType != TokenType::WHITESPACE && newTokenType != TokenType::LINE_END) {
			// Then, we need to extract the data required for this token, if required.
			// Add the character to the token value.
			tokenValue += ch;
		}

		prevTokenType = newTokenType;

		

		// Finally, we will create a new Token and add it to our token list

		// We repeat this process to the end of the string. We throw exceptions if things go wrong.
	}

	return TokenList();
}

LanguageDefinition::CharacterType LexeralAnalysis::classifyCharacter(const uint8_t ch) {
	/**try {
		return charClassifiers.at(ch);
	}
	catch (std::out_of_range) {
		throw std::exception("Unable to classify character!"); // TODO: print ch
	}**/

	// Try to find a matching character definition within the hashtable.
	auto result = charClassifiers.find(ch);

	if (result == charClassifiers.end()) {
		// Character type unknown.
		return CharacterType::UNKNOWN;
	}
	else {
		return result->second;
	}
}

LanguageDefinition::TokenType LexeralAnalysis::resolveTokenType(LanguageDefinition::CharacterType charType, LanguageDefinition::TokenType prevTokenType) {
	switch (charType) {
	case CharacterType::WHITESPACE:
		return TokenType::WHITESPACE;
	case CharacterType::OPERATOR:
		return TokenType::OPERATOR;
	case CharacterType::LETTER:
	case CharacterType::DIGIT:
		if (prevTokenType == TokenType::ID || prevTokenType == TokenType::LITERAL) {
			return prevTokenType;
		} else if (charType == CharacterType::DIGIT) {
			return TokenType::DIGIT;
		}
		else {
			return TokenType::ID;
		}
	default:
		return TokenType::UNKNOWN;
	}
}
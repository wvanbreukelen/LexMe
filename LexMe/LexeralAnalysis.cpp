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
	charClassifiers['.']  = CharacterType::PRECISION;

	// Add operators
	for (const auto &op : LanguageDefinition::opMap.getMap()) {
		std::string opText = op.second.text;

		for (unsigned int i = 0; i < opText.length(); i++) {
			charClassifiers[opText[i]] = CharacterType::OPERATOR;
		}
	}
}

TokenList LexeralAnalysis::lexString(const char* str) {
	return lexString(std::string(str));
}

TokenList LexeralAnalysis::lexString(const std::string& str) {
	unsigned int linePos = 1, charPos = 1;
	TokenList tokens;
	std::string tokenValue;
	LanguageDefinition::TokenType prevTokenType = TokenType::WHITESPACE;

	//for (auto const &ch : str) {
	for (unsigned int i = 0; i < str.size(); i++) {

		// First, we need to check with with kind of character we are dealing with.
		LanguageDefinition::CharacterType charType = classifyCharacter(str[i]);
		LanguageDefinition::TokenType newTokenType = resolveTokenType(charType, prevTokenType);

		if (newTokenType != prevTokenType) {
			if (!tokenValue.empty()) {
				// Token type transition, process the previous token type.
				if (prevTokenType == TokenType::OPERATOR) {
					std::vector<Operator> opMatches;
					LanguageDefinition::opMap.searchMatchingOperators(tokenValue, opMatches);

					for (auto op : opMatches) {
						// Finally, we will create a new Token and add it to our token list
						tokens.push_back(makeToken(prevTokenType, linePos, charPos, op.text));
					}
				} else {
					// If it is a string, we pop off the first character (the semicolon).
					if (prevTokenType == TokenType::STRING) {
						tokenValue.erase(0, 1);
					}

					// true and false are both literals, not identifiers, so change the type to literal.
					if (prevTokenType == TokenType::ID && tokenValue == LanguageDefinition::Literals::literalTrue || tokenValue == LanguageDefinition::Literals::literalFalse) {
						prevTokenType = TokenType::LITERAL;
					}

					// Finally, we will create a new Token and add it to our token list
					tokens.push_back(makeToken(prevTokenType, linePos, charPos, tokenValue));
				}
			}

			tokenValue = "";
		}

		if (newTokenType != TokenType::WHITESPACE && newTokenType != TokenType::LINE_END) {

			// Then, we need to extract the data required for this token, if required.
			// Add the character to the token value.

			tokenValue += str[i];
			
			
		}

		prevTokenType = newTokenType;

		if (str[i] == '\n') {
			linePos++;
			charPos = 1;
		} else {
			charPos++;
		}

		// We repeat this process to the end of the string. We throw exceptions if things go wrong.
	}

	return tokens;
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
	if (prevTokenType == TokenType::STRING) {
		if (charType == CharacterType::STR_QUOTE) {
			return TokenType::WHITESPACE; // If the string is empty, make it a whitespace.
		}
		else {
			return TokenType::STRING;
		}
	}
	
	switch (charType) {
	case CharacterType::WHITESPACE:
		return TokenType::WHITESPACE;
	case CharacterType::LINE_BREAK:
	case CharacterType::NEWLINE:
		return TokenType::LINE_END;
	case CharacterType::OPERATOR:
		return TokenType::OPERATOR;
	case CharacterType::LETTER:
	case CharacterType::DIGIT:
		if (prevTokenType == TokenType::ID || prevTokenType == TokenType::LITERAL) {
			return prevTokenType;
		} else if (charType == CharacterType::DIGIT) {
			return TokenType::LITERAL; // DIGIT
		}
		else {
			return TokenType::ID;
		}
	case CharacterType::PRECISION:
		// TODO: what will we do if there are two precision points?
		if (prevTokenType == TokenType::LITERAL) { // DIGIT
			return TokenType::LITERAL; // DIGIT
		}
	case CharacterType::STR_QUOTE:
		return TokenType::STRING;
	default:
		return TokenType::UNKNOWN;
	}
}
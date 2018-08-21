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
	
	// Then, do other (language dependend) special characters.
	charClassifiers[LanguageDefinition::LanguageGeneral::commentSingleLine]  = CharacterType::COMMENT_SINGLE_LINE;
	charClassifiers[LanguageDefinition::LanguageGeneral::instructionEnd] = CharacterType::LINE_BREAK;
	charClassifiers[LanguageDefinition::LanguageGeneral::stringQuote] = CharacterType::STR_QUOTE;

	charClassifiers['\n'] = CharacterType::NEWLINE;
	charClassifiers[' ']  = CharacterType::WHITESPACE;
	charClassifiers['.']  = CharacterType::PRECISION;

	// Add operators
	for (const auto &op : LanguageDefinition::operatorMap.getMap()) {
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

	for (unsigned int i = 0; i <= str.size(); i++) {

		// First, we need to check with with kind of character we are dealing with.
		LanguageDefinition::CharacterType charType = classifyCharacter(str, i);
		LanguageDefinition::TokenType newTokenType = resolveTokenType(charType, prevTokenType);

		if (newTokenType != prevTokenType) {

			if (!tokenValue.empty()) {
				if (prevTokenType != TokenType::LINE_COMMENT && prevTokenType != TokenType::BLOCK_COMMENT) {

					// Token type transition, process the previous token type.
					if (prevTokenType == TokenType::OPERATOR) {
						std::vector<Operator> opMatches;
						LanguageDefinition::operatorMap.searchMatching(tokenValue, opMatches);

						for (auto op : opMatches) {
							// Finally, we will create a new Token and add it to our token list
							tokens.push_back(makeToken(prevTokenType, linePos, charPos, op.text));
						}
					} else {
						// If the are dealing with a comment, ignore it.

							// If it is a string, we pop off the first character (the semicolon).
						if (prevTokenType == TokenType::LITERAL_STRING) {
							// Only pop if the string is long enough
							if (tokenValue.length() > 0) {
								tokenValue.erase(0, 1);
							}
						}

						// If we are dealing with a literal number, try to be more specific (if possible)
						if (prevTokenType == TokenType::LITERAL_NUMBER) {
							for (const auto &op : LanguageDefinition::literalMap.getMap()) {
								std::string opText = op.second.text;

								// Token value must be bigger than a type identifier, for example the literal identifier 0x (for hexadecimals) could fit the current length of the tokenValue field.
								if (tokenValue.length() > opText.length()) {
									if (tokenValue.rfind(opText, 0) != std::string::npos) {
										// We have found a literal match!
										// Let's change the token type to the corresponding literal.
										prevTokenType = op.second.tokenType;
									}
								}

							}
						}

						// true and false are both literals, not identifiers, so change the type to literal.
						if (prevTokenType == TokenType::ID && tokenValue == LanguageDefinition::Literals::literalTrue || tokenValue == LanguageDefinition::Literals::literalFalse) {
							prevTokenType = TokenType::LITERAL_BOOL;
						}

						// Finally, we will create a new Token and add it to our token list
						tokens.push_back(makeToken(prevTokenType, linePos, charPos, tokenValue));

					}
				}
				else {
					//std::cout << "Block comment!\n";
					//Token t = makeToken(newTokenType, linePos, charPos, tokenValue);
					//t->print(std::cout);
					//i++;
					//newTokenType.

					// If it's a block comment, we will skip ahead some characters forward to skip the closing bracket of the statement.
					if (prevTokenType == TokenType::BLOCK_COMMENT) {
						i += strlen(LanguageDefinition::Comments::multilineEnd) - 1;
					}
				}
			}

			tokenValue = "";
		}

		if (newTokenType != TokenType::WHITESPACE && newTokenType != TokenType::LINE_END && newTokenType != TokenType::INSTRUCTION_END) {

			// Then, we need to extract the data required for this token, if required.
			// Add the character to the token value.

			tokenValue += str[i];
		}

		if (prevTokenType == TokenType::INSTRUCTION_END || prevTokenType == TokenType::LINE_END) {
			tokens.push_back(makeToken(prevTokenType, linePos, charPos, "")); // Has no token value.
		}

		prevTokenType = newTokenType;

		if (str[i] == '\n') {
			linePos++;
			charPos = 1;
		}
		else {
			charPos++;
		}

		// We repeat this process to the end of the string. We throw exceptions if things go wrong.
	}

	return tokens;
}

LanguageDefinition::CharacterType LexeralAnalysis::classifyCharacter(const std::string& str, const unsigned int chIndex) {
	bool isComment = true;

	// Check for multiline comments.
	switch (str[chIndex]) {
	case LanguageDefinition::Comments::multilineStart[0]:

		if ((str.size()) - (strlen(LanguageDefinition::Comments::multilineStart) - 1) > chIndex) {
			for (unsigned int i = 1; i < strlen(LanguageDefinition::Comments::multilineStart); i++) {
				if (str[chIndex + i] != LanguageDefinition::Comments::multilineStart[i]) {
					isComment = false;
					break;
				}
			}
			
			if (isComment) {
				return CharacterType::MULTI_LINE_COMMENT_START;
			}
			
		}
		break;
	case LanguageDefinition::Comments::multilineEnd[0]:
		
		if ((str.size()) - (strlen(LanguageDefinition::Comments::multilineEnd) - 1) > chIndex) {
			for (unsigned int i = 1; i < strlen(LanguageDefinition::Comments::multilineEnd); i++) {
				if (str[chIndex + i] != LanguageDefinition::Comments::multilineEnd[i]) {
					isComment = false;
					
					break;
				}
			}
			if (isComment) {
				return CharacterType::MULTI_LINE_COMMENT_END;
			}
		}

		break;
	}

	// Try to find a matching character definition within the hashtable.
	auto result = charClassifiers.find(str[chIndex]);

	if (result == charClassifiers.end()) {
		// Character type unknown.
		return CharacterType::UNKNOWN;
	}
	else {
		return result->second;
	}
}

LanguageDefinition::TokenType LexeralAnalysis::resolveTokenType(LanguageDefinition::CharacterType charType, LanguageDefinition::TokenType prevTokenType) {
	if (prevTokenType == TokenType::LINE_COMMENT)
	{
		if (charType == CharacterType::NEWLINE) {
			return TokenType::WHITESPACE;
		} else {
			return TokenType::LINE_COMMENT;
		}
	}
	
	if (prevTokenType == TokenType::LITERAL_STRING) {
		if (charType == CharacterType::STR_QUOTE) {
			return TokenType::WHITESPACE; // If the string is empty, make it a whitespace.
		}
		else {
			return TokenType::LITERAL_STRING;
		}
	}

	if (prevTokenType == TokenType::BLOCK_COMMENT) {
		if (charType == CharacterType::MULTI_LINE_COMMENT_END) {
			return TokenType::WHITESPACE;
		} else {
			return TokenType::BLOCK_COMMENT;
		}
	}
	
	switch (charType) {
	case CharacterType::MULTI_LINE_COMMENT_START:
		return TokenType::BLOCK_COMMENT;
	case CharacterType::MULTI_LINE_COMMENT_END:
		throw std::runtime_error("Block comment without start!");
		//std::cout << "Block without start!\n";
		//return TokenType::UNKNOWN;
	case CharacterType::WHITESPACE:
		return TokenType::WHITESPACE;
	case CharacterType::LINE_BREAK:
		return TokenType::INSTRUCTION_END;
	case CharacterType::NEWLINE:
		return TokenType::LINE_END;
	case CharacterType::COMMENT_SINGLE_LINE:
		return TokenType::LINE_COMMENT;
		return TokenType::UNKNOWN;
	case CharacterType::OPERATOR:
		return TokenType::OPERATOR;
	case CharacterType::LETTER:
	case CharacterType::DIGIT:
		if (prevTokenType == TokenType::ID || prevTokenType == TokenType::LITERAL_NUMBER) {
			return prevTokenType;
		} else if (charType == CharacterType::DIGIT) {
			return TokenType::LITERAL_NUMBER; // DIGIT
		}
		else {
			return TokenType::ID;
		}
	case CharacterType::PRECISION:
		// TODO: what will we do if there are two precision points?
		if (prevTokenType == TokenType::LITERAL_NUMBER) { // DIGIT
			return TokenType::LITERAL_NUMBER; // DIGIT
		}
	case CharacterType::STR_QUOTE:
		return TokenType::LITERAL_STRING;
	default:
		return TokenType::UNKNOWN;
	}
}
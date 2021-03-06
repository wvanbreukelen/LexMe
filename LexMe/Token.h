#pragma once

#include <string>
#include <memory>
#include "LanguageDefinition.h"

typedef LanguageDefinition::TokenType TokenType;

class TokenData
{
private:
	std::string value;
	TokenType type;

	int linePos, lineCharPos;
	std::string file;

public:
	TokenData(TokenType type, int linePos, int lineCharPos, std::string value, std::string file) : type(type), linePos(linePos), lineCharPos(lineCharPos), value(value), file(file) {};
	
	void setValue(std::string value) {
		this->value = value;
	}

	std::string getValue() {
		return value;
	}

	TokenType getType() {
		return type;
	}

	int getLinePos() {
		return linePos;
	}

	int getLineCharPos() {
		return lineCharPos;
	}

	void print(std::ostream &os) {
		os << '[';

		switch (type) {
		case TokenType::OPERATOR:
			os << "operator: " << value;
			break;
		case TokenType::ID:
			os << "identifier: " << value;
			break;
		case TokenType::DIGIT:
			os << "number: " << value;
			break;
		case TokenType::STRING:
			os << "string: " << value;
			break;
		case TokenType::LITERAL:
			os << "literal: " << value;
			break;
		case TokenType::NEWLINE:
			os << "newline";
			break;
		case TokenType::UNKNOWN:
			os << "unknown";
			break;
		default:
			break;
		}

		os << ']';
		os << ' ' << linePos << ':' << lineCharPos << std::endl;
	}
};

typedef std::shared_ptr<TokenData> Token;

Token makeToken(TokenType type, int linePos, int lineCharPos, std::string value, std::string file = "");
#pragma once

#include <string>
#include <memory>
#include "LanguageDefinition.h"

typedef LanguageDefinition::TokenType TokenType;

class TokenData
{
private:
	std::string value;
	TokenType type = TokenType::UNDEFINED;

	int linePos, lineCharPos;
	std::string file;

public:
	TokenData() { }

	TokenData(TokenType type, int linePos, int lineCharPos, std::string value, std::string file) : type(type), linePos(linePos), lineCharPos(lineCharPos), value(value), file(file) {};
	
	void setValue(std::string value) {
		this->value = value;
	}

	const std::string& getValue() const {
		return value;
	}

	const TokenType& getType() const {
		return type;
	}

	int getLinePos() const {
		return linePos;
	}

	int getLineCharPos() const {
		return lineCharPos;
	}

	void print(std::ostream &os) const {
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
		case TokenType::LITERAL_STRING:
			os << "literal string: " << value;
			break;
		case TokenType::LITERAL_BOOL:
			os << "literal bool: " << value;
			break;
		case TokenType::LITERAL_NUMBER:
			os << "literal number: " << value;
			break;
		case TokenType::LITERAL_HEX:
			os << "literal hex: " << value;
			break;
		case TokenType::LITERAL_BIN:
			os << "literal bin: " << value;
			break;
		case TokenType::LITERAL_OCT:
			os << "literal oct: " << value;
			break;
		case TokenType::NEWLINE:
			os << "newline";
			break;
		case TokenType::LINE_END:
			os << "line end";
			break;
		case TokenType::INSTRUCTION_END:
			os << "instruction end";
			break;
		case TokenType::BLOCK_COMMENT:
			os << "block comment";
			break;
		case TokenType::LINE_COMMENT:
			os << "line comment";
			break;
		case TokenType::UNKNOWN:
			os << "unknown";
			break;
		case TokenType::WHITESPACE:
			os << "whitespace";
			break;
		default:
			break;
		}

		os << ']';
		os << ' ' << linePos << ':' << lineCharPos;
	}

	friend std::ostream& operator<< (std::ostream& os, const TokenData& tokenData) {
		tokenData.print(os);
		return os;
	}
};

typedef std::shared_ptr<TokenData> Token;

Token makeToken(TokenType type, int linePos, int lineCharPos, std::string value, std::string file = "");
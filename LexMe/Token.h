#pragma once

#include <string>
#include <memory>

enum class TokenType { OPERATOR, NUMBER, ID, NEWLINE, COMMENT };

class TokenData
{
private:
	std::string value;
	TokenType type;

	int linePos, lineCharPos;
	std::string file;

public:
	TokenData(TokenType type, int linePos, int lineCharPos, std::string file = "") : type(type), linePos(linePos), lineCharPos(lineCharPos), file(file) {};
	

	void setValue(std::string &value) {
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
			os << "op: " << value;
			break;
		case TokenType::ID:
			os << "id: " << value;
			break;
		case TokenType::NUMBER:
			os << "num: " << value;
			break;
		case TokenType::COMMENT:
			os << "comm: " << value;
			break;
		case TokenType::NEWLINE:
			os << "newline";
			break;
		default:
			break;
		}

		os << ']';
	}
};

typedef std::shared_ptr<TokenData> Token;

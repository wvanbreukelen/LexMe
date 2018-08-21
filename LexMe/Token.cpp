#include "Token.h"

Token makeToken(TokenType type,  int linePos, int lineCharPos, std::string value, std::string file) {
	return Token(new TokenData(type, linePos, lineCharPos, value, file));
}

// LexMe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "TokenList.h"

int main()
{
	Lexer lexer;

	TokenList tokenList = lexer.process("print-> 4 * var + 1");

	// TokenType type, int linePos, int lineCharPos, std::string file = ""

	tokenList.push_back(Token(new TokenData(TokenType::OPERATOR, 23, 2, "*")));
	tokenList.push_back(Token(new TokenData(TokenType::ID, 23, 4, "name")));

	tokenList.print(std::cout);

	getchar();
	
	// Do something with the token list.


    return 0;
}


// LexMe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "TokenList.h"

int main()
{
	Lexer lexer;

	std::string str("var = 3;print: 438.36 * 0x6 + var;\n 3 * 5;print: \"Hello World\";z = true; # Comment\n x = false;");

	TokenList tokenList = lexer.process(str);

	// TokenType type, int linePos, int lineCharPos, std::string file = ""

	//tokenList.push_back(Token(new TokenData(LanguageDefinition::TokenType::OPERATOR, 23, 2, "*")));
	//tokenList.push_back(Token(new TokenData(LanguageDefinition::TokenType::ID, 23, 4, "name")));

	tokenList.print(std::cout);

	getchar();
	getchar();
	
	// Do something with the token list.


    return 0;
}


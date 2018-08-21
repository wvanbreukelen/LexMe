// LexMe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "TokenList.h"

int main()
{
	Lexer lexer;

	TokenList tokenList = lexer.processFile("program.zz");

	tokenList.print(std::cout);

	getchar();
	getchar();
	
	// Do something with the token list.

    return 0;
}


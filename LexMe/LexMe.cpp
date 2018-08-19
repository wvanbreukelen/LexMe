// LexMe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lexer.h"
#include "TokenList.h"

int main()
{
	Lexer lexer;

	TokenList tokenList = lexer.process("print-> 4 * var + 1");
	
	// Do something with the token list.


    return 0;
}


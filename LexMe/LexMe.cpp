// LexMe.cpp : Defines the entry point for the console application.
//

#include "Lexer.h"
#include "Operator.h"
#include "TokenList.h"
#include "TreeNode.h"

int main()
{
	Lexer lexer;

	TokenList tokenList = lexer.processFile("program.zz");

	tokenList.print(std::cout);

	// Do something with the token list.

    return 0;
}
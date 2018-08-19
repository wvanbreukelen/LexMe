#pragma once

#include <vector>
#include <iostream>
#include "Token.h"

class TokenList : public std::vector<Token>
{

public:
	TokenList();
	~TokenList();

	void print(std::ostream &os);
};


#include "TokenList.h"


TokenList::TokenList()
{
}

void TokenList::print(std::ostream &os) {
	for (const Token& token : *this) {
		token->print(os);
		os << std::endl;
	}
}


TokenList::~TokenList()
{
}

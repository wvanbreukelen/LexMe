#pragma once

#include <type_traits>
#include "Operator.h"
#include "OperatorMap.h"

namespace LanguageDefinition {


	enum class TokenType { 
		OPERATOR,
		ID,
		NEWLINE,
		COMMENT,
		WHITESPACE,
		LITERAL,
		DIGIT,
		LINE_END,
		UNKNOWN
	};

	enum class CharacterType {
		LETTER,
		DIGIT,
		NEWLINE,
		OPERATOR,
		STR_QUOTE,
		COMMENT_SINGLE_LINE,
		LINE_BREAK,
		PARH_LEFT,
		PARH_RIGHT,
		PRECISION,
		WHITESPACE,
		UNKNOWN
	};

	const Operator languageOperators[]{
		{":"},
		{"*"},
		{"/"},
		{":"},
		{"+"},
		{"-"}
	};

	static OperatorMap opMap(languageOperators, std::extent<decltype(languageOperators)>::value);
}

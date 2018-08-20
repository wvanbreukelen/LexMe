#pragma once

#include <type_traits>
#include "Operator.h"
#include "OperatorMap.h"
#include "Literal.h"

namespace LanguageDefinition {


	enum class TokenType { 
		OPERATOR,
		ID,
		NEWLINE,
		LINE_COMMENT,
		WHITESPACE,
		LITERAL,
		DIGIT,
		STRING,
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
		{"-"},
		{"&"},
		{"&&"},
		{"|"},
		{"||"},
		{"->"},
		{"="},
		{"?"},
		{"=="},
		{"!="},
		{"!"},
		{">"},
		{"<"},
		{">="},
		{"<="},
	};

	// Define literals, like true
	// Note that not all literals can be described, for example 6.02e23 or "music" cannot be described.
	//const Literal languageLiterals[]{
	//	{"true"},
	//	{"false"}
	//};

	namespace Literals {
		static constexpr char* literalTrue = "true";
		static constexpr char* literalFalse = "false";
	}

	static OperatorMap opMap(languageOperators, std::extent<decltype(languageOperators)>::value);
}

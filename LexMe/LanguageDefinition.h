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
		LITERAL_STRING,
		LITERAL_BOOL,
		LITERAL_NUMBER,
		DIGIT,
		//STRING,
		LINE_END,
		INSTRUCTION_END,
		UNKNOWN,
		UNDEFINED
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
		{":", 1}, // Function call
		{"*", 3}, // Multiplication sign
		{"/", 3}, // Division sign
		{"+", 4}, // Addition sign
		{"-", 4}, // Substraction sign
		{"&", 8}, // Bitwise AND
		{"&&", 11}, // Logical AND
		{"|", 10}, // Bitwise OR
		{"||", 12}, // Logital OR
		{"->", 1}, // Undefined
		{"=", 14}, // Assignment operator
		{"?", 13}, // Inversion
		{"==", 7}, // Comparison operator
		{"!=", 7}, // Comparison-inverted operator
		{"!", 2}, // Logical NOT operator
		{">", 6}, // Comparison: larger-then
		{"<", 6}, // Comparison: smaller-then
		{">=", 6}, // Comparison: larger-even-then
		{"<=", 6}, // Comparison: larger-smaller-then
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

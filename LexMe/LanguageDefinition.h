#pragma once

#include <type_traits>
#include "Operator.h"
#include "SearchableMap.h"
#include "Literal.h"
#include "TypeDefinition.h"

namespace LanguageDefinition {

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

	const Literal literalIdentifiers[]{
		{"0x", TokenType::LITERAL_HEX },
		{"0b", TokenType::LITERAL_BIN },
		{"0o", TokenType::LITERAL_OCT }
	};

	namespace Literals {
		static constexpr char* literalTrue = "true";
		static constexpr char* literalFalse = "false";
	}

	namespace Comments {
		static constexpr char commentSingleLine = '#';
		static constexpr char* multilineStart = "/**";
		static constexpr char* multilineEnd = "**/";
	}

	namespace LanguageGeneral {
		static constexpr char instructionEnd = ';';
		static constexpr char stringQuote = '"';
	}

	static SearchableMap<Operator> operatorMap(languageOperators, std::extent<decltype(languageOperators)>::value);
	static SearchableMap<Literal> literalMap(literalIdentifiers, std::extent<decltype(literalIdentifiers)>::value);
}

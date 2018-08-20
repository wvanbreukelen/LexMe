#pragma once

#include <unordered_map>
#include <vector>

namespace LanguageDefinition {
	const char languageOperators[]{
		':',
		'*',
		'/',
		':',
		'+',
		'-'
	};

	enum class TokenType { OPERATOR, ID, NEWLINE, COMMENT, WHITESPACE, LITERAL, DIGIT, LINE_END, UNKNOWN };

	enum class CharacterType {
		LETTER,
		DIGIT,
		NEWLINE,
		OPERATOR,
		STR_QUOTE,
		COMMENT_SINGLE_LINE,
		LINE_BREAK,
		WHITESPACE,
		UNKNOWN
	};

	struct Operator {
		const char* text;

		Operator(const char* text) : text(text) {};
	};

	static const std::vector<Operator> operators{
		{ "+" },
		{ "-" },
		{ "/" },
		{ "*" },
		{ ":" },
	};
}

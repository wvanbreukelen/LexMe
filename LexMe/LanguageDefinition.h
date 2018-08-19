#pragma once

#include <unordered_map>

namespace LanguageDefinition {
	const char languageOperators[]{
		':',
		'*',
		'/',
		':',
		'+',
		'-'
	};

	enum class CharacterType {
		LETTER,
		DIGIT,
		NEWLINE,
		OPERATOR,
		STR_QUOTE,
		COMMENT_SINGLE_LINE,
		LINE_BREAK,
		UNKNOWN
	};

	class Operator {

	};
}

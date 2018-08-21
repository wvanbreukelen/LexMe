#include "stdafx.h"
#include "Lexer.h"


Lexer::Lexer()
{
}

TokenList Lexer::process(const std::string &text) {
	return lexerAnalysis.lexString(text);
}

TokenList Lexer::process(const char* text) {
	return lexerAnalysis.lexString(text);
}

TokenList Lexer::processFile(const char* filename) {
	// Open the file
	std::ifstream file(filename);

	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file.");
	}

	// Read the file
	std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	// Close the file
	file.close();

	// Do the lexeral analysis
	return lexerAnalysis.lexString(fileContents);
}

TokenList Lexer::processFile(std::string filename) {
	return processFile(filename.c_str());
}

Lexer::~Lexer()
{
}

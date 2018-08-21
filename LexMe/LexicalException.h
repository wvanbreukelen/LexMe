#pragma once

#include <exception>
#include <string>

class LexicalException : public std::exception {
private:
	std::string msg;

public:
	explicit LexicalException(const char* message) : msg(message) {}

	explicit LexicalException(const std::string& message) :
		msg(message)
	{}

	virtual ~LexicalException() throw () {}

	virtual const char* what() const throw () {
		return msg.c_str();
	}
};
#pragma once

#include <memory>
#include "LanguageDefinition.h"
#include "Token.h"
using LanguageDefinition::TokenType;

class ASTNode
{
private:
	std::shared_ptr<ASTNode> leftChild, rightChild;
	Token token;
	//TokenTyspe tokenType = TokenType::UNKNOWN;

public:
	ASTNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, Token token) : leftChild(std::move(left)), rightChild(std::move(right)), token(token) {};
	ASTNode() : leftChild(nullptr), rightChild(nullptr) {};

	void setLeftNode(std::unique_ptr<ASTNode> leftNode) {
		leftChild = std::move(leftNode);
	}

	void setRightNode(std::unique_ptr<ASTNode> rightNode) {
		rightChild = std::move(rightNode);
	}

	std::shared_ptr<ASTNode> getLeftChild () const {
		return leftChild;
	}

	std::shared_ptr<ASTNode> getRightChild() const {
		return rightChild;
	}

	bool hasLeftChild() {
		return (!leftChild) ? true : false;
	}

	bool hasRightChild() {
		return (!rightChild) ? true : false;
	}

	bool isValidToken() {
		return (token->getType() == TokenType::UNDEFINED);
	}

	Token getToken() {
		return token;
	}
};


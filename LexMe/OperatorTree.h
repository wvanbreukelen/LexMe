#pragma once

#include "TreeNode.h"
#include "TokenList.h"

#include <vector>
#include <memory>
#include <map>

class OperatorTree
{
	using TreeNode = TreeNode<TokenData, 2>;

	std::unique_ptr<TreeNode> root = std::make_unique<TreeNode>();

	std::vector<TokenData> tokens;

	std::map<std::string, int> precedenceMap {
		{ "*", 0 },
		{ "/", 0 },
		{ "%", 0 },
		{ "+", 1 },
		{ "-", 1 }
	};

	bool isLiteral(const TokenType& type) const {
		return
			type == TokenType::LITERAL_BIN ||
			type == TokenType::LITERAL_BOOL ||
			type == TokenType::LITERAL_HEX ||
			type == TokenType::LITERAL_NUMBER ||
			type == TokenType::LITERAL_OCT ||
			type == TokenType::LITERAL_STRING;
	}

	enum class States {
		LHS,
		OPERATOR,
		RHS
	};

	int getPrecedence(const TokenData& operant) {
		return precedenceMap.at(operant.getValue());
	}

	States state = States::LHS;

	int lhsIndex;
	int operantIndex;
	int prevOperantIndex;

	void solveOperation(const TokenData& lhs, const TokenData& operant, const TokenData& rhs, const TokenData* prevOperant) {
		if (!root->hasChild(0)) {
			root->setData(operant);
			root->claimNode(std::make_unique<TreeNode>(lhs), 0);
			root->claimNode(std::make_unique<TreeNode>(rhs), 1);
		}
		else if (getPrecedence(*prevOperant) > getPrecedence(operant)) {
			TreeNode* mostRightNode = &root->getChild(1);

			while (mostRightNode->hasChild(1)) {
				mostRightNode = &mostRightNode->getChild(1);
			}

			mostRightNode->setData(operant);

			mostRightNode->claimNode(std::make_unique<TreeNode>(lhs), 0);
			mostRightNode->claimNode(std::make_unique<TreeNode>(rhs), 1);
		}
		else {
			TreeNode* mostRightNode = &root->getChild(1);

			while (mostRightNode->hasChild(1)) {
				mostRightNode = &mostRightNode->getChild(1);
			}

			TreeNode& parent = mostRightNode->getParent();

			std::unique_ptr<TreeNode> newTree = std::make_unique<TreeNode>(operant);
			newTree->claimNode(std::make_unique<TreeNode>(rhs), 1);

			if (parent.hasParent()) {
				TreeNode& greatParent = parent.getParent();
				newTree->claimNode(parent.getParent().releaseNode(1), 0);
				greatParent.claimNode(std::move(newTree), 1);
			}
			else {
				newTree->claimNode(std::move(root), 0);
				root = std::move(newTree);
			}
		}
	}

public:
	OperatorTree() : lhsIndex(-1), operantIndex(-1), prevOperantIndex(-1) { }

	void addOperation(const TokenData& operation) {
		if (operation.getType() != TokenType::OPERATOR) {
			throw std::runtime_error("Expected an operator token");
		}

		if (state == States::OPERATOR) {
			tokens.emplace_back(operation);
			prevOperantIndex = operantIndex;
			operantIndex = tokens.size() - 1;
			state = States::RHS;
		}
	}

	void addLiteral(const TokenData& lit) {
		if (state == States::LHS) {
			tokens.emplace_back(lit);
			lhsIndex = tokens.size() - 1;
			state = States::OPERATOR;
		}
		else if (state == States::RHS) {
			tokens.emplace_back(lit);

			solveOperation(tokens[lhsIndex], tokens[operantIndex], tokens.back(), prevOperantIndex == -1 ? nullptr : &tokens[prevOperantIndex]);

			lhsIndex = tokens.size() - 1;

			state = States::OPERATOR;
		}
	}

	void readTokens(const TokenList& tokens) {
		auto it = tokens.begin();

		addLiteral(**it++);

		while (it < tokens.end() - 1) {
			addOperation(**it++);
			addLiteral(**it++);
		}
	}

	friend std::ostream& operator<< (std::ostream& os, const OperatorTree& operatorTree) {
		return os << *operatorTree.root;
	}
};
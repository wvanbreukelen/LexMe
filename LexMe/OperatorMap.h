#pragma once

#include <unordered_map>
#include "Operator.h"

class OperatorMap {
private:
	std::unordered_map<std::string, Operator> map;

public:
	OperatorMap(const Operator operators[], size_t size) {
		for (unsigned int i = 0; i < size; i++) {
			map[operators[i].text] = operators[i];
		}
	}

	std::unordered_map<std::string, Operator>& getMap();
	void insert(const Operator op);
	void searchMatchingOperators(const std::string text, std::vector<Operator>& matchingOps);
};
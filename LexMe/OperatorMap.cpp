#include "OperatorMap.h"

std::unordered_map<std::string, Operator>& OperatorMap::getMap() {
	return map;
}

void OperatorMap::insert(const Operator op) {
	map[op.text] = op;
}

void OperatorMap::searchMatchingOperators(const std::string text, std::vector<Operator>& matchingOps) {
	unsigned int textStart = 0;
	unsigned int textEnd = text.size();

	while (textStart < text.size()) {

		while (true) {
			if (textEnd <= textStart) {
				throw std::exception("Unknown operator!");
			}
			//auto op = findOperator(text.substr(textIndex, textEnd - textIndex));
			auto op = map.find(text.substr(textStart, textEnd - textStart));

			if (op == map.end()) {
				textEnd--;
			}
			else {
				matchingOps.push_back(op->second);
				textStart = textEnd;
				textEnd = text.size();
				break;
			}
		}
	}
}
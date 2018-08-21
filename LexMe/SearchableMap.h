#pragma once

#include <unordered_map>

template <typename T>
class SearchableMap {
private:
	std::unordered_map<std::string, T> map;

public:
	SearchableMap(const T elems[], const size_t size) {
		for (unsigned int i = 0; i < size; i++) {
			map[elems[i].text] = elems[i];
		}
	}

	std::unordered_map<std::string, T>& getMap() {
		return map;
	}

	void insert(const T op) {
		map[op.text] = op;
	}

	void searchMatching(const std::string text, std::vector<T>& matchingOps) {
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
};
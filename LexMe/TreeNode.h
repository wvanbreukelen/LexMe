#pragma once

#include <vector>
#include <array>
#include <stack>

template <class DATA_T, int CHILDREN_COUNT = -1>
class TreeNode {
	std::array<std::unique_ptr<TreeNode>, CHILDREN_COUNT> children;

	DATA_T data;

	size_t countChildren() const {
		size_t childrenCount = 0;

		for (size_t i = 0; i < CHILDREN_COUNT; i++) {
			childrenCount += children[i] != nullptr;
		}

		return childrenCount;
	}

	void print(std::ostream& os, int childIndex, int anchestorAmount, int brotherAmount, std::vector<bool>& dashesStack) const {
		if (anchestorAmount > 0) {
			for (int i = 0; i < anchestorAmount - 1; i++) {
				if (dashesStack[i]) {
					os << "|   ";
				}
				else {
					os << "    ";
				}
			}
		
			// If the child index equals the amount of brothers, it means the child is listed last and thus a \ can be used.
			if (childIndex == brotherAmount) {
				os << "\\---";
			}
			else {
				os << "|---";
			}
		}

		os << data << '\n';

		for (size_t i = 0; i < CHILDREN_COUNT; i++) {
			if (children[i] != nullptr) {
				bool unprintedChildrenRemaining = (i + 1) < CHILDREN_COUNT && children[i + 1] != nullptr;

				dashesStack.push_back(unprintedChildrenRemaining);
				children[i]->print(os, i, anchestorAmount + 1, countChildren() - 1, dashesStack);
				dashesStack.pop_back();
			}
		}
	}

public:
	/**
	 * @brief Default constructor
	 */
	TreeNode() : data() { }

	/**
	 * @brief Default constructor
	 */
	TreeNode(const DATA_T& data) : data(data) { }

	/**
	 * @brief Copy constructor
	 */
	TreeNode(const TreeNode& copied) {
		*this = copied;
	}

	/**
	 * @brief Copy assignment
	 */
	TreeNode& operator= (const TreeNode& copied) {
		data = copied.data;

		for (size_t i = 0; i < CHILDREN_COUNT; i++) {
			if (copied.children[i] != nullptr) {
				children[i] = std::make_unique<TreeNode>(*copied.children[i]);
			}
		}

		return *this;
	}

	/**
	 * @brief Move constructor
	 */
	TreeNode(TreeNode&& moved) {
		*this = moved;
	}
	
	/**
	 * @brief Move assignment
	 */
	TreeNode& operator= (TreeNode&& moved) {
		data = std::move(moved.data);
		children = std::move(moved.children);
		return *this;
	}

	// Temporary, default destructor is enough for this class
	~TreeNode() {
		std::cout << "TreeNode destructed" << std::endl;
	}

	DATA_T& getData() {
		return data;
	}

	void setData(const DATA_T& data) {
		this->data = data;
	}

	/**
	 * @brief Releases ownership from child n and returns an unmanaged pointer
	 * 
	 * @details
	 * Use this function when changing parents of subtrees. Once this function
	 * is called, the subtree becomes unmanaged and the responsibility of managing
	 * the memory is for the user.
	 * A nullptr is returned when there is no subtree known at index n.
	 *
	 * @param n Index of the subtree
	 *
	 * @return TreeNode<DATA_T, CHIILDREN_COUNT> released subtree
	 */
	std::unique_ptr<TreeNode>&& releaseSubtree(size_t n) {
		return std::move(children[n]);
	}

	/**
	 * @brief Claims ownership of the provided subtree
	 *
	 * @details
	 * Use this function when adding a subtree. Once this function is called,
	 * the subtree becomes managed by the parenting node and the responsibility
	 * of managing the memory is taken from the user. Which means the subtree
	 * may not be destructed before it is released using releaseSubtree(int n).
	 * Throws an exception when the new amount of children exceeds the capacity
	 * provided as template argument CHILDREN_COUNT.
	 * 
	 * @param subtree Pointer to the TreeNode to claim
	 *
	 * @return int Index of the newly claimed subtree
	 */
	size_t claimSubtree(std::unique_ptr<TreeNode>&& subtree) {
		size_t i = 0;

		for (; i < CHILDREN_COUNT; i++) {
			if (children[i] == nullptr) {
				children[i] = std::move(subtree);
				break;
			}
		}

		if (i == CHILDREN_COUNT) {
			throw std::out_of_range("Could not claim subtree since there are no free slots");
		}

		return i;
	}

	/**
	 * @brief Claims ownership of the provided subtree at index n
	 *
	 * @details
	 * Use this function when adding a subtree at the desired index. Once this
	 * function is called, the subtree becomes managed by the parenting node
	 * and the responsibility of managing the memory is taken from the user.
	 * Which means the subtree may not be destructed before it is released
	 * using releaseSubtree(int n).
	 * The subtree previously known at index n gets detructed, if applicable.
	 * Throws an exception when n is out of range of the bounds determined by
	 * the template argument CHILDREN_COUNT.
	 *
	 * @param subtree Pointer to the TreeNode to claim
	 * @param n Index to claim the TreeNode at
	 */
	void claimSubtree(std::unique_ptr<TreeNode>&& subtree, size_t n) {
		if (n >= CHILDREN_COUNT) {
			throw std::out_of_range("Could not claim subtree since n >= CHILDREN_COUNT");
		}

		if (children[i] == nullptr) {
			children[i] = std::move(subtree);
		}
	}

	friend std::ostream& operator<< (std::ostream& os, const TreeNode& node) {
		std::vector<bool> dashesStack;
		node.print(os, 0, 0, 0, dashesStack);
		return os;
	}
};
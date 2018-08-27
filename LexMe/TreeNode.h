#pragma once

#include <vector>
#include <array>

template <class DATA_T, int CHILDREN_COUNT, template<class...> class PTR_T>
class TreeNode;

template <class DATA_T, int CHILDREN_COUNT, template<class...> class PTR_T>
class TreePrinter {
	using TreeNode = TreeNode<DATA_T, CHILDREN_COUNT, PTR_T>;

	std::vector<bool>& dashesStack;
	std::vector<const TreeNode*>& visitedNodes;

	const TreeNode& treeNode;

	inline void printIdentation(std::ostream& os, int anchestorAmount, std::vector<bool>& dashesStack) const {
		for (int i = 0; i < anchestorAmount - 1; i++) {
			if (dashesStack[i]) {
				os << "|   ";
			}
			else {
				os << "    ";
			}
		}
	}

public:
	TreePrinter(const TreeNode& treeNode) : treeNode(treeNode) { }
	TreePrinter(const TreeNode& treeNode, std::vector<bool>& dashesStack, std::vector<const TreeNode*>& visitedNodes) : treeNode(treeNode), dashesStack(dashesStack), visitedNodes(visitedNodes) { }

	void print(std::ostream& os, int childIndex, int anchestorAmount, int brotherAmount) {
		auto it = std::find(visitedNodes.begin(), visitedNodes.end(), &treeNode);

		if (anchestorAmount > 0) {
			printIdentation(os, anchestorAmount, dashesStack);

			// If the child index equals the amount of brothers, it means the child is listed last and thus a \ can be used.
			if (childIndex == brotherAmount) {
				os << "\\---";
			}
			else {
				os << "|---";
			}
		}

		os << treeNode.data << '\n';

		if (it != visitedNodes.end()) {
			printIdentation(os, anchestorAmount, dashesStack);

			os << "    \\---[...]" << std::endl;
			return;
		}

		size_t i = 0;

		for (const TreeNode& child : treeNode) {
			// This boolean toggles true when there are unprinted children remaining
			bool unprintedChildrenRemaining = (i + 1) < treeNode.getSlots() && treeNode.children[i + 1] != nullptr;

			dashesStack.push_back(unprintedChildrenRemaining);
			visitedNodes.push_back(&treeNode);
			TreePrinter printer(child, dashesStack, visitedNodes);
			printer.print(os, i, anchestorAmount + 1, treeNode.countChildren() - 1);
			dashesStack.pop_back();

			i++;
		}
	}
};

template <class DATA_T, int CHILDREN_COUNT = -1, template<class...> class PTR_T = std::unique_ptr>
class TreeNode {
	using ChildrenContainer = std::array<PTR_T<TreeNode>, CHILDREN_COUNT>;

	TreeNode* parent;
	ChildrenContainer children;

	DATA_T data;

	size_t countChildren() const {
		size_t childrenCount = 0;

		for (size_t i = 0; i < CHILDREN_COUNT; i++) {
			childrenCount += children[i] != nullptr;
		}

		return childrenCount;
	}

	size_t getSlots() const {
		return CHILDREN_COUNT;
	}

	void print(std::ostream& os) const {
		std::vector<bool> dashesStack;
		std::vector<const TreeNode*> visitedNodes;
		TreePrinter<DATA_T, CHILDREN_COUNT, PTR_T> printer(*this, dashesStack, visitedNodes);
		printer.print(os, 0, 0, 0);
	}

	friend class TreePrinter<DATA_T, CHILDREN_COUNT, PTR_T>;

public:
	class iterator {
		using wrapping_iterator = typename ChildrenContainer::iterator;

		using self_type = iterator;
		using value_type = TreeNode<DATA_T, CHILDREN_COUNT, PTR_T>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;

		wrapping_iterator ptr;
		wrapping_iterator endPtr;

		void skipNullptrs() {
			while (ptr != endPtr && *ptr == nullptr) {
				ptr++;
			}
		}

	public:
		iterator(const wrapping_iterator& begin, const wrapping_iterator& end) : ptr(begin), endPtr(end) {
			skipNullptrs();
		}

		iterator(const wrapping_iterator& end) : ptr(end) { }

		reference operator* () const {
			return **ptr;
		}

		pointer operator-> () const {
			return *ptr;
		}

		self_type& operator++ () {
			ptr++;

			skipNullptrs();

			return *this;
		}

		self_type& operator++(int junk) {
			++(*this);
			return *this;
		}

		bool operator== (const self_type& rhs) const {
			return ptr == rhs.ptr;
		}

		bool operator!= (const self_type& rhs) const {
			return ptr != rhs.ptr;
		}
	};

	class const_iterator {
		using wrapping_iterator = typename ChildrenContainer::const_iterator;

		using self_type = const_iterator;
		using value_type = TreeNode<DATA_T, CHILDREN_COUNT, PTR_T>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;

		wrapping_iterator ptr;
		wrapping_iterator endPtr;

		void skipNullptrs() {
			while (ptr != endPtr && *ptr == nullptr) {
				ptr++;
			}
		}

	public:
		const_iterator(const wrapping_iterator& begin, const wrapping_iterator& end) : ptr(begin), endPtr(end) {
			skipNullptrs();
		}

		const_iterator(const wrapping_iterator& end) : ptr(end) { }

		const reference operator* () const {
			return **ptr;
		}

		const pointer operator-> () const {
			return *ptr;
		}

		self_type& operator++ () {
			ptr++;

			skipNullptrs();

			return *this;
		}

		self_type& operator++(int junk) {
			++(*this);
			return *this;
		}

		bool operator== (const self_type& rhs) const {
			return ptr == rhs.ptr;
		}

		bool operator!= (const self_type& rhs) const {
			return ptr != rhs.ptr;
		}
	};

	/**
	 * @brief Default constructor
	 */
	TreeNode() : parent(nullptr), data() { }

	/**
	 * @brief Constructor with data
	 */
	TreeNode(const DATA_T& data) : parent(nullptr), data(data) { }

	/**
	 * @brief Copy constructor
	 */
	TreeNode(const TreeNode& copied) {
		*this = copied;
	}

	/**
	 * @brief Copy assignment
	 * 
	 * @param copied
	 *
	 * @return TreeNode&
	 */
	TreeNode& operator= (const TreeNode& copied) {
		data = copied.data;

		for (size_t i = 0; i < getSlots(); i++) {
			if (copied.children[i] != nullptr) {
				std::unique_ptr<TreeNode> copy = std::make_unique<TreeNode>(*copied.children[i]);
				children[i] = std::move(PTR_T<TreeNode>(copy.release()));
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
	 *
	 * @param moved
	 *
	 * @return TreeNode&
	 */
	TreeNode& operator= (TreeNode&& moved) {
		data = std::move(moved.data);
		children = std::move(moved.children);
		return *this;
	}

	/**
	 * @brief Returns a boolean about whether the node has a parent
	 */
	bool hasParent() const {
		return parent != nullptr;
	}

	/**
	 * @brief Returns a reference to the parent node
	 */
	TreeNode& getParent() const {
		return *parent;
	}

	/**
	 * @brief Returns a reference to the node's data member
	 *
	 * @return DATA_T& Reference to the data member
	 */
	DATA_T& getData() {
		return data;
	}

	/**
	 * @brief Sets the node's data to the provided data
	 *
	 * @details
	 * Sets the node's data field to the provided data. This is done by
	 * directly copying the provided parameter. If a value is known by
	 * reference, the DATA_T template of the TreeNode must be a pointer,
	 * since references should not be null-initialized.
	 *
	 * @param data Data to copy into the node's data
	 */
	void setData(const DATA_T& data) {
		this->data = data;
	}

	/**
	 * @brief Iterates over all direct children looking for a node with the provided data
	 *
	 * @details 
	 * Iterates over all direct children looking for the first node with the data
	 * that equals the provided data param. If no node was found, a nullptr is 
	 * returned.
	 *
	 * @param data The data that is compared with the data of the nodes
	 *
	 * @return TreeNode* Pointer to the matched node
	 * @return nullptr No node was matched
	 */
	TreeNode* findNode(const DATA_T& data) {
		for (size_t i = 0; i < getSlots(); i++) {
			if (children[i] != nullptr && children[i]->getData() == data) {
				return children[i];
			}
		}

		return nullptr;
	}

	/**
	 * @brief Recurses through all descendant nodes and returns a pointer when found
	 *
	 * @details
	 * Recurses through all descendant nodes looking for the first node which data
	 * equals the provided data. If no node was found, a nullptr is returned.
	 *
	 * @param data The data that is compared with the data of the nodes
	 *
	 * @return TreeNode* Pointer to the matched node
	 * @return nullptr No node was matched
	 */
	TreeNode* findNodeRecursive(const DATA_T& data) {
		for (size_t i = 0; i < getSlots(); i++) {
			if (children[i] != nullptr) {
				if (children[i]->getData() == data) {
					return children[i];
				} else {
					return children[i]->findNodeRecursive(data);
				}
			}
		}

		return nullptr;
	}

	/**
	 * @brief Releases ownership from child n and returns an unmanaged pointer
	 * 
	 * @details
	 * Use this function when changing parents of nodes. Once this function
	 * is called, the node becomes unmanaged and the responsibility of managing
	 * the memory is for the user.
	 * A nullptr is returned when there is no node known at index n.
	 * Throws an out of range exception when n >= CHILDREN_COUNT.
	 *
	 * @param n Index of the node
	 *
	 * @return TreeNode<DATA_T, CHIILDREN_COUNT> released node
	 */
	PTR_T<TreeNode>&& releaseNode(size_t n) {
		if (n >= CHILDREN_COUNT) {
			throw std::out_of_range("Could not release node since n >= CHILDREN_COUNT");
		}

		children[n]->parent = nullptr;
		return std::move(children[n]);
	}

	/**
	 * @brief Claims ownership of the provided node
	 *
	 * @details
	 * Use this function when adding a node. Once this function is called,
	 * the node becomes managed by the parenting node and the responsibility
	 * of managing the memory is taken from the user. Which means the node
	 * may not be destructed before it is released using releaseNode(int n).
	 * Throws an exception when the new amount of children exceeds the capacity
	 * provided as template argument CHILDREN_COUNT.
	 * 
	 * @param node Pointer to the TreeNode to claim
	 *
	 * @return int Index of the newly claimed node
	 */
	size_t claimNode(PTR_T<TreeNode>&& node) {
		size_t i = 0;

		for (; i < CHILDREN_COUNT; i++) {
			if (children[i] == nullptr) {
				children[i] = std::move(node);
				children[i]->parent = this;
				break;
			}
		}

		if (i == CHILDREN_COUNT) {
			throw std::out_of_range("Could not claim node since there are no free slots");
		}

		return i;
	}

	/**
	 * @brief Claims ownership of the provided node at index n
	 *
	 * @details
	 * Use this function when adding a node at the desired index. Once this
	 * function is called, the node becomes managed by the parenting node
	 * and the responsibility of managing the memory is taken from the user.
	 * Which means the node may not be destructed before it is released
	 * using releaseNode(int n).
	 * The node previously known at index n gets detructed, if applicable.
	 * Throws an exception when n is out of range of the bounds determined by
	 * the template argument CHILDREN_COUNT.
	 *
	 * @param node Pointer to the TreeNode to claim
	 * @param n Index to claim the TreeNode at
	 */
	void claimNode(PTR_T<TreeNode>&& node, size_t n) {
		if (n >= CHILDREN_COUNT) {
			throw std::out_of_range("Could not claim node since n >= CHILDREN_COUNT");
		}

		children[n] = std::move(node);
		children[n]->parent = this;
	}

	/**
	 * @brief Returns true when a child is present at index n
	 */
	bool hasChild(size_t n) const {
		return children.at(n) != nullptr;
	}

	/**
	 * @brief Returns a reference to the child at index n
	 */
	TreeNode& getChild(size_t n) const {
		return *children.at(n);
	}

	iterator begin() {
		return iterator(children.begin(), children.end());
	}

	iterator end() {
		return iterator(children.end());
	}

	const const_iterator begin() const {
		return const_iterator(children.begin(), children.end());
	}

	const const_iterator end() const {
		return const_iterator(children.end());
	}

	friend std::ostream& operator<< (std::ostream& os, const TreeNode& node) {
		node.print(os);
		return os;
	}
};

template <class DATA_T, template<class...> class PTR_T>
class TreeNode<DATA_T, -1, PTR_T> {
	using ChildrenContainer = std::vector<PTR_T<TreeNode>>;

	TreeNode* parent;
	ChildrenContainer children;

	DATA_T data;

	size_t countChildren() const {
		size_t childrenCount = 0;

		for (size_t i = 0; i < children.size(); i++) {
			childrenCount += children[i] != nullptr;
		}

		return childrenCount;
	}

	size_t getSlots() const {
		return children.size();
	}

	void print(std::ostream& os) const {
		std::vector<bool> dashesStack;
		std::vector<const TreeNode*> visitedNodes;
		TreePrinter<DATA_T, -1, PTR_T> printer(*this, dashesStack, visitedNodes);
		printer.print(os, 0, 0, 0);
	}

	friend class TreePrinter<DATA_T, -1, PTR_T>;

public:
	class iterator {
		using wrapping_iterator = typename ChildrenContainer::iterator;

		using self_type = iterator;
		using value_type = TreeNode<DATA_T, -1, PTR_T>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;

		wrapping_iterator ptr;
		wrapping_iterator endPtr;

		void skipNullptrs() {
			while (ptr != endPtr && *ptr == nullptr) {
				ptr++;
			}
		}

	public:
		iterator(const wrapping_iterator& begin, const wrapping_iterator& end) : ptr(begin), endPtr(end) {
			skipNullptrs();
		}

		iterator(const wrapping_iterator& end) : ptr(end) { }

		reference operator* () const {
			return **ptr;
		}

		pointer operator-> () const {
			return *ptr;
		}

		self_type& operator++ () {
			ptr++;

			skipNullptrs();

			return *this;
		}

		self_type& operator++(int junk) {
			++(*this);
			return *this;
		}

		bool operator== (const self_type& rhs) const {
			return ptr == rhs.ptr;
		}

		bool operator!= (const self_type& rhs) const {
			return ptr != rhs.ptr;
		}
	};

	class const_iterator {
		using wrapping_iterator = typename ChildrenContainer::const_iterator;

		using self_type = const_iterator;
		using value_type = TreeNode<DATA_T, -1, PTR_T>;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;

		wrapping_iterator ptr;
		wrapping_iterator endPtr;

		void skipNullptrs() {
			while (ptr != endPtr && *ptr == nullptr) {
				ptr++;
			}
		}

	public:
		const_iterator(const wrapping_iterator& begin, const wrapping_iterator& end) : ptr(begin), endPtr(end) {
			skipNullptrs();
		}

		const_iterator(const wrapping_iterator& end) : ptr(end) { }

		const reference operator* () const {
			return **ptr;
		}

		const pointer operator-> () const {
			return *ptr;
		}

		self_type& operator++ () {
			ptr++;

			skipNullptrs();

			return *this;
		}

		self_type& operator++(int junk) {
			++(*this);
			return *this;
		}

		bool operator== (const self_type& rhs) const {
			return ptr == rhs.ptr;
		}

		bool operator!= (const self_type& rhs) const {
			return ptr != rhs.ptr;
		}
	};

	/**
	 * @brief Default constructor
	 */
	TreeNode() : parent(nullptr), data() { }

	/**
	 * @brief Constructor with data
	 */
	TreeNode(const DATA_T& data) : parent(nullptr), data(data) { }

	/**
	 * @brief Copy constructor
	 */
	TreeNode(const TreeNode& copied) {
		*this = copied;
	}

	/**
	 * @brief Copy assignment
	 *
	 * @param copied
	 *
	 * @return TreeNode&
	 */
	TreeNode& operator= (const TreeNode& copied) {
		data = copied.data;
		
		for (size_t i = 0; i < copied.children.size(); i++) {
			if (copied.children[i] != nullptr) {
				children.emplace_back(std::make_unique<TreeNode>(*copied.children[i]).release());
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
	 *
	 * @param moved
	 *
	 * @return TreeNode&
	 */
	TreeNode& operator= (TreeNode&& moved) {
		data = std::move(moved.data);
		children = std::move(moved.children);
		return *this;
	}

	/**
	 * @brief Returns a boolean about whether the node has a parent
	 */
	bool hasParent() const {
		return parent != nullptr;
	}

	/**
	 * @brief Returns a reference to the parent node
	 */
	TreeNode& getParent() const {
		return *parent;
	}

	/**
	 * @brief Returns a reference to the node's data member
	 *
	 * @return DATA_T& Reference to the data member
	 */
	DATA_T& getData() {
		return data;
	}

	/**
	 * @brief Sets the node's data to the provided data
	 *
	 * @details
	 * Sets the node's data field to the provided data. This is done by
	 * directly copying the provided parameter. If a value is known by
	 * reference, the DATA_T template of the TreeNode must be a pointer,
	 * since references should not be null-initialized.
	 *
	 * @param data Data to copy into the node's data
	 */
	void setData(const DATA_T& data) {
		this->data = data;
	}

	/**
	 * @brief Iterates over all direct children looking for a node with the provided data
	 *
	 * @details
	 * Iterates over all direct children looking for the first node with the data
	 * that equals the provided data param. If no node was found, a nullptr is
	 * returned.
	 *
	 * @param data The data that is compared with the data of the nodes
	 *
	 * @return TreeNode* Pointer to the matched node
	 * @return nullptr No node was matched
	 */
	TreeNode* findNode(const DATA_T& data) {
		for (size_t i = 0; i < children.size(); i++) {
			if (children[i] != nullptr && children[i]->getData() == data) {
				return children[i];
			}
		}

		return nullptr;
	}

	/**
	 * @brief Recurses through all descendant nodes and returns a pointer when found
	 *
	 * @details
	 * Recurses through all descendant nodes looking for the first node which data
	 * equals the provided data. If no node was found, a nullptr is returned.
	 *
	 * @param data The data that is compared with the data of the nodes
	 *
	 * @return TreeNode* Pointer to the matched node
	 * @return nullptr No node was matched
	 */
	TreeNode* findNodeRecursive(const DATA_T& data) {
		for (size_t i = 0; i < children.size(); i++) {
			if (children[i] != nullptr) {
				if (children[i]->getData() == data) {
					return children[i];
				}
				else {
					return children[i]->findNodeRecursive(data);
				}
			}
		}

		return nullptr;
	}

	/**
	 * @brief Releases ownership from child n and returns an unmanaged pointer
	 *
	 * @details
	 * Use this function when changing parents of nodes. Once this function
	 * is called, the node becomes unmanaged and the responsibility of managing
	 * the memory is for the user.
	 * A nullptr is returned when there is no node known at index n.
	 * Throws an out of range exception when n >= children.size().
	 *
	 * @param n Index of the node
	 *
	 * @return TreeNode<DATA_T, CHIILDREN_COUNT> released node
	 */
	PTR_T<TreeNode>&& releaseNode(size_t n) {
		children.at(n)->parent = nullptr;
		return std::move(children[n]);
	}

	/**
	 * @brief Claims ownership of the provided node
	 *
	 * @details
	 * Use this function when adding a node. Once this function is called,
	 * the node becomes managed by the parenting node and the responsibility
	 * of managing the memory is taken from the user. Which means the node
	 * may not be destructed before it is released using releaseNode(int n).
	 * Throws an exception when the new amount of children exceeds the capacity
	 * provided as template argument children.size().
	 *
	 * @param node Pointer to the TreeNode to claim
	 *
	 * @return int Index of the newly claimed node
	 */
	size_t claimNode(PTR_T<TreeNode>&& node) {
		size_t i = 0;

		for (; i < children.size(); i++) {
			if (children[i] == nullptr) {
				children[i] = std::move(node);
				children[i]->parent = this;
				break;
			}
		}

		if (i == children.size()) {
			children.push_back(std::move(node));
		}

		return i;
	}

	/**
	 * @brief Claims ownership of the provided node at index n
	 *
	 * @details
	 * Use this function when adding a node at the desired index. Once this
	 * function is called, the node becomes managed by the parenting node
	 * and the responsibility of managing the memory is taken from the user.
	 * Which means the node may not be destructed before it is released
	 * using releaseNode(int n).
	 * The node previously known at index n gets detructed, if applicable.
	 * Throws an exception when n is out of range of the bounds determined by
	 * children.size().
	 *
	 * @param node Pointer to the TreeNode to claim
	 * @param n Index to claim the TreeNode at
	 */
	void claimNode(PTR_T<TreeNode>&& node, size_t n) {
		children.at(n) = std::move(node);
		children[n]->parent = this;
	}

	/**
	 * @brief Returns true when a child is present at index n
	 */
	bool hasChild(size_t n) const {
		return children.size() < n && children[n] != nullptr;
	}

	/**
	 * @brief Returns a reference to the child at index n
	 */
	TreeNode& getChild(size_t n) const {
		return *children.at(n);
	}

	iterator begin() {
		return iterator(children.begin(), children.end());
	}

	iterator end() {
		return iterator(children.end());
	}

	const const_iterator begin() const {
		return const_iterator(children.begin(), children.end());
	}

	const const_iterator end() const {
		return const_iterator(children.end());
	}

	friend std::ostream& operator<< (std::ostream& os, const TreeNode& node) {
		node.print(os);
		return os;
	}
};
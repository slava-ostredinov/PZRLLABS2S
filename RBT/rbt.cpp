#include "rbt.h"
#include <iostream>
#include <limits>

BinarySearchTree::Node::Node(Key key, Value value, bool color, Node* parent, Node* left, Node* right) : keyValuePair(key, value), color(color), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node& other) : keyValuePair(other.keyValuePair) {}

bool BinarySearchTree::Node::operator==(const Node& other) const {
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree(int num) const {
    if (left)
        left->output_node_tree(num + 1);

    if (keyValuePair.first == std::numeric_limits<Key>::max())
        return;

    std::cout << "(" << keyValuePair.first << "," << keyValuePair.second << ")" << std::endl;

    if (right)
        right->output_node_tree(num + 1);
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value, Node** root) {
    if (!root)
        return;

    if (key <= keyValuePair.first) {
        if (left) {
            left->insert(key, value, root);
            return;
        }

        left = new Node(key, value, true, this);
        left->insertRebalance(root);
    } 
    else {
        if (right) {
            right->insert(key, value, root);
            return;
        }

        right = new Node(key, value, true, this);
        right->insertRebalance(root);
    }
}

void BinarySearchTree::Node::erase(const Key& key, Node** root) {
    if (!root)
        return;

    if (key != keyValuePair.first) {
        if (left && key <= keyValuePair.first)
            left->erase(key, root);
        else if (right)
            right->erase(key, root);
        return;
    }

    if (left && right) {
        Node* node = right;

        while (node->left)
            node = node->left;

        keyValuePair = node->keyValuePair;
        node->erase(node->keyValuePair.first, root);
        return;
    }

    if (color) {
        if (parent->left == this)
            parent->left = nullptr;
        else
            parent->right = nullptr;

        delete this;
        return;
    }

    Node* child = left ? left : right;

    if (!child) {
        color = false;
        eraseRebalance(root);

        if (parent) {
            if (parent->left == this)
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }

        if (*root == this)
            *root = nullptr;

        delete this;
        return;
    }

    if (parent) {
        if (parent->left == this)
            parent->left = child;
        else
            parent->right = child;
    }

    child->parent = parent;

    if (child->color)
        child->color = false;
    else
        child->eraseRebalance(root);

    if (*root == this)
        *root = child;

    delete this;
}

void BinarySearchTree::Node::rotateLeft() {
    Node* newRoot = right;

    newRoot->parent = parent;

    if (parent) {
        if (parent->left == this)
            parent->left = newRoot;
        else
            parent->right = newRoot;
    }

    right = newRoot->left;

    if (newRoot->left)
        newRoot->left->parent = this;

    parent = newRoot;
    newRoot->left = this;
}

void BinarySearchTree::Node::rotateRight() {
    Node* newRoot = left;

    newRoot->parent = parent;

    if (parent) {
        if (parent->left == this)
            parent->left = newRoot;
        else
            parent->right = newRoot;
    }

    left = newRoot->right;

    if (newRoot->right)
        newRoot->right->parent = this;

    parent = newRoot;
    newRoot->right = this;
}

void BinarySearchTree::Node::insertRebalance(Node** root) {
    if (!parent) {
        color = false;
        *root = this;
        return;
    }

    if (!parent->color)
        return;

    Node* uncle = parent->parent->left == parent ? parent->parent->right : parent->parent->left;

    if (uncle && uncle->color) {
        parent->color = false;
        uncle->color = false;
        parent->parent->color = true;
        parent->parent->insertRebalance(root);
        return;
    }
    Node* node = this;
    if (node == parent->right && parent == parent->parent->left) {
        parent->rotateLeft();
        node = left;
    } 
    else if (node == parent->left && parent == parent->parent->right) {
        parent->rotateRight();
        node = right;
    }

    node->parent->color = false;
    node->parent->parent->color = true;

    if (node == node->parent->left && node->parent == node->parent->parent->left) {
        if (node->parent->parent == *root)
            *root = node->parent;

        node->parent->parent->rotateRight();
    } else {
        if (node->parent->parent == *root)
            *root = node->parent;

        node->parent->parent->rotateLeft();
    }
}

void BinarySearchTree::Node::eraseRebalance(Node** root) {
    if (!parent)
        return;

    Node* sibling = parent->left == this ? parent->right : parent->left;

    if (sibling->color) {
        parent->color = true;
        sibling->color = false;

        if (parent->left == this) {
            if (parent == *root)
                *root = parent->right;

            parent->rotateLeft();
        } else {
            if (parent == *root)
                *root = parent->left;

            parent->rotateRight();
        }
    }

    if (!parent->color && !sibling->color && (!sibling->left || !sibling->left->color) && (!sibling->right || !sibling->right->color)) {
        sibling->color = true;
        parent->eraseRebalance(root);
        return;
    }

    if (parent->color && !sibling->color && (!sibling->left || !sibling->left->color) && (!sibling->right || !sibling->right->color)) {
        sibling->color = true;
        parent->color = false;
        return;
    }

    if (!sibling->color) {
        if(this == parent->left && (!sibling->right || !sibling->right->color) && sibling->left && sibling->left->color) {
            sibling->color = true;
            sibling->left->color = false;
            sibling->rotateRight();
        } else if (this == parent->right && sibling->right && sibling->right->color && (!sibling->left || !sibling->left->color)) {
            sibling->color = true;
            sibling->right->color = false;
            sibling->rotateLeft();
        }

        return;
    }

    sibling->color = parent->color;
    parent->color = false;

    if (this == parent->left) {
        if (sibling->right)
            sibling->right->color = false;

        if (parent == *root)
            *root = parent->right;

        parent->rotateLeft();
    } else {
        if (sibling->left) {
            sibling->left->color = false;
	}

        if (parent == *root) {
            *root = parent->left;
	}

        parent->rotateRight();
    }
}

size_t BinarySearchTree::Node::getMaxHeight() const {
    if (keyValuePair.first == std::numeric_limits<Key>::max()) {
        return 0;
    }

    size_t leftHeight = left ? left->getMaxHeight() : 0;
    size_t rightHeight = right ? right->getMaxHeight() : 0;

    return 1 + std::max(leftHeight, rightHeight);
}

BinarySearchTree::Node* BinarySearchTree::copySubtree(const Node* other) {
    if (!other) {
        return nullptr;
    }

    Node* node = new Node(*other);

    node->left = copySubtree(other->left);
    node->right = copySubtree(other->right);

    return node;
}

void BinarySearchTree::freeSubtree(Node* node) {
    if (!node)
        return;

    freeSubtree(node->left);
    freeSubtree(node->right);

    delete node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : _root(copySubtree(other._root)), _size(other._size) {}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this == &other) {
        return *this;
    }

    freeSubtree(_root);

    _root = copySubtree(other._root);
    _size = other._size;

    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept : _root(other._root), _size(other._size) {
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    std::swap(_root, other._root);
    std::swap(_size, other._size);

    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    freeSubtree(_root);
}

BinarySearchTree::Iterator::Iterator(Node* node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }
    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
	}
        return *this;
    }
    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("Ошибка");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }

    if (_node->left) {
        _node = _node->left;
        while (_node->right) {
            _node = _node->right;
	}
        return *this;
    }
    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("Ошибка");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const {
    return _node->keyValuePair.first == other._node->keyValuePair.first;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const {
    return _node->keyValuePair.first != other._node->keyValuePair.first;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node* node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (!_node) {
        throw std::runtime_error("Ошибка"); 
    }
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node->right) {
        _node = _node->right;

        while (_node->left)
            _node = _node->left;

        return *this;
    }
    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("Ошибка");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node->left) {
        _node = _node->left;
        while (_node->right)
            _node = _node->right;
        return *this;
    }
    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("Ошибка");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}
  


bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const {
    return _node->keyValuePair.first == other._node->keyValuePair.first;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const {
    return _node->keyValuePair.first != other._node->keyValuePair.first;
}

void BinarySearchTree::insert(const Key& key, const Value& value) {
    if (!_root) {
        _root = new Node(key, value);
	_root->right = new Node(std::numeric_limits<Key>::max(), value, false, _root);
	++_size;
        return;
    }

    Node* endNode = end()._node;

    if (endNode->parent) {
        endNode->parent->right = nullptr;
    }
    _root->insert(key, value, &_root);
    ++_size;
    Node* node = _root;
    while (node->right) {
        node = node->right;
    }
    node->right = endNode;
    endNode->parent = node;
}

void BinarySearchTree::erase(const Key& key) {
    if (!_root)
        return;

    while (find(key) != end()) {
        Node* endNode = end()._node;
	endNode->parent->right = nullptr;
	find(key)._node->erase(key, &_root);
	--_size;
	if (!_root) {
            delete endNode;
            return;
        }
	Node* node = _root;
	while (node->right) {
            node = node->right;
	}
        node->right = endNode;
        endNode->parent = node;
    }
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key) {
    Node* node = _root;
    while (node) {
        if (key < node->keyValuePair.first)
            node = node->left;
        else if (key > node->keyValuePair.first)
            node = node->right;
        else
            return Iterator(node);
    }
    return end();
}



BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const {
    const Node* node = _root;
    while (node) {
        if (key < node->keyValuePair.first)
            node = node->left;
        else if (key > node->keyValuePair.first)
            node = node->right;
        else
            return ConstIterator(node);
    }
    return cend();
}




BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node* node = _root;
    while (node->left) {
        node = node->left;
    }
    return Iterator(node);
}



BinarySearchTree::Iterator BinarySearchTree::end() {
    Node* node = _root;
    while (node->right) {
        node = node->right;
    }
    return Iterator(node);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node* node = _root;
    while (node->left) {
        node = node->left;
    }
    return ConstIterator(node);
}


BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node* node = _root;
    while (node->right) {
        node = node->right;
    }
    return ConstIterator(node);
}

size_t BinarySearchTree::size() const {
    return _size;
}


void BinarySearchTree::output_tree() const {
    if (_root) {
        _root->output_node_tree();
    }
}

size_t BinarySearchTree::max_height() const {
    if (!_root)
        return 0;

    return _root->getMaxHeight();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key& key) {
    Iterator finish = end();
    Iterator it = begin();
    while (it != finish && it->first != key) {
        ++it;
    }
    Iterator first = it;
    while (it != finish && it->first == key) {
        ++it;
    }
    return {first, it};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>BinarySearchTree::equalRange(const Key& key) const {
    ConstIterator finish = cend();
    ConstIterator it = cbegin();
    while (it != finish && it->first != key) {
        ++it;
    }
    ConstIterator first = it;
    while (it != finish && it->first == key) {
        ++it;
    }
    return {first, it};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    return cbegin();
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    ConstIterator it = cend();
    --it;
    return it;
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const {
    auto range = equalRange(key);
    ConstIterator it = range.first;
    ConstIterator best = it;
    for (; it != range.second; ++it) {
        if (it->second < best->second)
            best = it;
    }
    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const {
    auto range = equalRange(key);
    ConstIterator it = range.first;
    ConstIterator best = it;
    for (; it != range.second; ++it) {
        if (it->second > best->second)
            best = it;
    }
    return best;
}

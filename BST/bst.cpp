#include "bst.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <limits>

BinarySearchTree::Node::Node(Key key, Value value,
             Node *parent, Node *left,
             Node *right)
{
    keyValuePair.first = key;
    keyValuePair.second = value;
    this->parent = parent;
    this->left = left;
    this->right = right;
}

BinarySearchTree::Node::Node(const Node &other) : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}


bool BinarySearchTree::Node::operator==(const Node &other) const {
    if(keyValuePair == other.keyValuePair) {
        return true;
    }
    return false;
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left) {
        left->output_node_tree();
    }

    if (keyValuePair.first != std::numeric_limits<Key>::max()) {
        std::cout << keyValuePair.first << " : " << keyValuePair.second << std::endl;
    }

    if (right) {
        right->output_node_tree();
    }
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value) {
    if (key < keyValuePair.first) {
        if (left) {
            left->insert(key, value);
            return;
        }
        this->left = new Node(key, value, this);
    } else {
        if (right) {
            right->insert(key, value);
            return;
        }
        this->right = new Node(key, value, this);
    }
}


void BinarySearchTree::Node::erase(const Key& key) {
    if (key != keyValuePair.first) {
        if (key < keyValuePair.first && left != nullptr) {
            left->erase(key);
        } else if (right != nullptr) {
            right->erase(key);
        }
        return;
    }

    if (left != nullptr && right != nullptr) {
        Node* successor = right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }

        this->keyValuePair = successor->keyValuePair;
        successor->erase(successor->keyValuePair.first);
        return;
    }

    Node* childNode = nullptr;
    if (left != nullptr) {
        childNode = left;
    } else {
        childNode = right;
    }

    if (parent != nullptr) {
        if (parent->left == this) {
            parent->left = childNode;
        } else {
            parent->right = childNode;
        }
    }

    if (childNode != nullptr) {
        childNode->parent = parent;
    }
    delete this;
}


BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
    : _size(other._size) {
    if (other._root) {
        _root = new Node(*other._root);
    }
}




BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this != &other) {
        clear(_root);
        _size = other._size;
        if (other._root) {
            _root = new Node(*other._root);
        } else {
            _root = nullptr;
        }
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
    : _size(other._size), _root(other._root) {
    other._size = 0;
    other._root = nullptr;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this != &other) {
        clear(_root);
        _root = other._root;
        _size = other._size;
        other._root = nullptr;
        other._size = 0;
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    clear(_root);
    _root = nullptr;
    _size = 0;
}

BinarySearchTree::Iterator::Iterator(Node *node) {
    _node = node;
}

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

    if (_node->keyValuePair.first == std::numeric_limits<Key>::max() && !_node->right) {
        throw std::runtime_error("Ошибка");
    }

    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
        }
    } else {
        Node* ancestor = _node->parent;
        while (ancestor && _node == ancestor->right) {
            _node = ancestor;
            ancestor = ancestor->parent;
        }
        _node = ancestor;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
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
    } else {
        Node* ancestor = _node->parent;
        while (ancestor && _node == ancestor->left) {
            _node = ancestor;
            ancestor = ancestor->parent;
        }
        _node = ancestor;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) {
    _node = node;
}

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
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }

    if (_node->keyValuePair.first == std::numeric_limits<Key>::max() && !_node->right) {
        throw std::runtime_error("Ошибка");
    }

    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
        }
    } else {
        const Node* ancestor = _node->parent;
        while (ancestor && _node == ancestor->right) {
            _node = ancestor;
            ancestor = ancestor->parent;
        }
        _node = ancestor;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node) {
        throw std::runtime_error("Ошибка");
    }

    if (_node->left) {
        _node = _node->left;
        while (_node->right) {
            _node = _node->right;
        }
    } else {
        const Node* ancestor = _node->parent;
        while (ancestor && _node == ancestor->left) {
            _node = ancestor;
            ancestor = ancestor->parent;
        }
        _node = ancestor;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node != other._node;
}

void BinarySearchTree::clear(Node* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (!_root) {
        _root = new Node(key, value);
        _root->right = new Node(std::numeric_limits<Key>::max(), value, _root);
        ++_size;
        return;
    }

    _root->insert(key, value);
    ++_size;
}

void BinarySearchTree::erase(const Key& key) {
    if (!_root) return;

    while (true) {
        Node* target = _root;
        while (target) {
            if (key < target->keyValuePair.first) {
                target = target->left;
            } else if (key > target->keyValuePair.first) {
                target = target->right;
            } else {
                break;
            }
        }

        if (!target) break;
        if (target->keyValuePair.first == std::numeric_limits<Key>::max()) break;

        if (target == _root) {
            if (!_root->left && !_root->right) {
                delete _root;
                _root = nullptr;
            } else if (_root->right && !_root->left) {
                Node* newRoot = _root->right;
                if (newRoot->keyValuePair.first == std::numeric_limits<Key>::max()) {
                    delete newRoot;
                    delete _root;
                    _root = nullptr;
                } else {
                    newRoot->parent = nullptr;
                    delete _root;
                    _root = newRoot;
                }
            } else if (_root->left && !_root->right) {
                Node* newRoot = _root->left;
                newRoot->parent = nullptr;
                delete _root;
                _root = newRoot;
            } else {
                if (_root->right->keyValuePair.first == std::numeric_limits<Key>::max()) {
                    Node* sentinelNode = _root->right;
                    Node* newRoot = _root->left;
                    newRoot->parent = nullptr;

                    Node* rightmostNode = newRoot;
                    while (rightmostNode->right) {
                        rightmostNode = rightmostNode->right;
                    }
                    rightmostNode->right = sentinelNode;
                    sentinelNode->parent = rightmostNode;

                    delete _root;
                    _root = newRoot;
                } else {
                    Node* successor = _root->right;
                    while (successor->left) {
                        successor = successor->left;
                    }

                    if (successor->parent != _root) {
                        successor->parent->left = successor->right;
                        if (successor->right) {
                            successor->right->parent = successor->parent;
                        }
                        successor->right = _root->right;
                        _root->right->parent = successor;
                    }

                    successor->left = _root->left;
                    if (_root->left) {
                        _root->left->parent = successor;
                    }
                    successor->parent = nullptr;

                    delete _root;
                    _root = successor;
                }
            }
        } else {
            target->erase(key);
        }
        --_size;
    }
}



BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node* current = _root;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            return ConstIterator(current);
        }
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node* current = _root;
    while (current) {
        if (key < current->keyValuePair.first) {
            current = current->left;
        } else if (key > current->keyValuePair.first) {
            current = current->right;
        } else {
            return Iterator(current);
        }
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
    Iterator first = find(key);
    Iterator last = first;
    while (last != end() && last->first == key) {
        ++last;
    }
    return {first, last};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator first = find(key);
    ConstIterator last = first;
    while (last != cend() && last->first == key) {
        ++last;
    }
    return {first, last};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    return cbegin();
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (!_root) {
        return cend();
    }
    return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    ConstIterator it = find(key);
    if (it == cend()) return cend();

    ConstIterator result = it;
    ++it;
    while (it != cend() && it->first == key) {
        if (it->second < result->second) {
            result = it;
        }
        ++it;
    }
    return result;
}



BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    ConstIterator it = find(key);
    if (it == cend()) return cend();

    ConstIterator result = it;
    ++it;
    while (it != cend() && it->first == key) {
        if (it->second > result->second) {
            result = it;
        }
        ++it;
    }
    return result;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    if (!_root) return Iterator(nullptr);
    Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    if (!_root) {
        return Iterator(nullptr);
    }

    Node* current = _root;
    while (current->right) {
        current = current->right;
    }
    return Iterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    if (!_root) return ConstIterator(nullptr);
    const Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    if (!_root) {
        return ConstIterator(nullptr);
    }

    Node* current = _root;
    while (current->right) {
        current = current->right;
    }
    return ConstIterator(current);
}

size_t BinarySearchTree::size() const {
    return _size;
}


void BinarySearchTree::output_tree() {
    if (_root) {
        _root->output_node_tree();
        std::cout << std::endl;
    }
}

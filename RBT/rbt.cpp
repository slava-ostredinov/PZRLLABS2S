#include "rbt.h"
#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>

BinarySearchTree::Node::Node(Key key, Value value, Node* parent, Node* left, Node* right)
    : keyValuePair{key, value}, parent(parent), left(left), right(right), color(false)
{
}

BinarySearchTree::Node::Node(const Node& other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr), color(other.color)
{
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node& other) const
{
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const
{
    if (!this) return;
    
    std::queue<const Node*> q;
    q.push(this);
    
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            const Node* current = q.front();
            q.pop();
            
            if (current) {
                std::cout << "(" << current->keyValuePair.first 
                         << ", " << current->keyValuePair.second << ") ";
                q.push(current->left);
                q.push(current->right);
            } else {
                std::cout << "null ";
            }
        }
        std::cout << std::endl;
    }
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value)
{
    if (key < keyValuePair.first) {
        if (left) {
            left->insert(key, value);
        } else {
            left = new Node(key, value, this);
        }
    } else if (key > keyValuePair.first) {
        if (right) {
            right->insert(key, value);
        } else {
            right = new Node(key, value, this);
        }
    } else {
        keyValuePair.second = value;
    }
}

void BinarySearchTree::Node::erase(const Key& key)
{
    if (key < keyValuePair.first) {
        if (left) left->erase(key);
    } else if (key > keyValuePair.first) {
        if (right) right->erase(key);
    } else {
        Node* toDelete = this;
        
        if (!left && !right) {
            if (parent) {
                if (parent->left == this) parent->left = nullptr;
                else parent->right = nullptr;
            }
            delete this;
        }
        else if (!left || !right) {
            Node* child = left ? left : right;
            child->parent = parent;
            if (parent) {
                if (parent->left == this) parent->left = child;
                else parent->right = child;
            }
            delete this;
        }
        else {
            Node* successor = right;
            while (successor->left) {
                successor = successor->left;
            }
            keyValuePair = successor->keyValuePair;
            successor->erase(successor->keyValuePair.first);
        }
    }
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
    : _size(other._size), _root(nullptr)
{
    if (other._root) {
        _root = new Node(*other._root);
    }
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other)
{
    if (this != &other) {
        this->~BinarySearchTree();
       
        _size = other._size;
        if (other._root) {
            _root = new Node(*other._root);
        } else {
            _root = nullptr;
        }
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept
    : _size(other._size), _root(other._root)
{
    other._size = 0;
    other._root = nullptr;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept
{
    if (this != &other) {
        this->~BinarySearchTree();
        
        _size = other._size;
        _root = other._root;
        
        other._size = 0;
        other._root = nullptr;
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree()
{
    if (_root) {
        std::stack<Node*> nodes;
        nodes.push(_root);
        
        while (!nodes.empty()) {
            Node* current = nodes.top();
            nodes.pop();
            
            if (current->left) nodes.push(current->left);
            if (current->right) nodes.push(current->right);
            
            delete current;
        }
    }
}

BinarySearchTree::Iterator::Iterator(Node* node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*()
{
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const
{
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->()
{
    return &(_node->keyValuePair);
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const
{
    return &(_node->keyValuePair);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    if (!_node) return *this;
    
    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
        }
    } else {
        Node* parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    if (!_node) return *this;
    
    if (_node->left) {
        _node = _node->left;
        while (_node->right) {
            _node = _node->right;
        }
    } else {
        Node* parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    Iterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const
{
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const
{
    return _node != other._node;
}


BinarySearchTree::ConstIterator::ConstIterator(const Node* node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const
{
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const
{
    return &(_node->keyValuePair);
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    if (!_node) return *this;
    
    if (_node->right) {
        _node = _node->right;
        while (_node->left) {
            _node = _node->left;
        }
    } else {
        const Node* parent = _node->parent;
        while (parent && _node == parent->right) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    if (!_node) return *this;
    
    if (_node->left) {
        _node = _node->left;
        while (_node->right) {
            _node = _node->right;
        }
    } else {
        const Node* parent = _node->parent;
        while (parent && _node == parent->left) {
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    ConstIterator temp = *this;
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const
{
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const
{
    return _node != other._node;
}

void BinarySearchTree::insert(const Key& key, const Value& value)
{
    if (!_root) {
        _root = new Node(key, value);
    } else {
        _root->insert(key, value);
    }
    _size++;
}

void BinarySearchTree::erase(const Key& key)
{
    if (_root) {
        size_t oldSize = _size;
        _root->erase(key);
        if (_size != oldSize) {
            _size--;
        }
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const
{
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

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key)
{
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

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key& key)
{
    Iterator first = find(key);
    Iterator last = first;
    
    while (last != end() && last->first == key) {
        ++last;
    }
    
    return {first, last};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key& key) const
{
    ConstIterator first = find(key);
    ConstIterator last = first;
    
    while (last != cend() && last->first == key) {
        ++last;
    }
    
    return {first, last};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    if (!_root) return cend();
    
    Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    if (!_root) return cend();
    
    Node* current = _root;
    while (current->right) {
        current = current->right;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const
{
    Node* node = _root;
    Node* candidate = nullptr;
    
    while (node) {
        if (node->keyValuePair.first == key) {
            candidate = node;
            break;
        } else if (key < node->keyValuePair.first) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    
    return candidate ? ConstIterator(candidate) : cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const
{
    Node* node = _root;
    Node* candidate = nullptr;
    
    while (node) {
        if (node->keyValuePair.first == key) {
            candidate = node;
            break;
        } else if (key < node->keyValuePair.first) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    
    return candidate ? ConstIterator(candidate) : cend();
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
    if (!_root) return end();
    
    Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    if (!_root) return cend();
    
    Node* current = _root;
    while (current->left) {
        current = current->left;
    }
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const
{
    return _size;
}

void BinarySearchTree::output_tree()
{
    if (_root) {
        _root->output_node_tree();
    } else {
        std::cout << "Empty tree" << std::endl;
    }
}

size_t BinarySearchTree::max_height() const
{
    if (!_root) return 0;
    
    std::function<size_t(Node*)> height = [&](Node* node) -> size_t {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    };
    
    return height(_root);
}

#include "hash.h"
#include <stdexcept>


HashTable::HashTable(size_t size) noexcept : _capacity(size), _filled(0), table(size) {}

HashTable::~HashTable() = default;


size_t HashTable::hash_function(const KeyType &key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + static_cast<size_t>(c);
    }
    return hash % _capacity;
}


void HashTable::insert(const KeyType &key, const ValueType &value) {

    if (getLoadFactor() > 0.75) {
        size_t new_capacity = _capacity * 2;
        std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(new_capacity);
        for (auto& bucket : table) {
            for (auto& pair : bucket) {
                size_t new_index = 0;
                for (char c : pair.first) {
                    new_index = new_index * 31 + static_cast<size_t>(c);
                }
                new_index %= new_capacity;
                new_table[new_index].push_back(pair);
            }
        }
        table = std::move(new_table);
        _capacity = static_cast<int32_t>(new_capacity);
    }

    size_t index = hash_function(key);
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    table[index].push_back({key, value});
    _filled++;
}

bool HashTable::find(const KeyType &key, ValueType &value) const {
    size_t index = hash_function(key);

    for (const auto& pair : table[index]) {
        if (pair.first == key) {
            value = pair.second;
            return true;
        }
    }

    return false;
}

void HashTable::remove(const KeyType &key) {
    size_t index = hash_function(key);
    auto& bucket = table[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->first == key) {
            bucket.erase(it);
            _filled--;
            return;
        }
    }
}

ValueType& HashTable::operator[](const KeyType &key) {
    size_t index = hash_function(key);
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    table[index].push_back({key, 0.0});
    _filled++;

    return table[index].back().second;
}

double HashTable::getLoadFactor() {
    return static_cast<double>(_filled) / _capacity;
}

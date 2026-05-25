#include "list.h"
#include <iostream>
#include <stdexcept>

ForwardList::ForwardList() : head(nullptr), _size(0) {}

ForwardList::ForwardList(const ForwardList& other) : head(nullptr), _size(0) {
    Node* current = other.head;
    while (current) {
        push_back(current->num);
        current = current->next;
    }
}

ForwardList& ForwardList::operator=(const ForwardList& other) {
    if (this != &other) {
        clear();
        Node* current = other.head;
        while (current) {
            push_back(current->num);
            current = current->next;
        }
    }
    return *this;
}

ForwardList::ForwardList(ForwardList&& other) noexcept
    : head(other.head), _size(other._size) {
    other.head = nullptr;
    other._size = 0;
}

ForwardList& ForwardList::operator=(ForwardList&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        _size = other._size;
        other.head = nullptr;
        other._size = 0;
    }
    return *this;
}

ForwardList::~ForwardList() {
    clear();
}

void ForwardList::push_front(double n) {
    Node* newNode = new Node{n, head};
    head = newNode;
    _size++;
}

void ForwardList::push_back(double n) {
    Node* newNode = new Node{n, nullptr};
    if (!head) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    _size++;
}

void ForwardList::pop_front() {
    if (!head) {
        throw std::runtime_error("Cannot pop from empty list");
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    _size--;
}

void ForwardList::pop_back() {
    if (!head) {
        throw std::runtime_error("Cannot pop from empty list");
    }

    if (!head->next) {
        delete head;
        head = nullptr;
    } else {
        Node* current = head;
        while (current->next->next) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
    }
    _size--;
}

const double& ForwardList::front() const {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    return head->num;
}

double& ForwardList::front() {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    return head->num;
}

double* ForwardList::find(double n) {
    Node* current = head;
    while (current) {
        if (current->num == n) {
            return &(current->num);
        }
        current = current->next;
    }
    return nullptr;
}

const double* ForwardList::find(double n) const {
    Node* current = head;
    while (current) {
        if (current->num == n) {
            return &(current->num);
        }
        current = current->next;
    }
    return nullptr;
}

size_t ForwardList::size() const {
    return _size;
}

bool ForwardList::empty() const {
    return _size == 0;
}

void ForwardList::display() const {
    Node* current = head;
    std::cout << "[";
    while (current) {
        std::cout << current->num;
        if (current->next) {
            std::cout << ", ";
        }
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

void ForwardList::erase(double n) {
    if (!head) return;

    if (head->num == n) {
        pop_front();
        return;
    }

    Node* current = head;
    while (current->next && current->next->num != n) {
        current = current->next;
    }

    if (current->next) {
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
        _size--;
    }
}

void ForwardList::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    _size = 0;
}

void ForwardList::make_cycle(size_t ind) {
    if (ind >= _size || !head) return;

    Node* target = head;
    for (size_t i = 0; i < ind; i++) {
        target = target->next;
    }

    Node* last = head;
    while (last->next) {
        last = last->next;
    }
    last->next = target;
}

void ForwardList::break_cycle() {
    if (!head) return;

    auto [cycleStart, _] = find_cycle();
    if (!cycleStart) return;

    Node* last = cycleStart;
    while (last->next != cycleStart) {
        last = last->next;
    }
    last->next = nullptr; 
}

std::pair<ForwardList::Node*, size_t> ForwardList::find_cycle() const {
    if (!head) return {nullptr, 0};

    Node* slow = head;
    Node* fast = head;
    bool hasCycle = false;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            hasCycle = true;
            break;
        }
    }

    if (!hasCycle) return {nullptr, 0};

    
    slow = head;
    size_t steps = 0;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
        steps++;
    }

    return {slow, steps};
}

int main() {
    ForwardList list;
    for (int i = 1; i <= 5; i++) {
        list.push_back(i);
    }

    list.make_cycle(2);

    auto [cycleStart, stepsToCycle] = list.find_cycle();

    if (cycleStart) {
        std::cout << "Цикл обнаружен! Начало в узле: " << cycleStart->num << std::endl;
        std::cout << "Шагов до цикла: " << stepsToCycle << std::endl;

        list.break_cycle();
    }

    return 0; 
}

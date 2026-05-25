#pragma once
#include <utility>
#include <cstddef>

class ForwardList{
public:
        struct Node{
                double num;
                Node* next;
        };
        ForwardList();
        ForwardList(const ForwardList& other);
        ForwardList& operator=(const ForwardList& other);
        ForwardList(ForwardList&& other) noexcept;
        ForwardList& operator=(ForwardList&& other) noexcept;
        ~ForwardList();

        void push_front(double n);
        void push_back(double n);
        void pop_front();
        void pop_back();
        const double& front() const;
        double& front();
        double* find(double n);
        const double* find(double n) const;
        size_t size() const;
        bool empty() const;
        void display() const;
        void erase(double n);
        void clear();

        void make_cycle(size_t ind);
        void break_cycle();
        std::pair<Node*, size_t> find_cycle() const;
private:
        Node* head = nullptr;
        size_t _size = 0;
};

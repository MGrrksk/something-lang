#pragma once
#include <vector>

// Dynamic stack implementation with a variable size.
template<typename T>
class DynamicStack {
    std::vector<T> data;
    public:
        void push(const T& element) {
            data.push_back(element);
        }
        T pop() {
            T element = data.back();
            data.pop_back();
            return element;
        }
        T peek(int offset = 0) const {
            return data[data.size() - offset - 1];
        }
        T& operator[](int index) {
            return data[index];
        }
        const T& operator[](int index) const {
            return data[index];
        }
        void clear() {
            data.clear();
        }
};

// Fast stack implementation with a fixed size.
// Warning: No bounds checking is done.
template<typename T, unsigned int size>
class StaticStack {
    T data[size];
    T* top = data;
    public:
        void push(const T& element) {
            *top++ = element;
        }
        T pop() {
            return *top--;
        }
        T peek(int offset = 0) const {
            return *(top - offset - 1);
        }
        T& operator[](int index) {
            return data[index];
        }
        const T& operator[](int index) const {
            return data[index];
        }
        void clear() {
            top = data;
        }
};
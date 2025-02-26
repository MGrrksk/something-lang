#pragma once

// Fast stack implementation with a fixed size.
// Warning: No bounds checking is done.
template<typename T, unsigned int size>
class Stack {
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
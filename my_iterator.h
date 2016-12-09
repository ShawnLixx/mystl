#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

#include <stdlib.h>

namespace mystl {

    //A base _iterator class for containers.
    template<typename T>
    class _iterator {
    public:
        _iterator(T* pointer) {
            this->pointer = pointer;
        }
        //If you don't override +, - operations, you have to write a
        //override of operator=, because those operator in base class
        //return base class. When you use iterator a = a + 1, it'll break.
        _iterator operator=(const _iterator& it) {
            this->pointer = it.pointer;
            return *this;
        }
        virtual _iterator& operator++() {
            ++pointer;
            return *this;
        }
        virtual _iterator operator++(int) {
            _iterator temp = *this;
            ++pointer;
            return temp;
        }
        virtual _iterator& operator--() {
            --pointer;
            return *this;
        }
        virtual _iterator operator--(int) {
            _iterator temp = *this;
            --pointer;
            return temp;
        }
        virtual T& operator*() {
            return *pointer;
        }
        virtual _iterator operator+(size_t n) const {
            return _iterator(pointer + n);
        }
        virtual _iterator operator-(size_t n) const {
            return _iterator(pointer - n);
        }
        virtual size_t operator-(const _iterator& other) const {
            return this->pointer - other.pointer;
        }
        bool operator==(const _iterator& other) const {
            return this->pointer == other.pointer;
        }
        bool operator!=(const _iterator& other) const {
            return this->pointer != other.pointer;
        }
        T* get_pointer() const {
            return pointer;
        }
    protected:
        T* pointer;
    };

    //A base _reverse_iterator class for containers.
    template<typename T>
    class _reverse_iterator: public _iterator<T> {
    public:
        _reverse_iterator(T* pointer): _iterator<T>(pointer) {}
        virtual _reverse_iterator& operator++() {
            --this->pointer;
            return *this;
        }
        virtual _reverse_iterator operator++(int) {
            _reverse_iterator temp = *this;
            --this->pointer;
            return temp;
        }
        virtual _reverse_iterator& operator--() {
            ++this->pointer;
            return *this;
        }
        virtual _reverse_iterator operator--(int) {
            _reverse_iterator temp = *this;
            ++this->pointer;
            return temp;
        }
        virtual size_t operator-(const _reverse_iterator& other) const {
            return other.pointer - this->pointer;
        }
        virtual _reverse_iterator operator+(size_t n) const {
            return _reverse_iterator(this->pointer - n);
        }
        virtual T* operator-(size_t n) const {
            return _reverse_iterator(this->pointer + n);
        }
    };
}

#endif

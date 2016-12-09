#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <stdexcept>
#include "my_allocator.h"
#include "my_iterator.h"

using namespace mystl;

namespace mystl{

    template<typename T, typename Alloc = allocator<T> >
    class vector {
    public:
        typedef Alloc allocator_type;
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef size_t size_type;

        //Iterator implement.
        class iterator: public _iterator<T> {
        public:
            iterator(T* it): _iterator<T>(it) {}
            iterator operator=(const _iterator<T>& it) {
                this->pointer = it.get_pointer();
                return *this;
            }
        };
        typedef const iterator const_iterator;
        //Reverse iterator implement.
        class reverse_iterator: public _iterator<T> {
        public:
            reverse_iterator(T* it): _reverse_iterator<T>(it) {}
        };
        typedef const reverse_iterator const_reverse_iterator;
        
        //Default constructor, construct 0 element.
        explicit vector(const allocator_type& alloc = allocator_type()) {
            _max_size = 0;
            element_num = 0;
        }
        //Construct n elements with value _value.
        explicit vector(size_type n, const_reference _value = value_type(), 
                const allocator_type& alloc = allocator_type()) {
            _max_size = n;
            element_num = n;
            first_element_pointer = alloc.allocate(n);
            alloc.construct(first_element_pointer, n, _value);
        }
        //Range constructor. Construct elements as a copy between two iterator.
        template<typename inputIterator>
        explicit vector(inputIterator first, inputIterator last,
                const allocator_type& alloc = allocator_type()) {
            element_num = last - first;
            _max_size = element_num;
            first_element_pointer = alloc.allocate(element_num);
            alloc.construct(first_element_pointer, first, last);
        }
        //Copy constructor.
        vector(const vector& v) {
            this->element_num = v.element_num;
            this->_max_size = v._max_size;
            first_element_pointer = _allocator.allocate(element_num);
            _allocator.construct(first_element_pointer, v.begin(), v.end());
        }
        //Destructor.
        ~vector() {
            _allocator.destroy(first_element_pointer, element_num);
            _allocator.dealloc(first_element_pointer);
        }

        vector& operator=(const vector& v) {
            _allocator.destroy(first_element_pointer, element_num);
            if (_max_size < v.max_size()) {
                _allocator.dealloc(first_element_pointer);
                _allocator.allocate(v.size());
            }
            _allocator.construct(first_element_pointer, v.begin(), v.end());
        }
        
        
        //Functions about data access.
        //Get first iterator.
        iterator begin() const {
            return iterator(first_element_pointer);
        }
        //Get last iterator(iterator of last element + 1).
        iterator end() const {
            return iterator(first_element_pointer + element_num);
        }
        //Get first reverse iterator.
        reverse_iterator rbegin() const {
            return reverse_iterator(first_element_pointer + element_num - 1);
        }
        //Get last reverse iterator.
        reverse_iterator rend() const {
            return reverse_iterator(first_element_pointer - 1);
        }
        //Get const first reverse iterator.
        const iterator cbegin() const {
            return begin();
        }
        //Get const last reverse iterator.
        const iterator cend() const {
            return end();
        }
        //Get const first reverse iterator.
        const reverse_iterator crbegin() const {
            return rbegin();
        }
        //Get const last reverse iterator.
        const reverse_iterator crend() const {
            return rend();
        }
        
        //Get size.
        size_type size() const {
            return element_num;
        }
        //Get max size.
        size_type max_size() const {
            return _max_size;
        }
        //Resize vector.
        void resize(size_type n, const value_type& value = value_type()) {    
            if (n <= element_num) {
                _allocator.destroy(first_element_pointer + n, element_num - n);
            } else if (n <= _max_size) {
                _allocator.construct(first_element_pointer + element_num, n - element_num, value);  
            } else {
                auto_extend_space(n);
                _allocator.construct(first_element_pointer + element_num, n - element_num, value);
            }
            element_num = n;
        }
        //Check if the vector is empty.
        bool empty() const {
            return element_num == 0;
        }
        //Random access element.
        reference operator[](size_t n) {
            return *(first_element_pointer + n);
        }
        const_reference operator[](size_t n) const {
            return operator[](n);
        }
        //Access element.
        reference at(size_t n) {
            if (n >= element_num) {
                throw std::out_of_range("my_vector access out of range");
            } else {
                return operator[](n);
            }
        }
        const_reference at(size_t n) const {
            return at(n);
        }
        //Get front
        reference front() {
            return *(first_element_pointer);
        }
        const_reference front() const {
            return front();
        }
        //Get back
        reference back() {
            return *(first_element_pointer + element_num - 1);
        }
        const_reference back() const {
            return back();
        }
        //Get allocator.
        allocator_type& get_allocator() {
            return _allocator;
        }
        
        //Operations on vector.
        //Assign values.
        template<typename inputIterator>
        void assign(inputIterator first, inputIterator last) {
            _allocator.destroy(first_element_pointer, element_num);
            size_t n = last - first;
            if (_max_size < n) {
                _allocator.dealloc(first_element_pointer);
                first_element_pointer = _allocator.allocate(n);
                _max_size = last -first;
            }
            _allocator.construct(first_element_pointer, first, last);
            element_num = n;
        }
        //Push element to back.
        void push_back(const value_type& value) {
            if (element_num == _max_size) {
                auto_extend_space(_max_size + 1);
            }
            _allocator.construct(first_element_pointer + element_num, 1, value);
            ++element_num;
        }
        //Pop last element.
        void pop_back() {
            if (!empty()) {
                (first_element_pointer + element_num - 1)->~T();
                --element_num;
            }
        }
        //Insert element at position
        template<typename inputIterator>
        iterator insert(inputIterator position, const value_type& value) {
            size_type pos = position - begin();
            if (element_num == _max_size) {
                auto_extend_space(_max_size + 1);
            }
            position = begin() + pos;
            _allocator.move((position + 1).get_pointer(), position.get_pointer(),
                    end() - position);
            _allocator.construct(position.get_pointer(), 1, value);
            ++element_num;
            return position;
        }
        //Fill insert.
        template<typename inputIterator>
        void insert(inputIterator position, size_type n, const value_type& value) {
            size_type pos = position - begin();
            if (element_num + n > _max_size) {
                auto_extend_space(element_num + n);
            }
            position = begin() + pos;
            _allocator.move((position + n).get_pointer(), position.get_pointer(),
                    n);
            _allocator.construct(position.get_pointer(), n, value);
            element_num += n;
        }
        //Range insert.
        template<typename inputIterator>
        void insert(inputIterator position, inputIterator first, inputIterator last) {
            size_type n = last - first;
            size_type pos = position - begin();
            if (element_num + n > _max_size) {
                auto_extend_space(element_num + n);
            }
            position = begin() + pos;
            _allocator.move((position + n).get_pointer(),
                    position.get_pointer(), n);
            _allocator.reallc(position.get_pointer(), first, last);
            element_num += n;
        }
        //Erase element at position.
        template<typename inputIterator>
        iterator erase(inputIterator position) {
            (position.get_pointer())->~T();
            _allocator.copy(position.get_pointer(), (position + 1).get_pointer(),
                    end() - position() - 1);
            --element_num;
            return position;
        }
        template<typename inputIterator>
        iterator erase(inputIterator first, inputIterator last) {
            size_type n = last - first;
            _allocator.destroy(first.get_pointer(), n);
            _allocator.copy(first.get_pointer(), (first + n).get_pointer(),
                    n);
            element_num -= n;
            return first;
        }
        //Swap contents of two vector.
        void swap(vector& x) {
            //Just need to swap memory of two vector struct, no need to swap array space.
            _allocator.swap(this, &x);
        }
        //Clear vector.
        void clear() {
            _allocator.destroy(first_element_pointer, element_num);
            _allocator.dealloc(first_element_pointer);
            _max_size = 0;
            element_num = 0;
        }
    private:
        //element_num is num of elements stored.
        size_type element_num;
        //_max_size is the size of space the vector has.
        size_type _max_size;
        pointer first_element_pointer;
        allocator_type _allocator;

        //Handle element number overflow.
        void auto_extend_space(size_type required) {
            size_type new_size = _max_size;
            if (0 == _max_size)
                new_size = 1;
            while (new_size < required)
                new_size = new_size << 1;
            pointer temp = _allocator.allocate(new_size);
            _allocator.copy(temp, first_element_pointer, element_num);
            //I don't think we need to destruct all elements of previous space, we only need to free the space.
            //If previous data linked to other space, elements of our new copy may need them, and can use them directly.
            if (0 != _max_size)
                _allocator.dealloc(first_element_pointer);
            first_element_pointer = temp;
            _max_size = new_size;
        }
        
    };
}

#endif

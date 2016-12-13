#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <stdexcept>
#include "my_allocator.h"
#include "my_iterator.h"

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
        typedef T* iterator;
        typedef const T* const_iterator;

        //Reverse iterator implement.
        class reverse_iterator: public _iterator<T> {
        public:
            using typename _iterator<T>::value_type;
            using typename _iterator<T>::pointer;
            using typename _iterator<T>::reference;
            reverse_iterator(T* it): _pointer(it){}
            reverse_iterator& operator++() {
                return --_pointer;
            }
            reverse_iterator operator++(int) {
                reverse_iterator temp(_pointer);
                --_pointer;
                return temp;
            }
            reverse_iterator& operator--() {
                return ++_pointer;
            }
            reverse_iterator operator--(int) {
                reverse_iterator temp(_pointer);
                _pointer;
                return temp;
            }
            reverse_iterator operator=(reverse_iterator it) {
                return mystl_iterator::assign(*this, it);
            }
            reference operator*() {
                return *_pointer;
            }
            reverse_iterator operator+(size_type n) {
                return mystl_iterator::forward_random(*this, n);
            }
            reverse_iterator operator-(size_type n) {
                return mystl_iterator::backward_random(*this, n);
            }
            reverse_iterator operator-(reverse_iterator it) {
                return mystl_iterator::distance(it, *this);
            }
        private:
            pointer _pointer;
        };
        typedef const reverse_iterator const_reverse_iterator;
        
        //Default constructor, construct 0 element.
        explicit vector(const allocator_type& alloc = allocator_type()):
            element_num(0), _capacity(0){
        }
        //Construct n elements with value _value.
        explicit vector(size_type n, const_reference _value = value_type(), 
                const allocator_type& alloc = allocator_type()):
            element_num(n), _capacity(n){
            first_element_pointer = alloc.allocate(n);
            alloc.construct(first_element_pointer, n, _value);
        }
        //Range constructor. Construct elements as a copy between two iterator.
        template<typename inputIterator>
        explicit vector(inputIterator first, inputIterator last,
                const allocator_type& alloc = allocator_type()) {
            element_num = last - first;
            _capacity = element_num;
            first_element_pointer = alloc.allocate(element_num);
            alloc.construct(first_element_pointer, first, last);
        }
        //Copy constructor.
        vector(const vector& v): element_num(v.element_num), _capacity(v._capacity) {
            first_element_pointer = _allocator.allocate(element_num);
            _allocator.construct(first_element_pointer, v.begin(), v.end());
        }
        //Destructor.
        ~vector() {
            _allocator.destroy(first_element_pointer, element_num);
            if (0 != _capacity)
                _allocator.deallocate(first_element_pointer, _capacity);
        }

        vector& operator=(const vector& v) {
            _allocator.destroy(first_element_pointer, element_num);
            if (_capacity < v.capacity()) {
                if (0 != _capacity)
                    _allocator.deallocate(first_element_pointer, _capacity);
                _allocator.allocate(v.capacity());
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
        //Get capacity.
        size_type capacity() const {
            return _capacity;
        }
        //Get max size.
        size_type max_size() const {
            return ((size_t) - 1) / sizeof(T);
        }
        //Resize vector.
        void resize(size_type n, const value_type& value = value_type()) {    
            if (n <= element_num) {
                _allocator.destroy(first_element_pointer + n, element_num - n);
            } else if (n <= _capacity) {
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
            if (_capacity < n) {
                if (0 != _capacity)
                    _allocator.deallocate(first_element_pointer, _capacity);
                first_element_pointer = _allocator.allocate(n);
                _capacity = last -first;
            }
            _allocator.construct(first_element_pointer, first, last);
            element_num = n;
        }
        //Push element to back.
        void push_back(const value_type& value) {
            if (element_num == _capacity) {
                auto_extend_space(_capacity + 1);
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
        iterator insert(iterator position, const value_type& value) {
            size_type pos = position - first_element_pointer;
            if (element_num == _capacity) {
                auto_extend_space(_capacity + 1);
            }
            position = first_element_pointer + pos;
            _allocator.move(position + 1, position, 
                    end() - position);
            _allocator.construct(position, 1, value);
            ++element_num;
            return position;
        }
        //Fill insert.
        void insert(iterator position, size_type n, const value_type& value) {
            size_type pos = position - first_element_pointer;
            if (element_num + n > _capacity) {
                auto_extend_space(element_num + n);
            }
            position = begin() + pos;
            _allocator.move(position + n, position,
                    n);
            _allocator.construct(position, n, value);
            element_num += n;
        }
        //Range insert.
        void insert(iterator position, iterator first, iterator last) {
            size_type n = last - first;
            size_type pos = position - first_element_pointer;
            if (element_num + n > _capacity)
                auto_extend_space(element_num + n);
            position = first_element_pointer + pos;
            _allocator.move(position + n, position, n);
            _allocator.construct(position, first, last);
            element_num += n;
        }
        //Erase element at position.
        iterator erase(iterator position) {
            position->~T();
            if (position != end())
                _allocator.copy(position, position + 1, end() - position - 1);
            --element_num;
            return position;
        }
        iterator erase(iterator first, iterator last) {
            size_type n = last - first;
            if (n <= 0)
                return first;
            _allocator.destroy(first, n);
            _allocator.copy(first, first + n,
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
            if (0 != _capacity)
                _allocator.deallocate(first_element_pointer, _capacity);
            _capacity = 0;
            element_num = 0;
        }
    private:
        //element_num is num of elements stored.
        size_type element_num;
        //_capacity is the size of space the vector has.
        size_type _capacity;
        pointer first_element_pointer;
        allocator_type _allocator;

        //Handle element number overflow.
        void auto_extend_space(size_type required) {
            size_type new_size = _capacity;
            if (0 == _capacity)
                new_size = 1;
            while (new_size < required)
                new_size = new_size << 1;
            pointer temp = _allocator.allocate(new_size);
            _allocator.construct(temp, begin(), end());
            if (0 != _capacity) {
                _allocator.destroy(first_element_pointer, element_num);
                _allocator.deallocate(first_element_pointer, _capacity);
            }
            first_element_pointer = temp;
            _capacity = new_size;
        }
        
    };
}

#endif

#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <stdexcept>
#include "my_memory"
#include "my_algobase.h"
#include "my_iterator.h"

namespace mystl {

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
		typedef mystl::reverse_iterator<iterator> reverse_iterator;
		typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

        //Default constructor, construct 0 element.
        explicit vector(const allocator_type& alloc = allocator_type()):
            _size(0), _capacity(0), _first(0),  _allocator(alloc) {
        }
        //Construct n elements with value _value.
        explicit vector(size_type n, const_reference _value = value_type(), 
                const allocator_type& alloc = allocator_type()):
            _size(n), _capacity(n), _allocator(alloc) {
            _first = _allocator.allocate(n);
            uninitialized_fill_n(_first, n, _value);
        }
        //Range constructor. Construct elements as a copy between two iterator.
        template<typename inputIterator>
        explicit vector(inputIterator first, inputIterator last,
                const allocator_type& alloc = allocator_type()): _allocator(alloc)  {
            _size = last - first;
            _capacity = _size;
            _first = _allocator.allocate(_size);
			uninitialized_copy(first, last, _first);
		}
        //Copy constructor.
        vector(const vector& v): _size(v._size), _capacity(v._capacity) {
            _first = _allocator.allocate(_capacity);
			uninitialized_copy(v.begin(), v.end(), _first);
        }
        //Destructor.
        ~vector() {
			for (size_type i = 0; i < _size; ++i)
				_allocator.destroy(addressof(*(_first + i)));
            if (0 != _capacity)
                _allocator.deallocate(addressof(*_first), _capacity);
        }

        vector& operator=(const vector& v) {
			assign(v.begin(), v.end());
			return *this;
		}
        
        
        //Functions about data access.
        //Get first iterator.
        iterator begin() {
            return _first;
        }
		const_iterator begin() const {
			return _first;
		}
        //Get last iterator(iterator of last element + 1).
        iterator end() {
            return _first + _size;
        }
		const_iterator end() const {
			return _first + _size;
		}
        //Get first reverse iterator.
        reverse_iterator rbegin() {
            return reverse_iterator(_first + _size);
        }
		const_reverse_iterator rbegin() const {
			return rbegin();
		}
        //Get last reverse iterator.
        reverse_iterator rend() {
            return reverse_iterator(_first);
        }
		const_reverse_iterator rend() const {
			return rend();
		}
        
        //Get size.
        size_type size() const {
            return _size;
        }
        //Get capacity.
        size_type capacity() const {
            return _capacity;
        }
        //Get max size.
        size_type max_size() const {
			return _allocator.max_size();
		}
        //Resize vector.
        void resize(size_type n, const value_type& value = value_type()) {    
            if (n <= _size) {
				for (iterator i = _first + n; i < end() - n; ++i)
					_allocator.destroy(addressof(*(i)));
            } else if (n <= _capacity) {
				uninitialized_fill_n(_first + _size, n - _size, value);
            } else {
                auto_extend_space(n);
				uninitialized_fill_n(_first + _size, n - _size, value);
            }
            _size = n;
        }
        //Check if the vector is empty.
        bool empty() const {
            return _size == 0;
        }
        //Random access element.
        reference operator[](size_t n) {
            return *(_first + n);
        }
        const_reference operator[](size_t n) const {
            return operator[](n);
        }
        //Access element.
        reference at(size_t n) {
            if (n >= _size) {
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
            return *(_first);
        }
        const_reference front() const {
            return front();
        }
        //Get back
        reference back() {
            return *(_first + _size - 1);
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
			size_type n = last - first;
			if (_capacity < n) {
				auto_extend_space(n);
			}
			if (_size <= n) {
				copy(first, first + _size, _first);
				uninitialized_copy(first + _size, last, _first + _size);
			} else {
				copy(first, last, _first);
				for (size_type i = n; i < _size; ++i)
					_allocator.destroy(addresso(*(_first + i)));
			}
			_size = n;

		}
        //Push element to back.
        void push_back(const value_type& value) {
            if (_size == _capacity) {
                auto_extend_space(_capacity + 1);
            }
            _allocator.construct(addressof(*((_first + _size))), value);
            ++_size;
        }
        //Pop last element.
        void pop_back() {
            if (!empty()) {
                (_first + _size - 1)->~T();
                --_size;
            }
        }
        //Insert element at position
        iterator insert(iterator position, const value_type& value) {
            size_type pos = position - _first;
            if (_size == _capacity) {
                auto_extend_space(_capacity + 1);
            }
            position = _first + pos;
			if (!empty()) {
				_allocator.construct(addressof(*(_first + _size)), *(_first + _size - 1));
				copy_backwd(position, _first + _size - 1, position + 1);
			}
            *position = value;
            ++_size;
            return position;
        }
        //Fill insert.
        void insert(iterator position, size_type n, const value_type& value) {
            size_type pos = position - _first;
            if (_size + n > _capacity) {
                auto_extend_space(_size + n);
            }
            position = _first + pos;
			if (_size >= n) {
				uninitialized_copy(_first + _size - n, _first + _size, _first + _size);
			copy_backwd(position, _first + _size - n, position + n);
            fill_n(position, n, value);
			} else {
				uninitialized_copy(position, end(), position + n);
				fill(position, end(), value);
				uninitialized_fill(end(), position + n, value);
			}
			_size += n;
        }
        //Range insert.
        void insert(iterator position, iterator first, iterator last) {
            size_type n = last - first;
            size_type pos = position - _first;
            if (_size + n > _capacity)
                auto_extend_space(_size + n);
            position = _first + pos;
			if (_size >= n) {
				uninitialized_copy(_first + _size - n, _first + _size, _first + _size);
			copy_backwd(position, _first + _size - n, position + n);
			copy(first, last, position);
			} else {
				uninitialized_copy(position, end(), position + n);
				copy(first, first + (end() - position), position);
				uninitialized_copy(first + (end() - position), last, end());
			}
			_size += n;
        }
        //Erase element at position.
        iterator erase(iterator position) {
			if (position != end())
                copy(position + 1, end(), position);
			(end() - 1)->~value_type();
            --_size;
            return position;
        }
        iterator erase(iterator first, iterator last) {
            size_type n = last - first;
            if (n <= 0)
                return first;
            copy(last, end(), first);
			for (size_type i = n ; i > 0; --i)
				_allocator.destroy(addressof(*(end() - n)));
            _size -= n;
            return first;
        }
        //Swap contents of two vector.
        void swap(vector& x) {
			if (_capacity < x.size())
				auto_extend_space(x.size());
			if (x.capacity() < _size)
				x.auto_extend_space(_size);
			if (_size == x.size())
				swap_ranges(begin(), end(), x.begin());
			else if (_size < x.size()) {
				swap_ranges(begin(), end(), x.begin());
				uninitialized_copy(x.begin() + _size, x.end(), end());
				for (size_type i = _size; i < x.size(); ++i)
					x.get_allocator().destroy(addressof(*(x.begin() + i)));
			} else {
				swap_ranges(x.begin(), x.end(), begin());
				uninitialized_copy(begin() + x.size(), end(), x.end());
				for (size_type i = x.size(); i < _size; ++i)
					_allocator.destroy(addressof(*(_first + i)));
			}
			mystl::swap(_size, x._size);
		}
		//Resize capacity.
		void reserve(size_type n) {
			extend_space(n);
		}
        //Clear vector.
        void clear() {
            for (size_type i = 0; i < _size; ++i)
				_allocator.destroy(addressof(*(_first + i)));
            _size = 0;
        }
    private:
        //_size is num of elements stored.
        size_type _size;
        //_capacity is the size of space the vector has.
        size_type _capacity;
        pointer _first;
        allocator_type _allocator;

        //Handle element number overflow.
        void auto_extend_space(size_type required) {
            size_type new_size = _capacity;
            if (0 == _capacity)
                new_size = 1;
            while (new_size < required)
                new_size = new_size << 1;
			extend_space(new_size);
		}
		//Extend space.
		void extend_space(size_type n) {
			iterator temp(_allocator.allocate(n));
			if (0 != _capacity) {
				copy(_first, end(), temp);
				for (size_type i = 0; i < _size; ++i)
					_allocator.destroy(_first + i);
                _allocator.deallocate(_first, _capacity);
            }
            _first = temp;
            _capacity = n;

		}
        
    };

	//Functions.
	template<class T, class Alloc>
	bool operator==(const vector<T, Alloc>& l, const vector<T, Alloc>& r) {
		if (l.size() != r.size())
			return false;
		for (typename vector<T, Alloc>::size_type i = 0; i < l.size(); ++i) {
			if (l[i] != r[i])
				return false;
		}
		return true;
	}
	template<class T, class Alloc>
	bool operator!=(const vector<T, Alloc>& l, const vector<T, Alloc>& r) {
		return !(l == r);
	}
	template<class T, class Alloc>
	bool operator<(const vector<T, Alloc>& l, const vector<T, Alloc>& r) {
		typename vector<T, Alloc>::size_type i = 0;
		for ( ; i < l.size(); ++i) {
			if (i == r.size())
				return false;
			else if (l[i] == r[i])
				continue;
			else
				return l[i] < r[i];
		}
		if (i == r.size())
			return false;
		else
			return true;
	}
	template<class T, class Alloc>
	bool operator<=(const vector<T, Alloc>& l, const vector<T, Alloc>& r) {
		for (typename vector<T, Alloc>::size_type i = 0; i < l.size(); ++i) {
			if (i == r.size())
				return false;
			else if (l[i] == r[i])
				continue;
			else
				return l[i] < r[i];
		}
		return true;
	}
	template<class T, class Alloc>
	bool operator>(const vector<T, Alloc>& l, const vector<T, Alloc>& r) {
		for (typename vector<T, Alloc>::size_type i = 0; i < l.size(); ++i) {
			if (i == r.size())
				return true;
			else if (l[i] == r[i])
				continue;
			else
				return l[i] > r[i];
		}
		return false;
	}
	template<class T, class Alloc>
	bool operator>=(const vector<T, Alloc>& l, const vector<T, Alloc>& r) {
		typename vector<T, Alloc>::size_type i = 0;
		for ( ; i < l.size(); ++i) {
			if (i == r.size())
				return true;
			else if (l[i] == r[i])
				continue;
			else
				return l[i] > r[i];
		}
		if (i == r.size())
			return true;
		else
			return false;
	}
	//Swap two vector.
	template<class T, class Alloc>
	void swap(vector<T, Alloc>& l, vector<T, Alloc>& r) {
		l.swap(r);
	}

}

#endif

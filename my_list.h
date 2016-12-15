#ifndef MY_LIST_H
#define MY_LIST_H

#include "my_memory"
#include "my_algobase.h"
#include "my_iterator.h"

namespace mystl {
	//List node.
	template <class T>
	class node {
	public:
		T val;
		node* next;
		node* prev;
	};

	template<typename T, typename Alloc = allocator<T> >
	class list {
	public:
		typedef Alloc allocator_type;
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef size_t size_type;
		typedef node<T> node_type;
		typedef node<T>* link_type;

		//Iterator implement.
		class iterator: public mystl::iterator<bidirectional_iterator_tag, T> {
		public:
			typedef node<T>* link_type;

			iterator() {}
			iterator(link_type x): _node(x) {}
			iterator(const iterator& x): _node(x._node) {}
			iterator& operator=(const iterator& x) {
				_node = x._node;
				return *this;
			}
			bool operator==(const iterator& x) const {
				return _node == x._node;
			}
			bool operator!=(const iterator& x) const {
				return _node != x._node;
			}
			reference operator*() const {
				return _node->val;
			}
			pointer operator->() const {
				return &_node->val;
			}
			iterator& operator++() {
				_node = _node->next;
				return *this;
			}
			iterator operator++(int) {
				iterator temp(_node);
				_node = _node->next;
				return temp;
			}
			iterator& operator--() {
				_node = _node->prev;
				return *this;
			}
			iterator operator--(int) {
				iterator temp(_node);
				_node = _node->prev;
				return temp;
			}
			friend class list;
		private:
			link_type _node;
		};

		typedef const iterator const_iterator;
		
		typedef mystl::reverse_iterator<iterator> reverse_iterator;
		typedef const mystl::reverse_iterator<iterator> const_reverse_iterator;
		
		//Constructors.
		explicit list(const allocator_type& alloc = allocator_type()): 
			_allocator(alloc) {
			_node = alloc_node();
			_node->next = _node;
			_node->prev = _node;
		}
		explicit list(size_type n, const value_type& val = value_type(), 
				const allocator_type& alloc = allocator_type()):
			_allocator(alloc) {
			_node = alloc_node();
			_node->next = _node;
			_node->prev = _node;
			insert(end(), n, val);
		}
		template<class InputIterator>
		list(InputIterator first, InputIterator last, 
				const allocator_type& alloc = allocator_type()):
			_allocator(alloc) {
			_node = alloc_node();
			_node->next = _node;
			_node->prev = _node;
			insert(end(), first, last);
		}
		//Destructor.
		~list() {
			erase(begin(), end());
			dealloc_node(_node);
		}

		//Copy.
		list& operator=(const list& x) {
			erase(begin(), end());
			insert(begin(), x.begin(), x.end());
		}
		
		//Data access.
		iterator begin() {
			return iterator(_node->next);
		}
		const_iterator begin() const {
			return iterator(_node->next);
		}
		iterator end() {
			return iterator(_node);
		}
		const_iterator end() const {
			return iterator(_node);
		}
		iterator rbegin() {
			return reverse_iterator(end());
		}
		const_iterator rbegin() const {
			return reverse_iterator(end());
		}
		iterator rend() {
			return reverse_iterator(begin());
		}
		const_iterator rend() const {
			return reverse_iterator(begin());
		}
		bool empty() const {
			return size() == 0;
		}
		size_type size() const {
			return distance(begin(), end());
		}
		size_type max_size() const {
			return node_allocator.max_size();
		}
		reference front() {
			return *begin();
		}
		const_reference front() const {
			return front();
		}
		reference back() {
			return *(--end());
		}
		const_reference back() const {
			return back();
		}

		//Operations.
		template<class InputIterator>
		void assign(InputIterator first, InputIterator last) {
			erase(begin(), end());
			insert(begin(), first, last);
		}
		void assgin(size_type n, const value_type& value) {
			erase(begin(), end());
			insert(begin(), n, value);
		}
		void push_front(const value_type& value) {
			insert(end(), value);
		}
		void pop_front() {
			erase(begin());
		}
		void push_back(const value_type& value) {
			insert(--end(), value);
		}
		void pop_back() {
			erase(--end());
		}

		//Swap two list.
		void swap(list& x) {
			link_type temp = x._node;
			x._node = _node;
			_node = temp;
		}
		
		//Resize list.
		void resize(size_type n, value_type value = value_type()) {
			size_type _size = size();
			if (_size > n) {
				iterator temp(begin());
				advance(temp, n);
				erase(temp, end());
			} else {
				insert(--end(), n - _size, value);
			}
		}

		//Insert node to position.
		iterator insert(iterator position, const value_type& value) {
			link_type it = alloc_node();
			_allocator.construct(&(it->val), value);
			link_type temp = position._node->next;
			position._node->next = it;
			it->next = temp;
			temp->prev = it;
			it->prev = position._node;
			return iterator(it);
		}
		iterator insert(iterator position, size_type n, const value_type value) {
			for ( ; n > 0; --n) {
				insert(position, value);
			}
			return position;
		}
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last) {
			iterator temp(position);
			for ( ; first != last; ++first) {
				insert(temp, *first);
				++temp;
			}
			return position;
		}

		//Erase node of position.
		iterator erase(iterator position) {
			position._node->prev->next = position._node->next;
			position._node->next->prev = position._node->prev;
			iterator temp(position._node->next);
			_allocator.destroy(addressof(*position));
			dealloc_node(position._node);
			return temp;
		}
		iterator erase(iterator first, iterator last) {
			while (first != last)
				first = erase(first);
		}

		//Clear contents.
		void clear() {
			erase(begin(), end());
		}
		//Splice objects from other list to position.
		void splice(iterator position, list& x) {
			splice(position, x, x.begin(), x.end());
		}
		void splice(iterator position, list& x, iterator i) {
			if (i == x.end())
				return;
			link_type temp = position._node->next;
			position._node->next = i._node;
			i._node->prev = position._node;
			i._node->next = temp;
			temp->prev = i._node;
		}
		void splice(iterator position, list& x, iterator first, iterator last) {
			link_type temp = position._node->next;
			position._node->next = first._node;
			first._node->prev = position._node;
			iterator de = last;
			--de;
			de._node->next = temp;
			temp->prev = de._node;
		}
		void remove(const value_type& value) {
			for (iterator i = begin(); i != end(); ++i) {
				if (*i == value) {
					iterator temp = i;
					--temp;
					erase(i);
					i = temp;
				}
			}
		}

		//Remain unique, sort, merge, reverse unfinished.
		private:
			//A pointer to a node without value.
			link_type _node;
			allocator_type _allocator;
			allocator<node_type> node_allocator;

			link_type alloc_node() {
				return node_allocator.allocate(1);
			}
			void dealloc_node(link_type n) {
				return node_allocator.deallocate(n, 1);
			}
		
	};

	//Remain relational operator unfinished.
	template<class T, class Alloc>
	void swap(list<T, Alloc>& l, list<T, Alloc>& r) {
		l.swap(r);
	}
}

#endif

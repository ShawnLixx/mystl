#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

#include <stddef.h>
#include "my_algobase.h"

namespace mystl {

    //Iterator tags
    class input_iterator_tag {};
    class output_iterator_tag {};
    class forward_iterator_tag: public input_iterator_tag{};
    class bidirectional_iterator_tag: public forward_iterator_tag {};
    class random_access_iterator_tag: public bidirectional_iterator_tag {};

    //A base iterator class for containers.
    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    class iterator {
    public:
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
        
    };

    //For traits.
    template<class Iterator>
    class iterator_traits {
    public:
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };
    
    //Special for native pointer.
    template<class T>
    class iterator_traits<T*> {
    public:
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    //Special for native pointer to const.
    template<class T>
    class iterator_traits<const T*> {
    public:
        typedef random_access_iterator_tag iterator_category;
        //In my opinion, value_type should be const T.
        typedef const T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
    };

    //A base reverse_iterator class for containers.
    template<class Iterator>
    class reverse_iterator: public iterator<typename iterator_traits<Iterator>::iterator_category,
            typename iterator_traits<Iterator>::value_type,
            typename iterator_traits<Iterator>::difference_type,
            typename iterator_traits<Iterator>::pointer,
            typename iterator_traits<Iterator>::reference > {
    public:
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type value_type;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        typedef typename iterator_traits<Iterator>::pointer pointer;
        typedef typename iterator_traits<Iterator>::reference reference;

        //Constructors.
        reverse_iterator() {}
        explicit reverse_iterator(Iterator it): current(it) {}
        template<class U>
        reverse_iterator(const reverse_iterator<U>& it): current(it){}

        //Assign
        template<class U>
        reverse_iterator& operator=(const reverse_iterator<U>& it) {
            current = it.current;
        }

        //Return base iterator.
        Iterator base() const {
            return current;
        }

        //Operators override.
        reference operator*() const {
            Iterator temp(current);
            return *(--temp);
        }
        pointer operator->() const {
            return addressof(operator*());
        }
        reverse_iterator& operator++() {
            --current;
            return *this;
        }
        reverse_iterator operator++(int) {
            reverse_iterator temp(*this);
            --current;
            return temp;
        }
        reverse_iterator& operator--() {
            ++current;
            return *this;
        }
        reverse_iterator operator--(int) {
            reverse_iterator temp(*this);
            ++current;
            return temp;
        }
        reverse_iterator operator+(difference_type n) const {
            return reverse_iterator(base() - n);
        }
        reverse_iterator operator-(difference_type n) const {
            return reverse_iterator(base() + n);
        }
        reverse_iterator& operator+=(difference_type n) {
            current -= n;
            return *this;
        }
        reverse_iterator& operator-=(difference_type n) {
            current += n;
            return *this;
        }
    protected:
        Iterator current;
    };

    //Functions.
    
    //Distance for input_iterator.
    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
		for ( ; first != last; ++first)
            ++n;
        return n;
    }
    //Distance for random_access_iterator.
    template<class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    _distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
        return first - last;
    }
    //Distance for general.
    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type
    distance(Iterator first, Iterator last) {
		return _distance(first, last, typename iterator_traits<Iterator>::iterator_category());
    }

    //Advance for input_iterator.
    template<class InputIterator, class Distance>
    inline void _advance(InputIterator& it, Distance n, input_iterator_tag) {
        for (Distance i = 0; i < n; ++i)
            ++it;
    }
    //Advance for bidirectional_iterator.
    template<class BidrectionalIterator, class Distance>
    inline void _advance(BidrectionalIterator& it, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            for (Distance i = 0; i < n; ++i)
                ++it;
        } else {
            for (Distance i = 0; i > n; --i)
                --it;
        }
    }
    //Advance for random_access_iterator.
    template<class RandomAccessIterator, class Distance>
    inline void _advance(RandomAccessIterator& it, Distance n, random_access_iterator_tag) {
        it += n;
    }
    //Advance for general.
    template<class Iterator, class Distance>
    inline void advance(Iterator& it, Distance n) {
        _advance(it, n, typename iterator_traits<Iterator>::iterator_category());
    }

    //Functions for reverse_iterator.
    template<class Iterator1, class Iterator2>
    bool operator==(const reverse_iterator<Iterator1>& l, const reverse_iterator<Iterator2>& r) {
        return l.base() == r.base();
    }
    template<class Iterator1, class Iterator2>
    bool operator!=(const reverse_iterator<Iterator1>& l, const reverse_iterator<Iterator2>& r) {
        return l.base() != r.base();
    }
    template<class Iterator1, class Iterator2>
    bool operator<(const reverse_iterator<Iterator1>& l, const reverse_iterator<Iterator2>& r) {
        return l.base() < r.base();
    }
    template<class Iterator1, class Iterator2>
    bool operator>(const reverse_iterator<Iterator1>& l, const reverse_iterator<Iterator2>& r) {
        return l.base() > r.base();
    }
    template<class Iterator1, class Iterator2>
    bool operator<=(const reverse_iterator<Iterator1>& l, const reverse_iterator<Iterator2>& r) {
        return l.base() <= r.base();
    }
    template<class Iterator1, class Iterator2>
    bool operator>=(const reverse_iterator<Iterator1>& l, const reverse_iterator<Iterator2>& r) {
        return l.base() >= r.base();
    }




}

#endif

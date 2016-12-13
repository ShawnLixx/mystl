#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

#include <stddef.h>

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
        typedef Category iteraotr_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
        
    };

    //For traits.
    template<class Iterator>
    class iterator_traits {
        typedef typename Iterator::iteraotr_category iteraotr_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };
    
    //Special for native pointer.
    template<class T>
    class iterator_traits<T*> {
        typedef random_access_iterator_tag iteraotr_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    //Special for native pointer to const.
    template<class T>
    class iterator_traits<const T*> {
        typedef random_access_iterator_tag iterator_category;
        //In my opinion, value_type should be const T.
        typedef const T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
    };

    //Functions.
    
    //Distance for input_iterator.
    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n;
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
        return _distance(first, last, iterator_traits<Iterator>::iterator_category());
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
        _advance(it, n, iterator_traits<Iterator>::iteraotr_category());
    }

}

#endif

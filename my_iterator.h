#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

#include <stdlib.h>

namespace mystl {

    //A base _iterator class for containers.
    template<typename T, typename Pointer = T*, typename Reference = T&>
    class _iterator {
    public:
        //Should be declared in subclass.
        typedef T value_type;
        typedef Pointer pointer;
        typedef Reference reference;

        _iterator(pointer _pointer) {
            this->_pointer = _pointer;
        }
        
        
        template<typename InputIterator1, typename InputIterator2>
        friend InputIterator1 assign(InputIterator1, InputIterator2);
        
        pointer get_pointer() const {
            return _pointer;
        }
    protected:
        pointer _pointer;
    };

}

namespace mystl_iterator {
    //Functions of iterators.

    //Get forward n of iterator.
    template<typename InputIterator>
    InputIterator forward(InputIterator it, size_t n = 1) {
        for (size_t i = 0; i < n; ++i) {
            ++it;
        }
        return it;
    }
    template<typename InputIterator>
    InputIterator forward_random(InputIterator it, size_t n = 1) {
        return it + n;
    }
    //Get backward n of iterator.
    template<typename InputIterator>
    InputIterator backward(InputIterator it, size_t n = 1) {
        for (size_t i = 0; i < n; ++i) {
            --it;
        }
        return it;
    }
    template<typename InputIterator>
    InputIterator backward_random(InputIterator it, size_t n = 1) {
        return it - n;
    }
    //Get distance between two interator, second param is bigger than the first.
    template<typename InputIterator>
    size_t distance(InputIterator it1, InputIterator it2) {
        size_t n = 0;
        for ( ; it1 != it2; ++it1)
            ++n;
        return n;
    }
    template<typename InputIterator>
    size_t distance_random(InputIterator it1, InputIterator it2) {
        return it2 - it1;
    }
    template<typename InputIterator1, typename InputIterator2>
    InputIterator1 assign(InputIterator1 it1, InputIterator2 it2) {
        it1.pointer = it2.pointer;
        return it1;
    }
}

#endif

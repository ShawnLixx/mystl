#ifndef MY_UNINITIALIZED_H
#define MY_UNINITIALIZED_H

#include "my_iterator.h"

namespace mystl {
    
    //Copy construct object between input iterator to result.
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
            ForwardIterator result) {
        for ( ; first != last; ++first) {
            new(&*result) typename iterator_traits<ForwardIterator>::value_type(*first);
            ++result;
        }
        return result;
    }
    //Fill allocated space between two iterator with value,
    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last,
            const T& value) {
        for ( ; first != last; ++first) {
            new(&*first) typename iterator_traits<ForwardIterator>::value_type(value);
        }
    }
    //Fill allocated space with n values.
    template<class ForwardIterator, class Size, class T>
    void uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
        for ( ; n > 0; --n) {
            new(&*first) typename iterator_traits<ForwardIterator>::value_type(value);
            first++;
        }
    }
}

#endif

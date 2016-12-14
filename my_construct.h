#ifndef MY_CONSTRUCT_H
#define MY_CONSTRUCT_H

#include <new>

namespace mystl {

    //Construct in allocated space.
    template<class T1, class T2>
    void _construct(T1* p, const T2& value) {
        new(p) T1(value);
    }
    
    //Destory object.
    template<class T>
    void _destroy(T* p) {
        p->~T();
    }
}

#endif

#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <new>
#include <stdlib.h>
#include <string.h>
#include "my_iterator.h"

using namespace mystl;

namespace mystl {
    
    template<typename T>
    class allocator {
    public:
        
        //Allocate space of n * size of T, if failed throw bad_alloc.
        T* allocate(size_t n) {
            void* p = ::operator new(n * sizeof(T));
            if (0 == p) {
                throw std::bad_alloc();
            }
            return (T*) p;
        }
        //Recall space from p pointing to n T elements.
        void dealloc(T* p) {
            delete p;
        }
        //Construct n T elements, call T's default constructor.
        void construct(T* p, size_t n) {
            new(p) T[n];
        }
        //Construct n T elements, call T's copy constructor.
        void construct(T* p, size_t n, const T& value) {
            for(size_t i = 0; i < n; ++i) {
                new(p + i) T(value);
            }
        }
        //Construct elements copy between two iterator.
        template<typename inputIterator>
        void construct(T* p, inputIterator first, inputIterator last) {
            size_t n = last - first;
            for(size_t i = 0; i < n; ++i) {
                new(p + i) T(*(first + i));
            }
        }
        //If T == int, do not call function above!
        void construct(int* p, int n, int value) {
            for(size_t i = 0; i < n; ++i) {
                new(p + i) int(value);
            }
        }

        //Destruct elements.
        void destroy(T* p, size_t n) {
            for (size_t i = 0; i < n; ++i) {
                (p + i)->~T();
            }
        }
        //Copy content, from start to end.
        void copy(T* des, void* sou, size_t n) {
            memcpy(des, sou, n * sizeof(T));
        }
        //Also copy content, but can from end to start, deal with overlapping.
        void move(T* des, void* sou, size_t n) {
            memmove(des, sou, n * sizeof(T));
        }
        //Swap swap memory of two block.
        template<typename SWAP_TYPE>
        void swap(SWAP_TYPE* p1, SWAP_TYPE* p2) {
            SWAP_TYPE* temp = malloc(sizeof(SWAP_TYPE));
            memcpy(temp, p1, sizeof(SWAP_TYPE));
            memcpy(p1, p2, sizeof(SWAP_TYPE));
            memcpy(p2, temp, sizeof(SWAP_TYPE));
            free(temp);
        }
    };
}



#endif

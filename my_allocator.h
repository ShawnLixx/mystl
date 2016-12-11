#ifndef MY_ALLOCAvalue_typeOR_H
#define MY_ALLOCAvalue_typeOR_H

#include <new>
#include <stdlib.h>
#include <string.h>
#include "my_iterator.h"

using namespace mystl;

namespace mystl {
    
    template<typename T>
    class allocator {
    public:
        typedef T value_type;
        typedef const T const_value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        //Allocate space of n * size of value_type, if failed throw bad_alloc.
        pointer allocate(size_type n) {
            void* p = ::operator new(n * sizeof(value_type));
            if (0 == p) {
                throw std::bad_alloc();
            }
            return (pointer) p;
        }
        //Recall space from p pointing to n value_type elements.
        void dealloc(pointer p, size_type) {
            delete p;
        }
        //Construct n value_type elements, call value_type's default constructor.
        void construct(pointer p, size_type n = 1) {
            new(p) value_type[n];
        }
        //Construct n value_type elements, call value_type's copy constructor.
        void construct(pointer p, size_type n, const_reference value) {
            for(size_type i = 0; i < n; ++i) {
                new(p + i) value_type(value);
            }
        }
        //Construct elements copy between two iterator.
        template<typename inputIterator>
        void construct(pointer p, inputIterator first, inputIterator last) {
            size_type n = last - first;
            for(size_type i = 0; i < n; ++i) {
                new(p + i) value_type(*(first + i));
            }
        }
        //If value_type == int, do not call function above!
        void construct(int* p, int n, int value) {
            for(size_type i = 0; i < n; ++i) {
                new(p + i) int(value);
            }
        }

        //Destruct elements.
        void destroy(pointer p, size_type n) {
            for (size_type i = 0; i < n; ++i) {
                (p + i)->~value_type();
            }
        }
        //Copy content, from start to end.
        void copy(pointer des, void* sou, size_type n) {
            memcpy(des, sou, n * sizeof(value_type));
        }
        //Also copy content, but can from end to start, deal with overlapping.
        void move(pointer des, void* sou, size_type n) {
            memmove(des, sou, n * sizeof(value_type));
        }
        //Swap swap memory of two block.
        template<typename SWAP_value_typeYPE>
        void swap(SWAP_value_typeYPE* p1, SWAP_value_typeYPE* p2) {
            SWAP_value_typeYPE* temp = malloc(sizeof(SWAP_value_typeYPE));
            memcpy(temp, p1, sizeof(SWAP_value_typeYPE));
            memcpy(p1, p2, sizeof(SWAP_value_typeYPE));
            memcpy(p2, temp, sizeof(SWAP_value_typeYPE));
            free(temp);
        }
    };
}



#endif

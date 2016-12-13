#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <stddef.h>
#include <stdexcept>
#include "my_construct.h"

namespace mystl {
    
    template<class T>
    class allocator {
    public:
        typedef T value_type;
        typedef const T const_value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        
        template<class U>
        class rebind {
        public:
            typedef allocator<U> allocator;
        };

        //Contructors
        allocator() {}
        allocator(const allocator&) {}
        template<class U>
        allocator(const allocator<U>&) {}

        //Destructor
        ~allocator() {}
        
        //Get address.
        pointer address(reference x) const {
            return &x;
        }
        const_pointer address(const_reference x) const {
            return (const_pointer)&x;
        }

        //Allocate space of n * size of value_type, if failed throw bad_alloc.
        const_pointer allocate(size_type n, const void* = 0) {
            void* p = ::operator new(n * sizeof(value_type));
            if (0 == p) {
                throw std::bad_alloc();
            }
            return (pointer) p;
        }
        //Recall space from p pointing to n value_type elements.
        void deallocate(pointer p, size_type) {
            ::operator delete(p);
        }
        //Construct a element with value x.
        void construct(pointer p, const_reference x) {
            _construct(p, x);
        }
        //Destruct elements.
        void destroy(pointer p) {
            _destory(p);
        }
        
        //Get max_size.
        size_type max_size() const {
            return ((size_type)-1) / sizeof(value_type);
        }
    };
}



#endif

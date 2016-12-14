#ifndef MY_ALGOBASE_H
#define MY_ALGOBASE_H

namespace mystl {

    //Copy content between two iterator.
    template<class InputIterator, class OutputIterator>
    OutputIterator copy(InputIterator first, InputIterator last, OutputIterator des) {
        for ( ; first != last; ++first) {
            *des = *first;
            ++des;
        }
        return des;
    }
    //Copy from back.
    template<class BidirectionalIterator1, class BidirectionalIterator2>
    BidirectionalIterator2 copy_backwd(BidirectionalIterator1 first, BidirectionalIterator1 last,
            BidirectionalIterator2 des_last) {
        while (last != first) {
            *(--des_last) = *(--last);
        }
        return des_last;
        
    }
    //Fill space with value.
    template<class ForwardIterator, class T>
    void fill(ForwardIterator first, ForwardIterator last, const T& value) {
        for ( ; first != last; ++first) {
            *first = value;
        }
    }
    //Fill sapce with n value.
    template<class OutputIterator, class Size, class T>
    void fill_n(OutputIterator first, Size n, const T& value) {
        for ( ; n > 0; n--) {
            *first = value;
            ++first;
        }
    }

    //Get address.
    template<class T>
    T* addressof(T& value) {
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value)));
    }

	//Swap two object.
	template<class T>
	void swap(T& a, T& b) {
		T temp(a);
		a = b;
		b = temp;
	}
	//Swap objects in range.
	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
		for ( ; first1 != last1; ++first1) {
			swap(*first1, *first2);
			++first2;
		}
		return first2;
	}
}

#endif

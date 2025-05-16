#ifndef __ALGOBASE_H__
#define __ALGOBASE_H__

#include "stl_iterator_base.h"
#include <utility>

template<typename InputIterator1, typename InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (*first1 != *first2) {
            return false;
        }
    }
    return true;
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!binary_pred(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first) {
        *first = value;
    }
}

template<typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
    for (; n > 0; --n, ++first) {
        *first = value;
    }
    return first;
}

template<typename ForwardIterator1, typename ForwardIterator2, typename T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
    // typename iterator_tratis<ForwardIterator1>::value_type tmp = *a;
    // *a = *b;
    // *b = tmp;
    __iter_swap(a, b, value_type(a));
}

template<typename T>
inline const T& max(const T& a, const T& b)
{
    return b < a ? a : b;
}

template<typename T, typename Compare>
inline const T& max(const T& a, const T& b, Compare comp)
{
    return comp(b, a) ? a : b;
}

template<typename T>
inline const T& min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template<typename T, typename Compare>
inline const T& min(const T& a, const T& b, Compare comp)
{
    return comp(a, b) ? a : b;
}

// TODO
template<typename InputIterator1, typename InputIterator2>
std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
{
    while (first1 != last1 && binary_pred(*first1, *first2)) {
        ++first1;
        ++first2;
    }
    return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename T>
inline void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) {
            return true;
        }
        if (*first2 < *first1) {
            return false;
        }
    }

    return first1 == last1 && first2 != last2;
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2)) {
            return true;
        }
        if (comp(*first2, *first1)) {
            return false;
        }
    }

    return first1 == last1 && first2 != last2;
} 



#endif
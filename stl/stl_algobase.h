#ifndef __ALGOBASE_H__
#define __ALGOBASE_H__

#include "stl_iterator_base.h"
#include "type_traits.h"
#include "stl_pair.h"
#include <cstring>

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
    T tmp   = a;
    a       = b;
    b       = tmp;
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
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
{
    while (first1 != last1 && binary_pred(*first1, *first2)) {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename T>
inline void swap(T& a, T& b)
{
    T tmp   = a;
    a       = b;
    b       = tmp;
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

template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
{
    // 以 n 决定循环的执行次数, 速度快
    for (Distance n = last - first; n > 0; --n, ++result, ++first) {
        *result = *first;
    }
    return result;
}

template<typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template<typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
    return __copy_d(first, last, result, (ptrdiff_t*) nullptr);
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
    // 以迭代器等同与否, 决定循环是否继续, 速度慢
    for (; first != last; ++result, ++first) {
        *result = *first;
    }
    return result;
}

template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
    return __copy_d(first, last, result, distance_type(first));
}

// 完全泛化版本
template<typename InputIterator, typename OutputIterator>
struct __copy_dispatch
{
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
    {
        return __copy(first, last, result, iterator_category(first));
    }
};

// 偏特化版本, 两个参数都是 T* 指针形式
template<typename T>
struct __copy_dispatch<T*, T*>
{
    T* operator()(T* first, T* last, T* result) 
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

// 偏特化版本, 第一个参数为 const T*指针形式, 第二个参数为 T* 指针形式
template<typename T>
struct __copy_dispatch<const T*, T*>
{
    T* operator()(const T* first, const T* last, T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

// 重载
inline char* copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

// 重载
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

#endif
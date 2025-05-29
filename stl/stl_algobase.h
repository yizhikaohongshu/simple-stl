#ifndef __ALGOBASE_H__
#define __ALGOBASE_H__

#include "stl_iterator_base.h"
#include "type_traits.h"
#include "stl_pair.h"
#include <cstring>

template<typename _InputIterator1, typename _InputIterator2>
inline bool equal(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2)
{
    for (; __first1 != __last1; ++__first1, ++__first2) {
        if (*__first1 != *__first2) {
            return false;
        }
    }
    return true;
}

template<typename _InputIterator1, typename _InputIterator2, typename _BinaryPredicate>
inline bool equal(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _BinaryPredicate __binary_pred)
{
    for (; __first1 != __last1; ++__first1, ++__first2) {
        if (!__binary_pred(*__first1, *__first2)) {
            return false;
        }
    }
    return true;
}

template<typename _ForwardIterator, typename _Tp>
void fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value)
{
    for (; __first != __last; ++__first) {
        *__first = __value;
    }
}

template<typename _OutputIterator, typename _Size, typename _Tp>
_OutputIterator fill_n(_OutputIterator __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first) {
        *__first = __value;
    }
    return __first;
}

template<typename _ForwardIterator1, typename _ForwardIterator2, typename _Tp>
inline void __iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b, _Tp*)
{
    _Tp __tmp   = __a;
    __a         = __b;
    __b         = __tmp;
}

template<typename _ForwardIterator1, typename _ForwardIterator2>
inline void iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b)
{
    // typename iterator_tratis<_ForwardIterator1>::value_type tmp = *a;
    // *a = *b;
    // *b = tmp;
    __iter_swap(__a, __b, value_type(__a));
}

template<typename _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b)
{
    return __b < __a ? __a : __b;
}

template<typename _Tp, typename _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp)
{
    return __comp(__b, __a) ? __a : __b;
}

template<typename _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b)
{
    return __a < __b ? __a : __b;
}

template<typename _Tp, typename _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp)
{
    return __comp(__a, __b) ? __a : __b;
}

// TODO
template<typename _InputIterator1, typename _InputIterator2>
pair<_InputIterator1, _InputIterator2> mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2)
{
    while (__first1 != __last1 && *__first1 == *__first2) {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIterator1, _InputIterator2>(__first1, __first2);
}

template<typename _InputIterator1, typename _InputIterator2, typename _BinaryPredicate>
pair<_InputIterator1, _InputIterator2> mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _BinaryPredicate __binary_pred)
{
    while (__first1 != __last1 && __binary_pred(*__first1, *__first2)) {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIterator1, _InputIterator2>(__first1, __first2);
}

template<typename _Tp>
inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp   = __a;
    __a         = __b;
    __b         = __tmp;
}

template<typename _InputIterator1, typename _InputIterator2>
bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2)
{
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
        if (*__first1 < *__first2) {
            return true;
        }
        if (*__first2 < *__first1) {
            return false;
        }
    }

    return __first1 == __last1 && __first2 != __last2;
}

template<typename _InputIterator1, typename _InputIterator2, typename _Compare>
bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
{
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
        if (__comp(*__first1, *__first2)) {
            return true;
        }
        if (__comp(*__first2, *__first1)) {
            return false;
        }
    }

    return __first1 == __last1 && __first2 != __last2;
} 

template<typename _RandomAccessIterator, typename _OutputIterator, typename _Distance>
inline _OutputIterator __copy_d(_RandomAccessIterator __first, _RandomAccessIterator __last, _OutputIterator __result, _Distance*)
{
    // 以 n 决定循环的执行次数, 速度快
    for (_Distance __n = __last - __first; __n > 0; --__n, ++__result, ++__first) {
        *__result = *__first;
    }
    return __result;
}

template<typename _Tp>
inline _Tp* __copy_t(const _Tp* __first, const _Tp* __last, _Tp* __result, __true_type)
{
    memmove(__result, __first, sizeof(_Tp) * (__last - __first));
    return __result + (__last - __first);
}

template<typename _Tp>
inline _Tp* __copy_t(const _Tp* __first, const _Tp* __last, _Tp* __result, __false_type)
{
    return __copy_d(__first, __last, __result, (ptrdiff_t*) nullptr);
}

template<typename _InputIterator, typename _OutputIterator>
inline _OutputIterator __copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result, input_iterator_tag)
{
    // 以迭代器等同与否, 决定循环是否继续, 速度慢
    for (; __first != __last; ++__result, ++__first) {
        *__result = *__first;
    }
    return __result;
}

template<typename _RandomAccessIterator, typename _OutputIterator>
inline _OutputIterator __copy(_RandomAccessIterator __first, _RandomAccessIterator __last, _OutputIterator __result, random_access_iterator_tag)
{
    return __copy_d(__first, __last, __result, distance_type(__first));
}

// 完全泛化版本
template<typename _InputIterator, typename _OutputIterator>
struct __copy_dispatch
{
    _OutputIterator operator()(_InputIterator __first, _InputIterator __last, _OutputIterator __result)
    {
        return __copy(__first, __last, __result, iterator_category(__first));
    }
};

// 偏特化版本, 两个参数都是 _Tp* 指针形式
template<typename _Tp>
struct __copy_dispatch<_Tp*, _Tp*>
{
    _Tp* operator()(_Tp* __first, _Tp* __last, _Tp* __result) 
    {
        typedef typename __type_traits<_Tp>::has_trivial_assignment_operator t;
        return __copy_t(__first, __last, __result, t());
    }
};

// 偏特化版本, 第一个参数为 const _Tp*指针形式, 第二个参数为 _Tp* 指针形式
template<typename _Tp>
struct __copy_dispatch<const _Tp*, _Tp*>
{
    _Tp* operator()(const _Tp* __first, const _Tp* __last, _Tp* __result)
    {
        typedef typename __type_traits<_Tp>::has_trivial_assignment_operator t;
        return __copy_t(__first, __last, __result, t());
    }
};

template<typename _InputIterator, typename _OutputIterator>
inline _OutputIterator copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result)
{
    return __copy_dispatch<_InputIterator, _OutputIterator>()(__first, __last, __result);
}

// 重载
inline char* copy(const char* __first, const char* __last, char* __result)
{
    memmove(__result, __first, __last - __first);
    return __result + (__last - __first);
}

// 重载
inline wchar_t* copy(const wchar_t* __first, const wchar_t* __last, wchar_t* __result)
{
    memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
    return __result + (__last - __first);
}

// copy_backward
template<typename _BidirectionalIterator1, typename _BidirectionalIterator2, typename _Distance>
inline _BidirectionalIterator2 __copy_backward(_BidirectionalIterator1 __first, _BidirectionalIterator1 __last, _BidirectionalIterator2 __result, bidirectional_iterator_tag, _Distance*)
{
    while (__first != __last) {
        *--__result = *--__last;
    }
    return __result;
}

template<typename _RandomAccessIterator, typename _BidirectionalIterator, typename _Distance>
inline _BidirectionalIterator __copy_backward(_RandomAccessIterator __first, _RandomAccessIterator __last, _BidirectionalIterator __result, random_access_iterator_tag, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *--__result = *--__last;
    }
    return __result;
}

template<typename _BidirectionalIterator1, typename _BidirectionalIterator2, typename _BoolType>
struct __copy_backward_dispatch
{
    typedef typename iterator_traits<_BidirectionalIterator1>::iterator_category    _Cat;
    typedef typename iterator_traits<_BidirectionalIterator1>::difference_type      _Distance;

    static _BidirectionalIterator2 copy(_BidirectionalIterator1 __first, _BidirectionalIterator1 __last, _BidirectionalIterator2 __result)
    {
        return __copy_backward(__first, __last, __result, _Cat(), (_Distance*)(nullptr));
    }
};

// 偏特化版本, 两个参数都是 _Tp* 指针形式
template<typename _Tp>
struct __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
{
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
    {
        const ptrdiff_t _Num = __last - __first;

    }
};



#endif
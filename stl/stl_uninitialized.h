#ifndef __STL_UNINITIALIZED_H__
#define __STL_UNINITIALIZED_H__

#include "type_traits.h"
#include "stl_algobase.h"
#include "stl_construct.h"

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{   
    return fill_n(first, n, x);
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
    ForwardIterator cur = first;
    for (; n > 0; --n, ++cur) {
        construct(&*cur, x);
    }
    return cur;
}

template<typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
    /**
     * Plain Old Data, POD, 即标量类型或传统的 C struct 类型
     * POD 类型必然拥有 trivial ctor/dtor/copy/assignment 函数
     */
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, value_type(first));
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
    // TODO 调用 STL 算法 copy
    return copy(first, last, result);   
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
    ForwardIterator cur = result;
    for (; first != last; ++first, ++cur) {
        construct(&*cur, *first);
    }
    return cur;
}

template<typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
    fill(first, last, x);
}

template<typename ForwardIterator, typename T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
    ForwardIterator cur = first;
    for(; cur != last; ++cur) {
        construct(&*cur, x);
    }
}

template<typename ForwardIterator, typename T, typename T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD; 
    __uninitialized_fill_aux(first, last, x, is_POD());
}


template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
    __uninitialized_fill(first, last, x, value_type(first));
}



#endif
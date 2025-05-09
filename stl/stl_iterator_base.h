#ifndef __ITERATOR_TRAITS__
#define __ITERATOR_TRAITS__

#include <cstddef>

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template<typename I>
struct iterator_traits
{
    typedef typename I::iterator_category   iterator_category;
    typedef typename I::value_type          value_type;
    typedef typename I::difference_type     difference_type;
    typedef typename I::pointer             pointer;
    typedef typename I::reference           reference;
};

template<typename T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
};

template<typename T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
};

template<typename Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

// 决定某个迭代器的 value_type
template<typename Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(nullptr);
}

// 决定某个迭代器的 difference_type
template<typename Iterator>
typename iterator_traits<Iterator>::difference_type*
difference_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(nullptr);
}

// distance 函数, 计算两个迭代器之间的距离
template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, random_access_iterator_tag)
{
    return last - first;
}

template<typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
}

// advance : 迭代器参数 p 以及 数值 n, 函数内部将 p 累进 n 次
template<typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag)
{
    while (n--) {
        ++i;
    }
}

template<typename ForwardIterator, typename Distance>
inline void __advance(ForwardIterator& i, Distance n, forward_iterator_tag)
{
    __advance(i, n, input_iterator_tag());
}

template<typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0) {
        while (n--) ++i;
    }
    else {
        while (n++) --i;
    }
}

template<typename RandomIterator, typename Distance>
inline void __advance(RandomIterator& i, Distance n, random_access_iterator_tag)
{
    i += n;
}

template<typename InputIterator, typename Distance>
inline void advance(InputIterator& i, Distance n)
{
    __advance(i, n, typename iterator_traits<InputIterator>::iterator_category());
}


#endif
#ifndef __STL_PAIR_H__
#define __STL_PAIR_H__

template<typename T1, typename T2>
struct pair
{
    T1 first;
    T2 second;

    pair() : first(T1()), second(T2()) { }
    pair(const T1& __a, const T2& __b) : first(__a), second(__b) { }

    template<typename U1, typename U2>
    pair(const pair<U1, U2>& __p) : first(__p.first), second(__p.second) { }

    pair(const pair<T1, T2>& __o) : first(__o.first), second(__o.second) { }
};

template<typename T1, typename T2>
inline bool operator==(const pair<T1, T2>& __x, const pair<T1, T2>& __y)
{
    return __x.first == __y.first && __x.second == __y.second;
}

template<typename T1, typename T2>
inline bool operator<(const pair<T1, T2>& __x, const pair<T1, T2>& __y)
{
    return __x.first < __y.first || 
            (!(__y.first < __x.first) && __x.second < __y.second);
}

template<typename T1, typename T2>
inline bool operator!=(const pair<T1, T2>& __x, const pair<T1, T2>& __y)
{
    return !(__x == __y);
}

template<typename T1, typename T2>
inline bool operator>(const pair<T1, T2>& __x, const pair<T1, T2>& __y)
{
    return __y < __x;
}

template<typename T1, typename T2>
inline bool operator<=(const pair<T1, T2>& __x, const pair<T1, T2>& __y)
{
    return !(__y < __x);
}

template<typename T1, typename T2>
inline bool operator>=(const pair<T1, T2>& __x, const pair<T1, T2>& __y)
{
    return !(__x < __y);
}

template<typename T1, typename T2>
inline pair<T1, T2> make_pair(const T1& __x, const T2& __y)
{
    return pair<T1, T2>(__x, __y);
}

#endif // __STL_PAIR_H__
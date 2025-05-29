#ifndef __STL_PAIR_H__
#define __STL_PAIR_H__

template<typename _T1, typename _T2>
struct pair
{
    _T1 __first;
    _T2 __second;

    pair() : __first(_T1()), __second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : __first(__a), __second(__b) { }

    template<typename _U1, typename _U2>
    pair(const pair<_U1, _U2>& __p) : __first(__p.__first), __second(__p.__second) { }

    pair(const pair<_T1, _T2>& __o) : __first(__o.__first), __second(__o.__second) { }
};

template<typename _T1, typename _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.__first == __y.__first && __x.__second == __y.__second;
}

template<typename _T1, typename _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.__first < __y.__first || 
            (!(__y.__first < __x.__first) && __x.__second < __y.__second);
}

template<typename _T1, typename _T2>
inline bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return !(__x == __y);
}

template<typename _T1, typename _T2>
inline bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __y < __x;
}

template<typename _T1, typename _T2>
inline bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return !(__y < __x);
}

template<typename _T1, typename _T2>
inline bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return !(__x < __y);
}

template<typename _T1, typename _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
    return pair<_T1, _T2>(__x, __y);
}

#endif // __STL_PAIR_H__
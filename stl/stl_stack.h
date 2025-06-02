#ifndef __STL_STACK_H__
#define __STL_STACK_H__

template<typename _Tp, typename _Sequence>
class stack
{
// 模板友元声明
template<typename _Tp1, typename _Sequence1>
friend bool operator==(const stack<_Tp1, _Sequence1>&, const stack<_Tp1, _Sequence1>&);

template<typename _Tp1, typename _Sequence1>
friend bool operator<(const stack<_Tp1, _Sequence1>&, const stack<_Tp1, _Sequence1>&);

public:
    typedef typename _Sequence::value_type      value_type;
    typedef typename _Sequence::size_type       size_type;
    typedef          _Sequence                  container_type;
    typedef typename _Sequence::reference       reference;
    typedef typename _Sequence::const_reference const_reference;

protected:
    _Sequence c;

public:
    stack() : c() { }
    explicit stack(const _Sequence& __s) : c(__s) { }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& __x) { c.push_back(__x); }
    void pop() { c.pop_back(); }
};

template<typename _Tp, typename _Sequence>
bool operator==(const stack<_Tp, _Sequence>& __x, const stack<_Tp, _Sequence>& __y)
{
    return __x.c == __y.c;
}

template<typename _Tp, typename _Sequence>
bool operator<(const stack<_Tp, _Sequence>& __x, const stack<_Tp, _Sequence>& __y)
{
    return __x.c < __y.c;
}

template<typename _Tp, typename _Sequence>
bool operator!=(const stack<_Tp, _Sequence>& __x, const stack<_Tp, _Sequence>& __y)
{
    return !(__x == __y);
}

template<typename _Tp, typename _Sequence>
bool operator>(const stack<_Tp, _Sequence>& __x, const stack<_Tp, _Sequence>& __y)
{
    return __y < __x;
}

template<typename _Tp, typename _Sequence>
bool operator<=(const stack<_Tp, _Sequence>& __x, const stack<_Tp, _Sequence>& __y)
{
    return !(__y < __x);
}

template<typename _Tp, typename _Sequence>
bool operator>=(const stack<_Tp, _Sequence>& __x, const stack<_Tp, _Sequence>& __y)
{
    return !(__x < __y);
}

#endif // __STL_STACK_H__
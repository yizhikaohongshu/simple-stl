#ifndef __STL_QUEUE_H__
#define __STL_QUEUE_H__

template<typename _Tp, typename _Sequence>
class queue
{
template<typename _Tp1, typename _Sequence1>
friend bool operator==(const queue<_Tp1, _Sequence1>&, const queue<_Tp1, _Sequence1>&);

template<typename _Tp1, typename _Sequence1>
friend bool operator<(const queue<_Tp1, _Sequence1>&, const queue<_Tp1, _Sequence1>&);

public:
    typedef typename _Sequence::value_type      value_type;
    typedef typename _Sequence::size_type       size_type;
    typedef          _Sequence                  container_type;
    typedef typename _Sequence::reference       reference;
    typedef typename _Sequence::const_reference const_reference;

protected:
    _Sequence c;

public:
    queue() : c() { }
    explicit queue(const _Sequence& __s) : c(__s) { }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }

    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }

    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }

    void push(const value_type& __x) { c.push_back(__x); }
    void pop() { c.pop_front(); }
};

template<typename _Tp, typename _Sequence>
bool operator==(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
    return __x.c == __y.c;
}

template<typename _Tp, typename _Sequence>
bool operator<(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
    return __x.c < __y.c;
}
 
#endif // __STL_QUEUE_H__
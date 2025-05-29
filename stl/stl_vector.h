#ifndef __STL_VECTOR_H__
#define __STL_VECTOR_H__

#include <cstddef>
#include "stl_algobase.h"
#include "memory"

template<typename _Tp, typename _Alloc>
class vector
{
public:
    typedef _Tp             value_type;
    typedef value_type*     pointer;
    typedef value_type*     iterator;
    typedef value_type&     reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

protected:
    typedef simple_alloc<value_type, _Alloc> _M_data_allocator;

    iterator _M_start;
    iterator _M_finish;
    iterator _M_end_of_storage;

    void _M_insert_aux(iterator position, const _Tp& x);

public:
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return static_cast<size_type>(end() - begin()); }
    size_type capacity() const { return static_cast<size_type>(_M_end_of_storage - begin()); }
    bool empty() const { return _M_start == _M_finish; }
    reference operator[](size_type __n) { return *(begin() + __n); }

    vector() : _M_start(nullptr), _M_finish(nullptr), _M_end_of_storage(nullptr) { }
    vector(size_type __n, const _Tp& value) { fill_initialize(__n, value); }

    reference front() { return *begin(); }
    reference back() { return *(end() - 1); }

    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            construct(_M_finish, x);
            ++_M_finish;
        }
        else {
            _M_insert_aux(end(), x);
        }
    }

    void pop_back()
    {
        --_M_finish;
        destroy(_M_finish);
    }
    
    iterator erase(iterator position)
    {
        if (position + 1 != end()) {
            copy(position + 1, _M_finish, position);
        }
        --_M_finish;
        destroy(_M_finish);
        return position;
    }

    iterator erase(iterator first, iterator last)
    {
        iterator i = copy(last, _M_finish, first);
        destroy(i, _M_finish);
        _M_finish = _M_finish - (last - first);
        return first;
    }

    void resize(size_type new_size, const _Tp& x)
    {
        if (new_size < size()) {
            erase(begin() + new_size, end());
        }
        else {
            insert(end(), new_size - size(), x);
        }
    }

    void resize(size_type new_size) { resize(new_size, _Tp()); }
    void clear() { erase(begin(), end()); }

protected:
    iterator allocate_and_fill(size_type __n, const _Tp& x)
    {
        iterator result = _M_data_allocator::allocate(__n);
        uninitialized_fill_n(result, __n, x);
        return result;
    }

    void fill_initialize(size_type __n, const _Tp& value)
    {
        _M_start = allocate_and_fill(__n, value);
        _M_finish = _M_start + __n;
        _M_end_of_storage = _M_finish;
    }
};

template<typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage) {
        construct(_M_finish, *(_M_finish - 1));
        ++_M_finish;
        _Tp __x_copy = __x;
        copy_backward(__position, _M_finish - 2, _M_finish - 1);
        *__position = __x_copy;
    }
    else {
        const size_type __old_size  = size();
        const size_type __len       = __old_size != 0 ? 2 * __old_size : 1;

        iterator __new_start        = _M_data_allocator::allocate(__len);
        iterator __new_finish       = __new_start;
        try {
            __new_finish = uninitialized_copy(_M_start, __position, __new_start);
            construct(__new_finish, __x);
            ++__new_finish;
            __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
        }
        catch(...) {
            destroy(__new_start, __new_finish);
            _M_data_allocator::deallocate(__new_start, __len);
            throw;
        }

        destroy(begin(), end());
        deallocate(_M_start, _M_end_of_storage - _M_start);

        _M_start            = __new_start;
        _M_finish           = __new_finish;
        _M_end_of_storage   = __new_start + __len;
    }
}


#endif // __STL_VECTOR_H__
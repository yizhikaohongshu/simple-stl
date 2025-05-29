#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <new>
#include <cstdlib>
#include <cstddef>
#include <climits>
#include <cstdio>
#include <cstring>

#define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)

/**
 * @brief alloc 设计哲学
 * @details
 * 1. 向 system heap 要求空间;
 * @todo 2. 考虑多线程状态;
 * 3. 考虑内存不足时的应变措施;
 * 4. 考虑过多``小型区块"可能造成的内存碎片问题.
 */

typedef void (*__oom_handler_type)();

/**
 * @brief 第一级配置器
 * @details
 * 1. allocate() 直接使用 malloc(), deallocate() 直接使用 free()
 * 2. 模拟 C++ 的 set_new_handler() 以处理内存不足的状况
 */
template<int __inst>
class __malloc_alloc
{
private:
    // 用以处理内存不足的情况
    static void* _S_oom_malloc(size_t);
    static void* _S_oom_realloc(void*, size_t);

    static __oom_handler_type __oom_handler;

public:
    typedef char value_type;
    static void* allocate(size_t __n)
    {
        void* __result = malloc(__n);
        if (__result == nullptr) __result = _S_oom_malloc(__n);
        return __result;
    }

    static void deallocate(void* __p, size_t) 
    {
        free((char*)__p);
    }

    static void* reallocate(void* __p, size_t, size_t __new_sz)
    {
        void* __result = realloc((char*)__p, __new_sz);
        if (__result == nullptr) __result = _S_oom_realloc(__p, __new_sz);
        return __result;
    }

    static __oom_handler_type set_malloc_handler(__oom_handler_type __f)
    {
        __oom_handler_type __old = __oom_handler;
        __oom_handler = __f;
        return __old;
    }
};

template<int __inst>
__oom_handler_type __malloc_alloc<__inst>::__oom_handler = static_cast<__oom_handler_type>(nullptr);

template<int __inst>
void* __malloc_alloc<__inst>::_S_oom_malloc(size_t __n)
{
    __oom_handler_type __my_malloc_handler;
    void* __result;

    for (;;) {
        __my_malloc_handler = __oom_handler;
        if (__my_malloc_handler == nullptr) { __THROW_BAD_ALLOC; }
        (*__my_malloc_handler)();
        __result = malloc(__n);
        if (__result) return __result;
    }
}

template<int __inst>
void* __malloc_alloc<__inst>::_S_oom_realloc(void* __p, size_t __n)
{
    __oom_handler_type __my_malloc_handler;
    void* __result;

    for (;;) {
        __my_malloc_handler = __oom_handler;
        if (__my_malloc_handler == nullptr) { __THROW_BAD_ALLOC; }
        (*__my_malloc_handler)();
        __result = realloc(__p, __n);
        if (__result) return __result;
    }
}

typedef __malloc_alloc<0> malloc_alloc;

constexpr int _ALIGN        = 8;         // 小型区块的上调边界
constexpr int _MAX_BYTES    = 128;       // 小型区块的上界
constexpr int _NFREELISTS   = _MAX_BYTES / _ALIGN;

/**
 * _M_client_data[1]:
 * 1. 不是表示只有一字节可用空间;
 * 2. 这是一种经典的``C风格技巧"，称为柔性数组的早期实现;
 * 3. 实际使用时，内存块会比 union _Node_alloc_obj 本身大很多, _M_client_data 只是提供访问这块内存的入口
 */
union _Node_alloc_obj {
    union _Node_alloc_obj*  _M_free_list_link;
    char                    _M_client_data[1];
};

/**
 * @brief 第二级配置器
 * @details
 * 1. 维护 16 个自由链表 (free lists)
 * 2. 如果需求区块大于 128 bytes, 转调用第一级配置器
 */
template<bool __threads, int __inst>
class __default_alloc_template
{
private:
    typedef _Node_alloc_obj _Obj;

    static size_t _S_round_up(size_t __bytes)
    {
        return ((__bytes + _ALIGN - 1) & ~(_ALIGN - 1));
    }

    static _Obj* volatile _S_free_list[_NFREELISTS];

    static size_t _S_freelist_index(size_t __bytes)
    {
        return ((__bytes + _ALIGN - 1) / _ALIGN - 1);
    }

    /**
     * @brief 返回一个大小为 __n 的对象, 并可能加入大小为 __n 的其他区块到 free list
     */
    static void* _S_refill(size_t __n);
    /**
     * @brief 配置一大块空间, 可容纳 nobjs 个大小为 __size 的区块
     */
    static char* _S_chunk_alloc(size_t __size, int& __nobjs);

    static char*    _S_start_free;
    static char*    _S_end_free;
    static size_t   _S_heap_size;

public:
    inline static void* allocate(size_t __n);

    inline static void deallocate(void* __p, size_t __n);

    static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz);
};

template<bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_start_free = nullptr;

template<bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_end_free = nullptr;

template<bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;

template<bool __threads, int __inst>
typename __default_alloc_template<__threads, __inst>::_Obj* volatile
__default_alloc_template<__threads, __inst>::_S_free_list[_NFREELISTS] = { nullptr, };

/**
 * @brief 配置一大块空间, 可容纳 __nobjs 个大小为 __size 的区块, 如果配置 __nobjs 个区块有所不便, __nobjs 可能会降低
 * @param __nobjs number of objects, 每次从内存池中请求的内存块数量
 */
template<bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_chunk_alloc(size_t __size, int& __nobjs)
{
    char*   __result;
    size_t  __total_bytes    = __size * __nobjs;
    size_t  __bytes_left     = _S_end_free - _S_start_free;

    if (__bytes_left >= __total_bytes) {
        // 内存池剩余空间完全满足需求量
        __result        = _S_start_free;
        _S_start_free   += __total_bytes;
        return __result;
    } 
    else if (__bytes_left >= __size) {
        // 内存池剩余空间不能完全满足需求量, 但足够供应一个及以上的区块
        __nobjs         = static_cast<int>(__bytes_left / __size);
        __total_bytes   = __size * __nobjs;
        __result        = _S_start_free;
        _S_start_free   += __total_bytes;
        return __result;
    }
    else {
        size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
        
        if (__bytes_left > 0) {
            _Obj* volatile* __my_free_list = _S_free_list + _S_freelist_index(__bytes_left);
            ((_Obj*)(_S_start_free))->_M_free_list_link = *__my_free_list;
            *__my_free_list = (_Obj*)(_S_start_free);
        }

        _S_start_free = static_cast<char*>(malloc(__bytes_to_get));
        if (_S_start_free == nullptr) {
            _Obj* volatile* __my_free_list;
            _Obj* __p;

            for (size_t __i = __size; __i <= _MAX_BYTES; __i += _ALIGN) {
                __my_free_list = _S_free_list + _S_freelist_index(__i);
                __p = *__my_free_list;
                if (__p != nullptr) {
                    *__my_free_list = __p->_M_free_list_link;
                    _S_start_free   = (char*)(__p);
                    _S_end_free     = _S_start_free + __i;
                    return _S_chunk_alloc(__size, __nobjs);
                }
            }
            _S_end_free     = nullptr;
            _S_start_free   = (char*)(malloc_alloc::allocate(__bytes_to_get));
        }
        _S_heap_size    += __bytes_to_get;
        _S_end_free     = _S_start_free + __bytes_to_get;

        return _S_chunk_alloc(__size, __nobjs);
    }
}

/**
 * @details 返回一个大小为 n 的对象, 并且有时候会为适当的 free list 增加节点
 * 假设 n 已经适当上调至 8 的整数倍
 */
template<bool __threads, int __inst>
void* __default_alloc_template<__threads, __inst>::_S_refill(size_t __n)
{
    int     __nobjs = 20;
    char*   __chunk = _S_chunk_alloc(__n, __nobjs);
    _Obj* volatile* __my_free_list;
    _Obj*   __result;
    _Obj*   __current_obj;
    _Obj*   __next_obj;

    // 如果内存池只能提供 1 个块，直接返回给调用者. 无需构建自由链表（因为没有剩余块）
    if (1 == __nobjs) return __chunk;
    __my_free_list = _S_free_list + _S_freelist_index(__n);

    // 以下在 __chunk 空间内建立 free list
    __result = (_Obj*)(__chunk);
    *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);

    // 将 free list 的各节点串接起来
    for (int __i = 1; ; ++__i) {
        __current_obj = __next_obj;
        __next_obj = (_Obj*)((char*)(__next_obj) + __n);
        if (__nobjs - 1 == __i) {
            __current_obj->_M_free_list_link = nullptr;
            break;
        }
        else {
            __current_obj->_M_free_list_link = __next_obj;
        }
    }
    return __result;
}

template<bool __threads, int __inst>
void* __default_alloc_template<__threads, __inst>::reallocate(void* __p, size_t __old_sz, size_t __new_sz)
{
    void*   __result;
    size_t  __copy_sz;

    if (__old_sz > static_cast<size_t>(_MAX_BYTES) && __new_sz > static_cast<size_t>(_MAX_BYTES)) {
        return realloc(__p, __new_sz);
    }
    if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) {
        return __p;
    }
    __result    = allocate(__new_sz);
    __copy_sz   = __new_sz > __old_sz ? __old_sz : __new_sz;
    memcpy(__result, __p, __copy_sz);
    deallocate(__p, __old_sz);
    return __result;
}

template<bool __threads, int __inst>
inline void* __default_alloc_template<__threads, __inst>::allocate(size_t __n)
{
    _Obj* volatile* __my_free_list;
    _Obj* __result;

    // 调用一级配置器
    if (__n > static_cast<size_t>(_MAX_BYTES)) {
        return malloc_alloc::allocate(__n);
    }

    __my_free_list = _S_free_list + _S_freelist_index(__n);
    __result = *__my_free_list;
    if (__result == nullptr) {
        void* __r = _S_refill(_S_round_up(__n));
        return __r;
    }

    *__my_free_list = __result->_M_free_list_link;
    return __result;
}

template<bool __threads, int __inst>
inline void __default_alloc_template<__threads, __inst>::deallocate(void* __p, size_t __n)
{
    _Obj* __q = static_cast<_Obj*>(__p);
    _Obj* volatile* __my_free_list;

    // 调用一级配置器
    if (__n > static_cast<size_t>(_MAX_BYTES)) {
        malloc_alloc::deallocate(__p, __n);
        return;
    }

    __my_free_list = _S_free_list + _S_freelist_index(__n);
    // 回收区块
    __q->_M_free_list_link = *__my_free_list;
    *__my_free_list = __q;
}


template<typename _Tp, typename _Alloc = malloc_alloc>
class simple_alloc
{
    typedef _Alloc __alloc_type;
public:
    typedef typename _Alloc::value_type __alloc_value_type;
    typedef _Tp value_type;
    // 转调用, 使配置器的配置单位从 bytes 转为个别元素的大小 (sizeof(_Tp))
    static _Tp* allocate(size_t __n)
    { return __n == 0 ? nullptr : static_cast<_Tp*>(__alloc_type::allocate(__n * sizeof(_Tp))); }
    
    static _Tp* allocate(void)
    { return static_cast<_Tp*>(__alloc_type::allocate(sizeof(_Tp))); }

    static void deallocate(_Tp* __p, size_t __n)
    {
        if (__n != 0) {
            __alloc_type::deallocate(__p, __n * sizeof(_Tp));
        }
    }

    static void deallocate(_Tp* __p) 
    { __alloc_type::deallocate(__p, sizeof(_Tp)); }
};

#endif  // __ALLOC_H__
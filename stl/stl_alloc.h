#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <new>
#include <cstdlib>
#include <cstddef>
#include <climits>
#include <iostream>

#define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)

/**
 * alloc 设计哲学
 * 1. 向 system heap 要求空间;
 * 2. （todo）考虑多线程状态;
 * 3. 考虑内存不足时的应变措施;
 * 4. （todo）考虑过多``小型区块"可能造成的内存碎片问题.
 */

typedef void (*__oom_handler_type)();

// 第一级配置器
template<int __inst>
class __malloc_alloc
{
private:
    // 用以处理内存不足的情况
    static void* _S_oom_malloc(size_t);
    static void* _S_oom_realloc(void*, size_t);

    static __oom_handler_type __oom_handler;

public:
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
__oom_handler_type __malloc_alloc<__inst>::__oom_handler = (__oom_handler_type)(nullptr);

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

// 第二级配置器
// todo

typedef __malloc_alloc<0> malloc_alloc;

template<typename T, typename Alloc = malloc_alloc>
class simple_alloc
{
public:
    static T* allocate(size_t n)
    { return n == 0 ? nullptr : (T*)Alloc::allocate(n * sizeof(T)); }
    
    static T* allocate(void)
    { return (T*)Alloc::allocate(sizeof(T)); }

    static void deallocate(T* p, size_t n)
    {
        if (n != 0) {
            Alloc::deallocate(p, n * sizeof(T));
        }
    }

    static void deallocate(T* p) 
    { Alloc::deallocate(p, sizeof(T)); }
};

#endif  // __ALLOC_H__
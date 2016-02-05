#include "short_alloc.h"
#include <iostream>
#include <new>
#include <vector>
#include <cstdint>
#include <cassert>

// Replace new and delete just for the purpose of demonstrating that
//  they are not called.

std::size_t memory = 0;
std::size_t alloc = 0;

void* operator new(std::size_t s) //throw(std::bad_alloc)
{
    memory += s;
    ++alloc;
    return std::malloc(s);
}

void  operator delete(void* p) noexcept//throw()
{
    --alloc;
    std::free(p);
}

void operator delete(void* ptr, std::size_t) noexcept //throw()
{
    ::operator delete(ptr);
}

void memuse()
{
    std::cout << "memory = " << memory << '\n';
    std::cout << "alloc = " << alloc << '\n';
}

template <typename T, size_t NUM_EL, size_t ALIG = alignof(T)>
using MyAlloc = short_alloc<T, NUM_EL * sizeof(T), ALIG>; // arena holds at most NUM_EL elements of type T, aligning allocation requests to ALIG

template <typename T, size_t NUM_EL, size_t ALIG = alignof(T)>
class MyVec
    :                       MyAlloc<T, NUM_EL, ALIG>::arena_type,
      public std::vector<T, MyAlloc<T, NUM_EL, ALIG> >
{
    using arena_t  = typename MyAlloc<T, NUM_EL, ALIG>::arena_type;
    using alloc_t  =          MyAlloc<T, NUM_EL, ALIG>;
    using vector_t = std::vector<T, alloc_t>;

    //static_assert(alloc_t::size % alloc_t::alignment == 0, "size is not a multiple of alignment");

public:    
    using size_type      = typename vector_t::size_type;
    using allocator_type = typename vector_t::allocator_type;

    MyVec()
        : arena_t{}, vector_t{alloc_t(static_cast<arena_t &>(*this))}
    {
        vector_t::reserve(NUM_EL); // set up the std::vector to use the entire arena!!!
        assert(vector_t::capacity() == NUM_EL);
    }

};



int main()
{
    constexpr size_t NUM_EL = 2;

    MyVec<uint8_t, NUM_EL, alignof(uint64_t)> vec;
    vec.clear();
    vec.push_back(0x11);
    vec.push_back(0x22);

    uint64_t val = * reinterpret_cast<uint64_t *>(vec.data()); // access data with a uint64_t
    assert(
              ( htole64(val)                                          // host_to_little_endian in /usr/include/endian.h
                & static_cast<uint64_t>(UINT64_C(0x000000000000FFFF)) // mask out junk-bits
              )
              == 0x2211
          );
    
    memuse();
}

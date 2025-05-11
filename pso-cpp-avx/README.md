Testing vector procesing with AVX.
XD.
[original](https://github.com/r4qq/pso-crazy-man/tree/main/pso-cpp) one.

So i've written simple custom allocator to allocate alligned memory for the avx instructions.
Heavily "inspired" by [this](https://github.com/zhourrr/aligned-memory-allocator/blob/main/aligned_allocator.h).
         
```cpp
#pragma once

#include <cstddef> 
#include <cstdlib> 

template <typename T>
class CustomAlignedAllocator 
{
public:
    // Type alias for the value type managed by the allocator
    using value_type = T;

    // Default constructor, no initialization needed
    CustomAlignedAllocator() = default;

    // Converting constructor to allow construction from an allocator of a different type
    // This enables interoperability between allocators of different types
    template<typename U>
    CustomAlignedAllocator(CustomAlignedAllocator<U>&) {}

    // Allocates memory for n objects of type T with proper alignment
    // Uses std::aligned_alloc to ensure memory is aligned according to T's alignment requirements
    T* allocate(size_t n)
    {
        // Allocates memory of size n * sizeof(T) with alignment specified by alignof(T)
        // Returns a pointer to the allocated memory, cast to T*
        return static_cast<T*>(std::aligned_alloc(alignof(T), n * sizeof(T)));
    }

    // Deallocates memory previously allocated by this allocator
    // The size parameter is ignored as std::free doesn't require it
    void deallocate(T* pointer, std::size_t)
    {
        // Frees the memory pointed to by pointer using std::free
        // Safe to call with nullptr (std::free handles it)
        free(pointer);
    }

    // Equality comparison with another allocator of type U
    // Returns true, indicating all CustomAlignedAllocator instances are considered equal
    // This is valid since the allocator is stateless and uses std::aligned_alloc/free
    template <typename U>
    bool operator==(const CustomAlignedAllocator<U>&) const noexcept { return true; }
    
    // Inequality comparison with another allocator of type U
    // Returns false, consistent with operator== returning true
    template <typename U>
    bool operator!=(const CustomAlignedAllocator<U>&) const noexcept { return false; }
};

```
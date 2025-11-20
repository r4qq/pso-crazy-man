#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <type_traits>

inline void* portableAllocate(size_t alignment, size_t size) noexcept 
{
#if defined (_WIN32) || (_WIN64)
    return _aligned_malloc(alignment, size);
#elif defined (_APPLE_)
    void* ptr;
    if (posix_memalign(&ptr, alignment, size)) 
    {
        return nullptr;
    }
    return ptr;
#else
    return std::aligned_alloc(alignment, size);
#endif
}

inline void portableDeallocate(void* ptr) noexcept
{
#if defined (_WIN32) || (_WIN64)
    _aligned_free(ptr);
#else
    std::free(ptr);
#endif    
}

template<typename T, size_t Alignment>
class AlignedAllocator 
{
    public:
        using value_type = T;
        using is_always_equal = std::true_type;

        static_assert((Alignment & (Alignment - 1)) == 0,
                      "Allignment must be power of 2");
        static_assert(Alignment >= alignof(T), 
                      "Allignment must be >= allignof(T)");

        AlignedAllocator() noexcept = default;
        
        template<typename U> 
        AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}

        template<typename U>
        struct rebind {using other = AlignedAllocator<U, Alignment>;};

        bool operator==(const AlignedAllocator&) const noexcept {return true;}
        bool operator!=(const AlignedAllocator& a) const noexcept {return !(*this == a);}
    
        constexpr T* allocate(size_t n) 
        {
            if (n == 0)
                return nullptr;

            const std::size_t bytes = n * sizeof(T);
            const std::size_t alignedBytes = (bytes + Alignment - 1) & ~(Alignment - 1);

            void* p = portableAllocate(Alignment, alignedBytes);
            
            if (!p)
                throw std::bad_alloc();

            return static_cast<T*>(p);
        }

        constexpr void deallocate(T* ptr, size_t n)
        {
            portableDeallocate(ptr);
        }
};
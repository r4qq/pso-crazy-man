#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <type_traits>

template<typename T, size_t Allignment>
class AllignedAllocator 
{
    using value_type = T;
    using is_always_equal = std::true_type;

    static_assert(Allignment % 2 == 0, 
                  "Allignment must be power of 2");
    static_assert(Allignment >= alignof(T), 
                  "Allignment must be >= allignof(T)");

    AllignedAllocator() noexcept = default;
    
    template<typename U> 
    AllignedAllocator(const AllignedAllocator<U, Allignment>&) noexcept {}

    template<typename U>
    struct rebind {using other = AllignedAllocator<U, Allignment>;};

    bool operator==(const AllignedAllocator&) const noexcept {return true;}
    bool operator!=(const AllignedAllocator& a) const noexcept {return !(*this == a);}

    public:
        T* allocate(size_t n) 
        {
            size_t size = n * sizeof(T);
            size_t allignedSize = (size + (Allignment - 1)) & ~(Allignment - 1);
            void* ptr = std::aligned_alloc(Allignment, allignedSize);

            if (ptr == nullptr) 
            {
                throw std::bad_alloc();
            }

            return static_cast<T*>(ptr);
        }

        void dealloate(T* p, size_t n)
        {
            free(p);
        }
};
#pragma once

#include <cstddef>
#include <cstdlib>

template <typename T>
class CustomAllocator 
{
    public:
        using value_type = T;

        CustomAllocator() = default;
        template<typename U> CustomAllocator(CustomAllocator<U>&) {};
        T* allocate(size_t n)
        {
            return static_cast<T*>(std::aligned_alloc(alignof(T), n * sizeof(T)));
        }

        void deallocate(T* pointer, std::size_t)
        {
            free(pointer);
        }

        template <typename U>
        bool operator==(const CustomAllocator<U>&) const noexcept { return true; }
    
        template <typename U>
        bool operator!=(const CustomAllocator<U>&) const noexcept { return false; }
};

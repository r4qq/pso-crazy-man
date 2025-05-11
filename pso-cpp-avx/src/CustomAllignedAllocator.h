#pragma once

#include <cstddef>
#include <cstdlib>

template <typename T>
class CustomAllignedAllocator 
{
    public:
        using value_type = T;

        CustomAllignedAllocator() = default;
        template<typename U> CustomAllignedAllocator(CustomAllignedAllocator<U>&) {};
        T* allocate(size_t n)
        {
            return static_cast<T*>(std::aligned_alloc(alignof(T), n * sizeof(T)));
        }

        void deallocate(T* pointer, std::size_t)
        {
            free(pointer);
        }

        template <typename U>
        bool operator==(const CustomAllignedAllocator<U>&) const noexcept { return true; }
    
        template <typename U>
        bool operator!=(const CustomAllignedAllocator<U>&) const noexcept { return false; }
};

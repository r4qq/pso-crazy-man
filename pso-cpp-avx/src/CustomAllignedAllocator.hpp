#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

template <typename T>
class CustomAllignedAllocator 
{
    public:
        using value_type = T;

        CustomAllignedAllocator() = default;
        template<typename U> CustomAllignedAllocator(CustomAllignedAllocator<U>&) {};
        T* allocate(size_t n)
        {
            size_t alignment = 64;
            size_t size = n * sizeof(T);

            size_t aligned_size = (size + alignment - 1) & ~(alignment - 1);
            void* ptr = std::aligned_alloc(alignment, aligned_size);
            if (!ptr) 
            {
                throw std::bad_alloc();
            }
            return static_cast<T*>(ptr);
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

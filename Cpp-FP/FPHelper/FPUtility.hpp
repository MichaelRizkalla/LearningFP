
#ifndef FP_UTILITY_HPP
#define FP_UTILITY_HPP
#include <cmath>
#include <memory>
#include <string>

namespace fp {

    template < class Type, class Allocator, class... TArgs, std::enable_if_t< !std::is_array_v< Type >, int > = 0 >
    std::unique_ptr< Type, std::function< void(Type*) > > allocate_unique(Allocator alloc, TArgs... args) {
        using allocator_type = std::allocator_traits< Allocator >::template rebind_alloc< Type >;

        auto custom_deleter = [](Type* ptr, allocator_type m_alloc) {
            std::allocator_traits< allocator_type >::destroy(m_alloc, ptr);
            m_alloc.deallocate(ptr, 1);
        };

        allocator_type type_alloc { alloc };

        Type* ptr = type_alloc.allocate(1);
        std::allocator_traits< allocator_type >::construct(alloc, ptr, std::forward< TArgs >(args)...);

        return { ptr, std::bind(custom_deleter, std::placeholders::_1, type_alloc) };
    }

    template < class Type, class ConstructedType, class Allocator, class... TArgs, std::enable_if_t< !std::is_array_v< Type >, int > = 0 >
    std::unique_ptr< Type, std::function< void(Type*) > > allocate_unique(Allocator alloc, TArgs... args) {
        using constructor    = std::allocator_traits< Allocator >::template rebind_alloc< ConstructedType >;

        auto custom_deleter = [](Type* ptr, constructor c_alloc) {
            std::allocator_traits< constructor >::destroy(c_alloc, reinterpret_cast< ConstructedType* >(ptr));
            c_alloc.deallocate(reinterpret_cast< ConstructedType* >(ptr), 1);
        };

        constructor ctor_alloc { alloc };

        ConstructedType* ptr = ctor_alloc.allocate(1);
        std::allocator_traits< constructor >::construct(ctor_alloc, ptr, std::forward< TArgs >(args)...);

        return { static_cast< Type* >(ptr), std::bind(custom_deleter, std::placeholders::_1, ctor_alloc) };
    }

    template < class ArrayType, class Allocator, std::enable_if_t< std::is_array_v< ArrayType > && std::extent_v< ArrayType > == 0, int > = 0 >
    std::unique_ptr< ArrayType, std::function< void(std::remove_extent_t< ArrayType >*) > > allocate_unique(Allocator alloc, const std::size_t size) {
        using Type           = std::remove_extent_t< ArrayType >;
        using allocator_type = std::allocator_traits< Allocator >::template rebind_alloc< Type >;

        auto custom_array_deleter = [](Type* ptr, allocator_type m_alloc, std::size_t m_size) {
            for (std::size_t i = 0; i < m_size; ++i) { std::allocator_traits< allocator_type >::destroy(m_alloc, ptr + i); }
            m_alloc.deallocate(ptr, m_size);
        };

        allocator_type type_alloc { alloc };

        Type* ptr = type_alloc.allocate(size);
        if constexpr (std::is_default_constructible_v< Type >) {
            for (std::size_t i = 0; i < size; ++i) { std::allocator_traits< allocator_type >::construct(alloc, ptr + i); }
        }

        return { ptr, std::bind(custom_array_deleter, std::placeholders::_1, type_alloc, size) };
    }

    namespace impl {
        std::string weekday[7] = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
    } // namespace impl

    // Credits: https://www.tutorialspoint.com/day-of-the-week-in-cplusplus
    std::string zellersAlgorithm(int day, int month, int year) {
        int mon;
        if (month > 2)
            mon = month;
        else {
            mon = (12 + month);
            year--;
        }
        int y = year % 100;
        int c = year / 100;
        int w = (day + std::floor((13 * (mon + 1)) / 5) + y + std::floor(y / 4) + std::floor(c / 4) + (5ull * c));
        w     = w % 7;
        return impl::weekday[w];
    }

} // namespace fp

#endif // FP_UTILITY_HPP
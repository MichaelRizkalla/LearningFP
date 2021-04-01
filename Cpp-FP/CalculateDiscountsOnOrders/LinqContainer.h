// This header represents a header with a linq container
// It contains minimal functionalities to run CalculateDiscountsOnOrders

#ifndef LINQ_CONTAINER
#define LINQ_CONTAINER

#include <algorithm>
#include <concepts>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <ranges>

template < typename Type >
concept addable = requires(Type lhs, Type rhs) {
    lhs + rhs;
};

template < typename Type >
concept dividable = requires(Type lhs, Type rhs) {
    lhs / rhs;
};

template < class Type, class Allocator = std::allocator< Type > >
class LinqContainer : public std::ranges::view_interface<LinqContainer<Type, Allocator>> {
  public:
    using value_type     = std::vector< Type >::value_type;
    using allocator_type = std::vector< Type >::allocator_type;
    using size_type      = std::vector< Type >::size_type;
    using iterator       = std::vector< Type >::iterator;
    using const_iterator = std::vector< Type >::const_iterator;

    LinqContainer(std::initializer_list< Type > elements_) : elements(elements_) {};
    LinqContainer(std::vector< Type > elements_) : elements(elements_) {};
    LinqContainer(size_type size) : elements(size) {};
    LinqContainer()  = default;
    ~LinqContainer() = default;

    inline LinqContainer& emplace_back(Type&& element) {
        elements.emplace_back(std::move(element));
        return *this;
    }
    inline LinqContainer& emplace_back(const Type& element) {
        elements.emplace_back(element);
        return *this;
    }

    [[nodiscard]] inline constexpr void      resize(size_type size_) { return elements.resize(size_); }

    [[nodiscard]] inline constexpr iterator       begin() noexcept { return elements.begin(); }
    [[nodiscard]] inline constexpr const_iterator begin() const noexcept { return elements.begin(); }
    [[nodiscard]] inline constexpr iterator       end() noexcept { return elements.end(); }
    [[nodiscard]] inline constexpr const_iterator end() const noexcept { return elements.end(); }

    [[nodiscard]] inline constexpr value_type&       at(size_type index) { return elements.at(index); }
    [[nodiscard]] inline constexpr const value_type& at(size_type index) const { return elements.at(index); }

    [[nodiscard]] auto Take(size_type size_) && {
        if (size_ > this->size()) throw std::out_of_range { "Requested size is greater than the container size" };
        elements.resize(size_);
        return std::move(*this);
    }
    [[nodiscard]] auto Take(size_type size_) const& {
        if (size_ > this->size()) throw std::out_of_range { "Requested size is greater than the container size" };
        LinqContainer new_elements {};
        new_elements.resize(this->size());
        while (size_ > 0) { new_elements[size_ - 1] = at(size_ - 1); }

        return std::move(new_elements);
    }

    [[nodiscard]] Type Average() const requires addable< Type >&& dividable< Type > {
        return std::accumulate(elements.begin(), elements.end(), value_type(0)) / elements.size();
    }

    template < std::predicate< Type, Type > Functor >
    [[nodiscard]] auto OrderBy(Functor&& func) && -> LinqContainer {
        std::sort(elements.begin(), elements.end(), func);

        return std::move(elements);
    }
    template < std::predicate< Type, Type > Functor >
    [[nodiscard]] auto OrderBy(Functor&& func) const& -> LinqContainer {
        std::vector< Type, allocator_type > new_elements = elements;
        std::sort(new_elements.begin(), new_elements.end(), func);

        return LinqContainer { std::move(new_elements) };
    }

    template < std::predicate< Type > Functor >
    [[nodiscard]] auto Where(Functor&& func) && -> LinqContainer {
        LinqContainer new_elements {};
        new_elements.resize(this->size());
        auto count = Where_Internal(begin(), end(), new_elements.begin(), func);
        new_elements.resize(count);
        return std::move(new_elements);
    }
    template < std::predicate< Type > Functor >
    [[nodiscard]] auto Where(Functor&& func) const& -> LinqContainer {
        LinqContainer new_elements {};
        new_elements.resize(this->size());
        auto count = Where_Internal(begin(), end(), new_elements.begin(), func);
        new_elements.resize(count);
        return std::move(new_elements);
    }

    template < class Functor, class Ret = std::invoke_result_t< Functor, Type >, class Alloc = std::allocator_traits< allocator_type >::template rebind_alloc< Ret > >
    [[nodiscard]] auto Select(Functor&& func) && -> LinqContainer< Ret, Alloc > {
        LinqContainer< Ret, Alloc > new_elements {};
        new_elements.resize(this->size());
        Select_Internal(begin(), end(), new_elements.begin(), func);

        return std::move(new_elements);
    }
    template < class Functor, class Ret = std::invoke_result_t< Functor, Type >, class Alloc = std::allocator_traits< allocator_type >::template rebind_alloc< Ret > >
    [[nodiscard]] auto Select(Functor&& func) const& -> LinqContainer< Ret, Alloc > {
        LinqContainer< Ret, Alloc > new_elements {};
        new_elements.resize(this->size());
        Select_Internal(begin(), end(), new_elements.begin(), func);

        return std::move(new_elements);
    }

  private:
    template < class Init, class OutIt, class TInit, class Functor >
    [[nodiscard]] auto Where_Internal(const Init start, const OutIt last, TInit target, Functor&& func) const {
        auto       first_element  = start;
        const auto last_element   = last;
        auto       target_element = target;
        size_type  count          = 0;
        for (; first_element != last_element; first_element++) {
            if (func(*first_element)) {
                *target_element = *first_element;
                target_element++;
                count++;
            }
        }
        return count;
    }

    template < class Init, class OutIt, class TInit, class Functor >
    auto Select_Internal(const Init start, const OutIt last, TInit target, Functor&& func) const {
        auto       first_element  = start;
        const auto last_element   = last;
        auto       target_element = target;
        for (; first_element != last_element; first_element++, (void)target_element++) { *target_element = func(*first_element); }
    }

    std::vector< Type, Allocator > elements;
};

#endif LINQ_CONTAINER

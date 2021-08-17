// Traits.hpp: Define some helper type traits
//
// Created By: Michael Rizkalla
// Date:		02/04/2021

#ifndef TRAITS_FP
#define TRAITS_FP

#include <stdint.h>
#include <type_traits>
#include <utility>

namespace fp {

    template < typename Type >
    concept addable = requires(Type lhs, Type rhs) {
        lhs + rhs;
    };

    template < typename Type >
    concept dividable = requires(Type lhs, Type rhs) {
        lhs / rhs;
    };

    template < typename Type, typename T >
    concept has_compose = requires(Type type) {
        type.Compose(T {});
    };

    // Helper structs to track function arguments and return type
    template < class... >
    struct list {};
    template < class >
    struct gen_list;
    template < class... >
    struct pop_first_from_list;

    template < template < typename... > class List >
    struct pop_first_from_list< List<> > {
        using type = void;
    };
    template < template < typename... > class List, class FirstType, class... Types >
    struct pop_first_from_list< List< FirstType, Types... > > {
        using type = FirstType;
    };

    template < class... >
    struct count_list_element;
    template < template < typename... > class List >
    struct count_list_element< List<> > {
        static constexpr uint64_t value = 0;
    };
    template < template < typename... > class List, class Type >
    struct count_list_element< List< Type > > {
        static constexpr uint64_t value = 1;
    };
    template < template < typename... > class List, class FirstType, class... Types >
    struct count_list_element< List< FirstType, Types... > > {
        static constexpr uint64_t value = 1 + count_list_element< List< Types... > >::value;
    };

    template < class... >
    struct functor_traits {};

    namespace impl {
        template < class Ret, class... Args >
        struct functor_traits_eval {
            using return_type           = Ret;
            using argument_types        = list< Args... >;
            static constexpr auto value = true;
        };
    } // namespace impl

    template < template < class... > class Func, class Ret, class... Args >
    struct functor_traits< Func< Ret(Args...) > > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = false;
        static constexpr auto has_const    = false;
    };
    template < class Ret, class... Args >
    struct functor_traits< Ret (*)(Args...) > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = false;
        static constexpr auto has_const    = false;
    };
    template < class Ret, class... Args >
    struct functor_traits< Ret (*)(Args...) noexcept > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = true;
        static constexpr auto has_const    = false;
    };
    template < class Closure, class Ret, class... Args >
    struct functor_traits< Ret (Closure::*)(Args...) > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = false;
        static constexpr auto has_const    = false;
    };
    template < class Closure, class Ret, class... Args >
    struct functor_traits< Ret (Closure::*)(Args...) const > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = false;
        static constexpr auto has_const    = true;
    };
    template < class Closure, class Ret, class... Args >
    struct functor_traits< Ret (Closure::*)(Args...) noexcept > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = true;
        static constexpr auto has_const    = false;
    };
    template < class Closure, class Ret, class... Args >
    struct functor_traits< Ret (Closure::*)(Args...) const noexcept > : public impl::functor_traits_eval< Ret, Args... > {
        using typename impl::functor_traits_eval< Ret, Args... >::return_type;
        using typename impl::functor_traits_eval< Ret, Args... >::argument_types;
        using impl::functor_traits_eval< Ret, Args... >::value;
        static constexpr auto has_noexcept = true;
        static constexpr auto has_const    = true;
    };
    template < class Type >
    requires(!std::is_class_v< Type >) struct functor_traits< Type > : public impl::functor_traits_eval< Type > {
        using typename impl::functor_traits_eval< Type >::return_type;
        using typename impl::functor_traits_eval< Type >::argument_types;
        using impl::functor_traits_eval< Type >::value;
        static constexpr auto has_noexcept = false;
        static constexpr auto has_const    = false;
    };
    template < class Type >
    requires std::is_class_v< Type > struct functor_traits< Type > : public functor_traits< decltype(&Type::operator()) > {};

    // Type traits
    template < class FirstCallable, class SecondCallable >
    struct is_composable {
      public:
        using first_return                      = typename functor_traits< FirstCallable >::return_type;
        using second_args                       = typename functor_traits< SecondCallable >::argument_types;
        using first_of_second_args              = typename pop_first_from_list< second_args >::type;
        static constexpr auto second_args_count = count_list_element< second_args >::value;

      public:
        static constexpr auto value = std::same_as< std::remove_cvref_t< first_of_second_args >, std::remove_cvref_t< first_return > > && (second_args_count == 1);
    };

    template < class FirstCallable, class SecondCallable >
    concept composable = is_composable< FirstCallable, SecondCallable >::value;

} // namespace fp

#endif // TRAITS_FP

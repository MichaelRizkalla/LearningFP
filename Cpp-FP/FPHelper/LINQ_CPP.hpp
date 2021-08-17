

#ifndef LINQ_CPP_CONTAINER
#define LINQ_CPP_CONTAINER

#include <CompositionHelper.hpp>
#include <FPUtility.hpp>
#include <Traits.hpp>
#include <concepts>
#include <iterator>
#include <memory>
#include <type_traits>
#include <variant>
#include <vector>

namespace linq {

    template < class Type >
    using UniqueRef = std::unique_ptr< Type, std::function< void(Type*) > >;

    template < class Type >
    struct IEnumerator {
        /*using iterator_concept  = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;*/
        using value_type      = typename std::vector< Type >::value_type;
        using difference_type = typename std::vector< Type >::difference_type;
        using pointer         = typename std::vector< Type >::pointer;
        using const_pointer   = typename std::vector< Type >::const_pointer;
        using reference       = typename std::vector< Type >::reference;
        using const_reference = typename std::vector< Type >::const_reference;
        using size_type       = typename std::vector< Type >::size_type;
        using iterator        = typename std::vector< Type >::iterator;
        using const_iterator  = typename std::vector< Type >::const_iterator;

        virtual bool MoveNext() noexcept = 0;

        virtual value_type Current() const noexcept = 0;
    };

    template < class Type >
    class IEnumerable {
      public:
        virtual UniqueRef< IEnumerator< Type > > GetEnumerator() = 0;
    };

    template < class TSource, class TResult, class Allocator, class Func = fp::CompositionFunction< void(void), void > >
    struct Enumerator final : IEnumerator< TResult > {
      private:
        using Base = IEnumerator< TResult >;

      public:
        using typename Base::value_type;
        using typename Base::difference_type;
        using typename Base::pointer;
        using typename Base::const_pointer;
        using typename Base::reference;
        using typename Base::const_reference;
        using typename Base::size_type;
        using typename Base::iterator;
        using typename Base::const_iterator;

        using function_type = Func;

        Enumerator(std::initializer_list< TSource >&& data) :
            mData(std::allocate_shared< std::vector< TSource, Allocator > >(Allocator {}, data)), mIndex(-1), mTransformation(nullptr) {};
        Enumerator(std::initializer_list< TSource >&& data, size_type idx, Func&& transform) :
            mData(std::allocate_shared< std::vector< TSource, Allocator > >(Allocator {}, data)), mIndex(idx), mTransformation(transform) {};

        template < class OtherType, class OutputType, class OtherAlloc, class OtherFunc, class NewFunc >
        Enumerator(const Enumerator< OtherType, OutputType, OtherAlloc, OtherFunc >& enumerator, NewFunc&& transform) :
            mData(enumerator.mData), mIndex(enumerator.mIndex) {
            if constexpr (std::is_same_v< typename decltype(enumerator.mTransformation)::return_type, void >) {
                mTransformation = Func { transform };
            } else {
                mTransformation = enumerator.mTransformation.Compose(transform);
            }
        }

        bool MoveNext() noexcept override {
            if (++mIndex >= mData->size()) { return false; }
            return true;
        }

        TResult Current() const noexcept override {
            if constexpr (std::is_same_v< typename Func::return_type, void >) {
                return mData->at(mIndex);
            } else if constexpr (std::is_invocable_v< Func, TSource >) {
                return mTransformation(mData->at(mIndex));
            } else if constexpr (std::is_invocable_v< Func, TSource, size_type >) {
                return mTransformation(mData->at(mIndex), mIndex);
            }
        }

        void Reset() { mIndex = -1; }

        template < class OtherType, class OutputType, class OtherAlloc, class OtherFunc >
        friend class Enumerator;

      protected:
        std::shared_ptr< std::vector< TSource, Allocator > > mData;
        size_type                                            mIndex;
        Func                                                 mTransformation;
    };

    template < class TSource, class Allocator = std::allocator< TSource >, class TResult = TSource, int Controller = 0 >
    struct Enumerable final : IEnumerable< TResult > {
        using MyEnumerator =
            Enumerator< TSource, TResult, Allocator,
                        std::conditional_t< Controller == 0, fp::CompositionFunction< void(void), void >,
                                            std::conditional_t< Controller == 1, fp::CompositionFunction< TResult(TSource), void >,
                                                                fp::CompositionFunction< TResult(TSource, typename std::vector< TSource >::size_type), void > > > >;

      public:
        Enumerable(std::initializer_list< TSource > data) : mEnumerator { fp::allocate_unique< IEnumerator< TSource >, MyEnumerator >(Allocator {}, std::move(data)) } {}
        Enumerable(UniqueRef< IEnumerator< TResult > > enumerator) : mEnumerator(std::move(enumerator)) {}

        UniqueRef< IEnumerator< TResult > > GetEnumerator() override {
            return fp::allocate_unique< IEnumerator< TResult >, MyEnumerator >(Allocator {}, *reinterpret_cast< MyEnumerator* >(mEnumerator.get()));
        }

        template < class TAggregate, class Func, class Func2, class TResult = std::invoke_result_t< Func2, TAggregate > >
        requires(std::is_invocable_r_v< TAggregate, Func, TAggregate, TSource >) TResult Aggregate(TAggregate seed, Func&& func, Func2&& resultSelector) {
            TAggregate result = seed;
            while (mEnumerator->MoveNext()) { result = func(result, mEnumerator->Current()); }

            return resultSelector(result);
        }

        template < class TAccumulate, class Func, class TResult = std::invoke_result_t< Func, TAccumulate, TSource > >
        requires(std::same_as< TAccumulate, TResult >) TResult Aggregate(TAccumulate seed, Func&& func) {
            while (mEnumerator->MoveNext()) { seed = func(seed, mEnumerator->Current()); }

            return seed;
        }

        template < class Func >
        requires(std::is_invocable_r_v< TSource, Func, TSource, TSource >&& fp::addable< TSource >) TSource Aggregate(Func&& func) {
            TSource result {};
            if (mEnumerator->MoveNext()) { result = mEnumerator->Current(); }

            while (mEnumerator->MoveNext()) { result = func(result, mEnumerator->Current()); }

            return result;
        }

        template < class Func, class TResult_ = std::invoke_result_t< Func, TSource > >
        requires(std::is_invocable_v< Func, TSource >) auto Select(Func&& transform) {
            return Enumerable< TSource, Allocator, TResult_, 1 > {
                fp::allocate_unique< IEnumerator< TResult_ >, Enumerator< TSource, TResult_, Allocator, fp::CompositionFunction< TResult_(TSource), void > > >(
                    Allocator {}, *reinterpret_cast< MyEnumerator* >(mEnumerator.get()), transform)
            };
        }

        template < class Func, class TResult_ = std::invoke_result_t< Func, TSource, typename MyEnumerator::size_type > >
        requires(std::is_invocable_v< Func, TSource, typename MyEnumerator::size_type >) auto Select(Func&& transform) {
            return Enumerable< TSource, Allocator, TResult_, 2 > {
                fp::allocate_unique< IEnumerator< TResult_ >,
                                     Enumerator< TSource, TResult_, Allocator, fp::CompositionFunction< TResult_(TSource, typename MyEnumerator::size_type), void > > >(
                    Allocator {}, *reinterpret_cast< MyEnumerator* >(mEnumerator.get()), transform)
            };
        }

      private:
        UniqueRef< IEnumerator< TResult > > mEnumerator;
    };

} // namespace linq

#endif // LINQ_CPP_CONTAINER
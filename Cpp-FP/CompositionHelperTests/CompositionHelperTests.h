// CompositionHelperTests.h
// This contains unit tests to the implementation in CompositionHelper.hpp
//
// Created By: Michael Rizkalla
// Date:		02/04/2021

#ifndef COMPOSITION_HELPER_TESTS
#define COMPOSITION_HELPER_TESTS

#include <CompositionHelper.hpp>
#include <LinqContainer.hpp>
#include <array>
#include <cassert>
#include <limits>
#include <utility>

#define EXPECTED_RESULT(type, count_, ...)                     \
    auto results = std::array< type, count_ > { __VA_ARGS__ }; \
    auto count   = 0;

#define CHECK_RESULT(type, testedElement)        \
    [&count, &results](type element) {           \
        assert(testedElement == results[count]); \
        count++;                                 \
    }

double add(std::pair< double, double > x) noexcept {
    return x.first + x.second;
}
double square(double x) noexcept {
    return x * x;
}
double subtract_ten(double x) noexcept {
    return x - 10.;
}

void test_function_composition() {
    auto my_data = fp::LinqContainer< std::pair< double, double > > { { 3, 1 }, { 5, 1 }, { 7, 1 }, { 8, 1 } };
    EXPECTED_RESULT(double, 4, 6, 26, 54, 71)

    auto F = fp::CompositionFunction< decltype(add) > { add }.Compose(square).Compose(subtract_ten);

    my_data.Select(F).ForEach(CHECK_RESULT(double, element));
}

static auto lambda_add_one       = [](int x) { return x + 1; };
static auto lambda_int_to_double = [](int x) { return static_cast< double >(x); };
static auto lambda_square        = [](double x) { return x * x; };
static auto lambda_subtract_ten  = [](double x) { return x - 10.; };

void test_lambda_composition() {
    auto my_data = fp::LinqContainer< int > { 3, 5, 7, 8 };
    EXPECTED_RESULT(double, 4, 6, 26, 54, 71)

    auto F = fp::CompositionFunction< decltype(lambda_add_one) > { lambda_add_one }.Compose(lambda_int_to_double).Compose(lambda_square).Compose(lambda_subtract_ten);

    my_data.Select(F).ForEach(CHECK_RESULT(double, element));
}

void test_free_compose() {
    auto my_data = fp::LinqContainer< int > { 3, 5, 7, 8 };
    EXPECTED_RESULT(double, 4, 6, 26, 54, 71)

    auto F = fp::Compose(fp::Compose(lambda_add_one, lambda_int_to_double), lambda_square).Compose(lambda_subtract_ten);

    my_data.Select(F).ForEach(CHECK_RESULT(double, element));
}

enum class IngredientType : std::uint8_t {
    Flour = 1,
    Salad = 2,
    Meat  = 3,

    Undefined = std::numeric_limits< std::uint8_t >::max(),
};

struct Ingredient {
    IngredientType type = IngredientType::Undefined;
};
struct Cost {
    int cost = 0;
};
struct Food {
    int food = 0;
};
struct Calories {
    int calories = 0;
};
struct Weight {
    int weight = 0;
};

Cost calcCost(Ingredient ing) {
    return Cost { static_cast< std::uint8_t >(ing.type) * 10 };
}
struct Buyer {
    Food operator()(Cost c) const { return Food { c.cost / 10 }; }
};
static auto lambda_cook_food = [](Food f) { return Calories { (f.food % 2) * 1000 }; };
struct Scale {
    Weight operator()(Calories c) { return Weight { c.calories / 10 + 50 }; }
};

void test_combination() {
    auto my_data = fp::LinqContainer< Ingredient > { { IngredientType::Flour }, { IngredientType::Salad }, { IngredientType::Meat } };
    EXPECTED_RESULT(int, 3, 150, 50, 150)

    auto F = fp::CompositionFunction< decltype(calcCost) > { calcCost }.Compose(Buyer {}).Compose(lambda_cook_food).Compose(Scale {});

    my_data.Select(F).ForEach(CHECK_RESULT(Weight, element.weight));
}

// Traits
static_assert(std::is_nothrow_invocable_v< decltype(fp::CompositionFunction< decltype(add) > { add }.Compose(square)), std::pair< double, double > >);
static_assert(!std::is_nothrow_invocable_v< decltype(fp::CompositionFunction< decltype(calcCost) > { calcCost }.Compose(Buyer {})), Ingredient >);
static_assert(std::is_invocable_v< decltype(fp::CompositionFunction< decltype(calcCost) > { calcCost }.Compose(Buyer {})), Ingredient >);
static_assert(!std::is_invocable_v< decltype(fp::CompositionFunction< decltype(calcCost) > { calcCost }.Compose(Buyer {})), std::pair< double, double > >);
static_assert(std::is_invocable_v< decltype(fp::Compose(lambda_add_one, lambda_int_to_double)), int >);
static_assert(!std::is_invocable_v< decltype(fp::Compose(lambda_add_one, lambda_int_to_double)), Ingredient >);

#endif // COMPOSITION_HELPER_TESTS

// CalculateDiscountsOnOrders.h
// This project is an implementation from an example in:
// https://www.youtube.com/watch?v=6uwRajbkaqI
//
// Created By: Michael Rizkalla
// Date:		30/03/2021

#include "CalculateDiscountsOnOrders.h"

#include <LINQ_CPP.hpp>
#include <Traits.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>

int main() {
    // construct our application with fake 4 orders
    fp::Application app {};

    // get the discounts
    auto                     discounts = app.getOrdersWithDiscount({ {}, {}, {}, {} });
    std::vector< fp::Order > some_orders { {}, {}, {}, {} };
    auto                     more_discounts = app.getOrdersWithDiscount(some_orders);

    fp::CompositionFunction< void(void), void >::return_type;

    linq::Enumerable cont { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    auto             result = cont.Select([](int x) -> double { return static_cast< double >(x) * x; });

    auto enumerator = result.GetEnumerator();

    while (enumerator->MoveNext()) { std::cout << enumerator->Current() << '\n'; }
    linq::Enumerable fruits      = { "apple", "mango", "orange", "passionfruit", "grape" };
    std::string      longestName = fruits.Aggregate(
        "banana",

        [](auto longest, auto next) { return std::strlen(next) > std::strlen(longest) ? next : longest; },

        [](auto fruit) {
            auto str = std::string(fruit);
            int  ln  = str.length();

            for (int i = 0; i < ln; i++) {
                if (str[i] >= 'a' && str[i] <= 'z')
                    str[i] = str[i] - 32;
                else if (str[i] >= 'A' && str[i] <= 'Z')
                    str[i] = str[i] + 32;
            }

            return str;
        });

    std::cout << "The fruit with the longest name is " << longestName << '\n';

    linq::Enumerable ints { 4, 8, 8, 3, 9, 0, 7, 8, 2 };

    // Count the even numbers in the array, using a seed value of 0.
    int numEven = ints.Aggregate(0, [](auto total, auto next) { return next % 2 == 0 ? total + 1 : total; });

    std::cout << "The number of even integers is: " << numEven << '\n';

    std::string sentence = "the quick brown fox jumps over the lazy dog";
    // Split the string into individual words.
    linq::Enumerable< std::string > words = { "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" };

    // Prepend each word to the beginning of the
    // new sentence to reverse the word order.
    std::string reversed = words.Aggregate([](auto workingSentence, auto next) { return next + " " + workingSentence; });

    std::cout << reversed << '\n';
}

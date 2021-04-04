
#ifndef FP_UTILITY_HPP
#define FP_UTILITY_HPP
#include <cmath>
#include <string>

namespace fp {

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
        int w = (day + std::floor((13 * (mon + 1)) / 5) + y + std::floor(y / 4) + std::floor(c / 4) + (5 * c));
        w     = w % 7;
        return impl::weekday[w];
    }

} // namespace fp

#endif // FP_UTILITY_HPP
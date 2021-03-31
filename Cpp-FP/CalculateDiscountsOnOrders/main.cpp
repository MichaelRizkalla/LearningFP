// CalculateDiscountsOnOrders.h
// This project is an implementation from an example in:
// https://www.youtube.com/watch?v=6uwRajbkaqI
//
// Created By: Michael Rizkalla
// Date:		30/03/2021

#include "CalculateDiscountsOnOrders.h"

int main() {
    // construct our application with fake 4 orders
    Application app {};

    // get the discounts
    auto                 discounts = app.getOrdersWithDiscount({ {}, {}, {}, {} });
    std::vector< Order > some_orders { {}, {}, {}, {} };
    auto                 more_discounts = app.getOrdersWithDiscount(some_orders);
}

// CompositionExample.h
// This project is an implementation from an example in:
// https://www.youtube.com/watch?v=Svi1N-lbzpI
//
// Created By: Michael Rizkalla
// Date:		03/04/2021

#include "CompositionExample.h"

int main() {
    using namespace fpExample;

    Application app {};

    ProcessConfiguration config {};
    Customer             customer {};
    Order                order {};

    config.invoiceChoice      = InvoiceChoice::Inv3;
    config.shippingChoice     = ShippingChoice::Sh2;
    config.freightChoice      = FreightChoice::fr3;
    config.availabilityChoice = AvailabilityChoice::AV2;
    config.shippingDateChoice = ShippingDateChoice::SD2;

    order.customer  = customer;
    order.date.Date = std::chrono::day(16) / std::chrono::March / 2021;
    order.cost      = 2000;

    auto CostOfOrder = app.CalcAdjustedCostOfOrder(config, InvoicingPath {}, AvailabilityPath {});

    std::cout << "Cost of order:" << CostOfOrder(order);
}

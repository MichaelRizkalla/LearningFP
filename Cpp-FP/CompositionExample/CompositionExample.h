// CompositionExample.h
// This project is an implementation from an example in:
// https://www.youtube.com/watch?v=Svi1N-lbzpI
//
// Created By: Michael Rizkalla
// Date:		03/04/2021

#ifndef COMPOSITION_EXAMPLE_
#define COMPOSITION_EXAMPLE_

#include <CompositionHelper.hpp>
#include <LinqContainer.hpp>
#include <stdint.h>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include "CompositionExampleTypes.h"
#include <FPUtility.hpp>

namespace fpExample {

    class Application {
      private:
        static decltype(auto) InvoicePathFunc(ProcessConfiguration config, InvoicingPath invPath) {
            auto p = fp::Compose(invPath.invoiceFunctions.Where([&](InvoiceChooser x) { return x.invoiceChoice == config.invoiceChoice; })
                                     .Select([](InvoiceChooser x) { return x.calcInvoice; })
                                     .FirstOrDefault(),
                                 invPath.shippingFunctions.Where([&](ShippingChooser x) { return x.shippingChoice == config.shippingChoice; })
                                     .Select([](ShippingChooser x) { return x.calcShipping; })
                                     .FirstOrDefault())
                         .Compose(invPath.freightFunctions.Where([&](FreightChooser x) { return x.freightChoice == config.freightChoice; })
                                      .Select([](FreightChooser x) { return x.calcFreight; })
                                      .FirstOrDefault());

            return p;
        }

        static decltype(auto) AvailabilityPathFunc(ProcessConfiguration config, AvailabilityPath avPath) {
            auto p = fp::Compose(avPath.availabilityFunctions.Where([&](AvailabilityChooser x) { return x.availabilityChoice == config.availabilityChoice; })
                                     .Select([](AvailabilityChooser x) { return x.calcAvailability; })
                                     .FirstOrDefault(),
                                 avPath.shippingDateFunctions.Where([&](ShippingDateChooser x) { return x.shippingDateChoice == config.shippingDateChoice; })
                                     .Select([](ShippingDateChooser x) { return x.calcShippingDate; })
                                     .FirstOrDefault());

            return p;
        }

      public:
        Application()  = default;
        ~Application() = default;

        decltype(auto) CalcAdjustedCostOfOrder(ProcessConfiguration config, InvoicingPath invPath, AvailabilityPath avPath) const noexcept {
            auto AdjustCost = [](Order r, auto&& fr, auto&& sh) noexcept {
                auto f         = fr(r);
                auto s         = sh(r);
                auto dayString = fp::zellersAlgorithm(uint32_t(s.date.Date.day()), uint32_t(s.date.Date.month()), int { s.date.Date.year() });
                std::cout << "\n\nDate of shipping: " << dayString << '\n';

                double cost = dayString == "Monday" ? f.cost + 1000 : f.cost + 500;

                return cost;
            };

            auto return_function = [&](Order r) { return AdjustCost(r, InvoicePathFunc(config, invPath), AvailabilityPathFunc(config, avPath)); };

            return return_function;
        }
    };

} // namespace fpExample

#endif // COMPOSITION_EXAMPLE_

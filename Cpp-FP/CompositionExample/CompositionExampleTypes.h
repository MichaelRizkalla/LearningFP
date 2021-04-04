
#ifndef COMPOSITION_EXAMPLE_TYPES_
#define COMPOSITION_EXAMPLE_TYPES_

#include <CompositionHelper.hpp>
#include <LinqContainer.hpp>
#include <stdint.h>

namespace fpExample {

    template < class Signature >
    using fp_function = fp::CompositionFunction< Signature, void >;

    enum class InvoiceChoice : std::uint8_t { Inv1 = 0, Inv2 = 1, Inv3 = 2, Inv4 = 3, Inv5 = 4 };

    enum class ShippingChoice : std::uint8_t { Sh1, Sh2, Sh3 };

    enum class FreightChoice : std::uint8_t { fr1, fr2, fr3, fr4, fr5, fr6 };

    enum class AvailabilityChoice : std::uint8_t { AV1, AV2, AV3, AV4 };

    enum class ShippingDateChoice : std::uint8_t { SD1, SD2, SD3, SD4, SD5 };

    struct ProcessConfiguration {
        InvoiceChoice      invoiceChoice;
        ShippingChoice     shippingChoice;
        FreightChoice      freightChoice;
        AvailabilityChoice availabilityChoice;
        ShippingDateChoice shippingDateChoice;
    };

    struct DateTime {
        std::chrono::year_month_day                 Date;
        std::chrono::hh_mm_ss< std::chrono::hours > Time;
    };

    struct Customer {};

    struct Order {
        Customer customer {};
        DateTime date {};
        double   cost {};
    };

    struct Invoice {
        double cost { 0 };
    };

    struct Shipping {
        double cost { 0 };
        int    shipperID { 0 };
    };

    struct Freight {
        double cost { 0 };
    };

    struct Availability {
        DateTime date {};
    };

    struct ShippingDate {
        DateTime date {};
    };

    // Functions
    struct InvoiceFunction {
        static auto calcInvoice1(Order o) noexcept {
            std::cout << "Invioce 1\n";
            Invoice invoice {};
            invoice.cost = o.cost * 1.1;
            return invoice;
        }
        static auto calcInvoice2(Order o) noexcept {
            std::cout << "Invioce 2\n";
            Invoice invoice {};
            invoice.cost = o.cost * 1.2;
            return invoice;
        }
        static auto calcInvoice3(Order o) noexcept {
            std::cout << "Invioce 3\n";
            Invoice invoice {};
            invoice.cost = o.cost * 1.3;
            return invoice;
        }
        static auto calcInvoice4(Order o) noexcept {
            std::cout << "Invioce 4\n";
            Invoice invoice {};
            invoice.cost = o.cost * 1.4;
            return invoice;
        }
        static auto calcInvoice5(Order o) noexcept {
            std::cout << "Invioce 5\n";
            Invoice invoice {};
            invoice.cost = o.cost * 1.5;
            return invoice;
        }
    };
    struct ShippingFunction {
        static auto calShipping1(Invoice i) noexcept {
            std::cout << "Shipping 1\n";
            Shipping s {};
            s.shipperID = (i.cost > 1000) ? 1 : 2;
            s.cost      = i.cost;

            return s;
        }
        static auto calShipping2(Invoice i) noexcept {
            std::cout << "Shipping 2\n";
            Shipping s {};
            s.shipperID = (i.cost > 1100) ? 1 : 2;
            s.cost      = i.cost;

            return s;
        }
        static auto calShipping3(Invoice i) noexcept {
            std::cout << "Shipping 3\n";
            Shipping s {};
            s.shipperID = (i.cost > 1200) ? 1 : 2;
            s.cost      = i.cost;

            return s;
        }
    };
    struct FreightFunction {
        static auto calcFreightCost1(Shipping s) noexcept {
            std::cout << "Freight 1\n";
            Freight f {};
            f.cost = (s.shipperID == 1) ? s.cost * 0.25 : s.cost * 0.5;
            return f;
        }
        static auto calcFreightCost2(Shipping s) noexcept {
            std::cout << "Freight 2\n";
            Freight f {};
            f.cost = (s.shipperID == 1) ? s.cost * 0.28 : s.cost * 0.52;
            return f;
        }
        static auto calcFreightCost3(Shipping s) noexcept {
            std::cout << "Freight 3\n";
            Freight f {};
            f.cost = (s.shipperID == 1) ? s.cost * 0.3 : s.cost * 0.6;
            return f;
        }
        static auto calcFreightCost4(Shipping s) noexcept {
            std::cout << "Freight 4\n";
            Freight f {};
            f.cost = (s.shipperID == 1) ? s.cost * 0.35 : s.cost * 0.65;
            return f;
        }
        static auto calcFreightCost5(Shipping s) noexcept {
            std::cout << "Freight 5\n";
            Freight f {};
            f.cost = (s.shipperID == 1) ? s.cost * 0.15 : s.cost * 0.2;
            return f;
        }
        static auto calcFreightCost6(Shipping s) noexcept {
            std::cout << "Freight 6\n";
            Freight f {};
            f.cost = (s.shipperID == 1) ? s.cost * 0.1 : s.cost * 0.15;
            return f;
        }
    };
    struct AvailabilityFunction {
        static auto calcAvailability1(Order o) noexcept {
            std::cout << "Availability 1\n";
            Availability a {};
            a.date      = o.date;
            a.date.Date = std::chrono::sys_days { a.date.Date } + std::chrono::days(3);
            return a;
        }
        static auto calcAvailability2(Order o) noexcept {
            std::cout << "Availability 2\n";
            Availability a {};
            a.date      = o.date;
            a.date.Date = std::chrono::sys_days { a.date.Date } + std::chrono::days(2);
            return a;
        }
        static auto calcAvailability3(Order o) noexcept {
            std::cout << "Availability 3\n";
            Availability a {};
            a.date      = o.date;
            a.date.Date = std::chrono::sys_days { a.date.Date } + std::chrono::days(1);
            return a;
        }
        static auto calcAvailability4(Order o) noexcept {
            std::cout << "Availability 4\n";
            Availability a {};
            a.date      = o.date;
            a.date.Date = std::chrono::sys_days { a.date.Date } + std::chrono::days(4);
            return a;
        }
    };
    struct ShippingDateFunction {
        static auto calcShippingDate1(Availability a) noexcept {
            std::cout << "ShippingDate 1\n";
            ShippingDate s {};
            s.date      = a.date;
            s.date.Date = std::chrono::sys_days { s.date.Date } + std::chrono::days(1);

            return s;
        }
        static auto calcShippingDate2(Availability a) noexcept {
            std::cout << "ShippingDate 2\n";
            ShippingDate s {};
            s.date      = a.date;
            s.date.Date = std::chrono::sys_days { s.date.Date } + std::chrono::days(2);

            return s;
        }
        static auto calcShippingDate3(Availability a) noexcept {
            std::cout << "ShippingDate 3\n";
            ShippingDate s {};
            s.date      = a.date;
            s.date.Time = std::chrono::hh_mm_ss< std::chrono::hours > { s.date.Time.hours() + std::chrono::hours(14) };

            return s;
        }
        static auto calcShippingDate4(Availability a) noexcept {
            std::cout << "ShippingDate 4\n";
            ShippingDate s {};
            s.date      = a.date;
            s.date.Time = std::chrono::hh_mm_ss< std::chrono::hours > { s.date.Time.hours() + std::chrono::hours(20) };

            return s;
        }
        static auto calcShippingDate5(Availability a) noexcept {
            std::cout << "ShippingDate 5\n";
            ShippingDate s {};
            s.date      = a.date;
            s.date.Time = std::chrono::hh_mm_ss< std::chrono::hours > { s.date.Time.hours() + std::chrono::hours(10) };

            return s;
        }
    };

    // Paths
    struct InvoiceChooser {
        InvoiceChoice                          invoiceChoice;
        fp_function< Invoice(Order) noexcept > calcInvoice;
    };
    struct ShippingChooser {
        ShippingChoice                            shippingChoice;
        fp_function< Shipping(Invoice) noexcept > calcShipping;
    };
    struct FreightChooser {
        FreightChoice                             freightChoice;
        fp_function< Freight(Shipping) noexcept > calcFreight;
    };
    struct InvoicingPath {
        inline static const fp::LinqContainer< InvoiceChooser > invoiceFunctions {
            { InvoiceChoice ::Inv1, InvoiceFunction::calcInvoice1 }, { InvoiceChoice ::Inv2, InvoiceFunction::calcInvoice2 },
            { InvoiceChoice ::Inv3, InvoiceFunction::calcInvoice3 }, { InvoiceChoice ::Inv4, InvoiceFunction::calcInvoice4 },
            { InvoiceChoice ::Inv5, InvoiceFunction::calcInvoice5 },
        };
        inline static const fp::LinqContainer< ShippingChooser > shippingFunctions {
            { ShippingChoice::Sh1, ShippingFunction::calShipping1 },
            { ShippingChoice::Sh2, ShippingFunction::calShipping2 },
            { ShippingChoice::Sh3, ShippingFunction::calShipping3 },
        };
        inline static const fp::LinqContainer< FreightChooser > freightFunctions {
            { FreightChoice::fr1, FreightFunction::calcFreightCost1 }, { FreightChoice::fr2, FreightFunction::calcFreightCost2 },
            { FreightChoice::fr3, FreightFunction::calcFreightCost3 }, { FreightChoice::fr4, FreightFunction::calcFreightCost4 },
            { FreightChoice::fr5, FreightFunction::calcFreightCost5 }, { FreightChoice::fr6, FreightFunction::calcFreightCost6 },
        };
    };

    struct AvailabilityChooser {
        AvailabilityChoice                          availabilityChoice;
        fp_function< Availability(Order) noexcept > calcAvailability;
    };
    struct ShippingDateChooser {
        ShippingDateChoice                                 shippingDateChoice;
        fp_function< ShippingDate(Availability) noexcept > calcShippingDate;
    };
    struct AvailabilityPath {
        inline static const fp::LinqContainer< AvailabilityChooser > availabilityFunctions {
            { AvailabilityChoice ::AV1, AvailabilityFunction::calcAvailability1 },
            { AvailabilityChoice ::AV2, AvailabilityFunction::calcAvailability2 },
            { AvailabilityChoice ::AV3, AvailabilityFunction::calcAvailability3 },
            { AvailabilityChoice ::AV4, AvailabilityFunction::calcAvailability4 },
        };
        inline static const fp::LinqContainer< ShippingDateChooser > shippingDateFunctions {
            { ShippingDateChoice::SD1, ShippingDateFunction::calcShippingDate1 }, { ShippingDateChoice::SD2, ShippingDateFunction::calcShippingDate2 },
            { ShippingDateChoice::SD3, ShippingDateFunction::calcShippingDate3 }, { ShippingDateChoice::SD4, ShippingDateFunction::calcShippingDate4 },
            { ShippingDateChoice::SD5, ShippingDateFunction::calcShippingDate5 },
        };
    };

} // namespace fpExample

#endif // COMPOSITION_EXAMPLE_TYPES_
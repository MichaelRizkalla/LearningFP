// CalculateDiscountsOnOrders.h
// This project is an implementation from an example in:
// https://www.youtube.com/watch?v=6uwRajbkaqI
// 
// Created By: Michael Rizkalla 
// Date:		30/03/2021

#ifndef CALCULATE_DISCOUNTS_ON_ORDERS
#define CALCULATE_DISCOUNTS_ON_ORDERS
// Not using #pragma once since it's not a part of the standard
#include <functional>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
#include "LinqContainer.h"

// C++ does not have an equivalent to C# System.Decimal
using decimal = long double;

struct Order {
	Order(decimal discount_ = 0) : discount(discount_) {}
	Order& operator=(const Order& rhs) { // Quick work-around for const members, not the best practice.
		std::memcpy(this, &rhs, sizeof(Order));

		return *this;
	}
private:
	const decimal discount;
};

using QualifierFunc = std::function<bool(const Order&)>;
using DiscountFunc = std::function<decimal(const Order&)>;
using Rule = std::pair<QualifierFunc, DiscountFunc>;

class Application {
public:
	Application() = default;
	~Application() = default;

	LinqContainer<Order> getOrdersWithDiscount(LinqContainer<Order>&& ordersToProcess) const {
		return ordersToProcess.Select([order_rules = GetDiscountRules()](const auto order) { return Run(order, order_rules); });
	}

	LinqContainer<Order> getOrdersWithDiscount(const LinqContainer<Order>& ordersToProcess) const {
		return ordersToProcess.Select([order_rules = GetDiscountRules()](const auto order) { return Run(order, order_rules); });
	}

	const LinqContainer<Rule>& GetDiscountRules() const {
		return rules;
	}

protected:
	static Order Run(const Order& r, const LinqContainer<Rule>& rules) {
		const auto discount = rules.Where([&r](const auto rule) { return rule.first(r); }).Select([&r](const auto rule) { return rule.second(r); }).OrderBy(std::less{}).Take(3).Average();
		const auto newOrder = Order{ discount };

		return newOrder;
	}

	// Add more rules as convenient
	const LinqContainer<Rule> rules{ {
			{ Rule{QualifierFunc{[](const Order&) -> bool {return true; }}, DiscountFunc{[](const Order&)->decimal {return 10.; }}} },
			{ Rule{QualifierFunc{[](const Order&) -> bool {return true; }}, DiscountFunc{[](const Order&)->decimal {return 1.; }}} },
			{ Rule{QualifierFunc{[](const Order&) -> bool {return true; }}, DiscountFunc{[](const Order&)->decimal {return 3.; }}} }
	} };
};

#endif // CALCULATE_DISCOUNTS_ON_ORDERS

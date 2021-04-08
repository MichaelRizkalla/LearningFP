#![allow(non_snake_case)]

use std::vec::Vec;
use float_ord::FloatOrd;
use linq::iter::Enumerable;

type Decimal = f64;
type QualifierFn = fn(&Order)->bool;
type DiscountFn = fn(&Order)->Decimal;
type Rule = (QualifierFn, DiscountFn);

struct Order {
    discount: Decimal,
}

impl Order{
    pub fn new_with_discount(discount_ : Decimal) -> Self {
        Order {
            discount: discount_,
        }
    }

    pub fn new() -> Self {
        Order {
            discount: 0.,
        }
    }
}

#[allow(unused_macros)]
macro_rules! order {
    () => {
        Order::new()
    };
    ($a: expr) => {
        Order::new_with_discount($a)
    };
}

pub trait Average {
    fn average(self) -> Decimal;
}

impl<F, T> Average for T
    where T: Iterator<Item = F>,
          F: std::borrow::Borrow<Decimal>
{
    fn average(self) -> Decimal {
        self.zip(1..)
            .fold(0.,
                  |s, (e, i)| (*e.borrow() + s * (i - 1) as Decimal) / i as Decimal)
    }
}

struct Application {}

impl Application {
    pub fn new() -> Self{
        Application{}
    }

    fn get_discount_rules() -> Vec<Rule> {
        let array : Vec<Rule> = vec![
            (|_order: &Order| {true}, |_order: &Order| {10.}),
            (|_order: &Order| {false}, |_order: &Order| {1.}),
            (|_order: &Order| {true}, |_order: &Order| {5.}),
            (|_order: &Order| {false}, |_order: &Order| {20.}),
            (|_order: &Order| {true}, |_order: &Order| {2.}),
            (|_order: &Order| {true}, |_order: &Order| {3.}),
        ];
        array
    }

    fn run(r: &Order, rules: Vec<Rule>) -> Order {
        let discount = rules.iter()
        .where_by(|rule: &&Rule| rule.0(r))
        .select(|rule: &Rule| rule.1(r))
        .order_by(|lhs| FloatOrd::<f64>(*lhs))
        .collect::<Vec<_>>().as_slice()[0..3].to_vec().iter()
        .average();

        order![discount]
    }

    pub fn get_order_with_discount(&self, orders_to_process: Vec<Order>) -> Vec<Order> {
        orders_to_process.iter().select(|order| Application::run(order, Application::get_discount_rules())).collect()
    }
}

fn main() {
    let app = Application::new();

    let orders = vec![Order::new(), Order::new(), Order::new(), Order::new()];

    let result = app.get_order_with_discount(orders);

    println!("discount 1: {0}", result[0].discount);
    println!("discount 2: {0}", result[1].discount);
    println!("discount 3: {0}", result[2].discount);
    println!("discount 4: {0}", result[3].discount);
}


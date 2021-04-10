use chrono::prelude::*;
use linq::iter::Enumerable;

pub trait Compose<'a, A, B, C, Z> {
    fn compose(self, outer_function: Z) -> Box<dyn Fn(A) -> C + 'a>
    where 
        Self: Fn(A) -> B + 'a,
        Z: Fn(B) -> C + 'a;
} 

impl<'a, A, B, C, Z, T> Compose<'a, A, B, C, Z> for T
where
    T: Fn(A) -> B,
{
    fn compose(self, outer_function: Z) -> Box<dyn Fn(A) -> C + 'a>
    where 
        Self: Fn(A) -> B + 'a,
        Z: Fn(B) -> C + 'a,
    {
        Box::new(move |a| outer_function(self(a)))
    }
}

#[derive(PartialEq, Copy, Clone)]
enum InvoiceChoice { Inv1 = 0, Inv2 = 1, Inv3 = 2, Inv4 = 3, Inv5 = 4 }

#[derive(PartialEq, Copy, Clone)]
enum ShippingChoice { Sh1, Sh2, Sh3 }

#[derive(PartialEq, Copy, Clone)]
enum FreightChoice { Fr1, Fr2, Fr3, Fr4, Fr5, Fr6 }

#[derive(PartialEq, Copy, Clone)]
enum AvailabilityChoice { AV1, AV2, AV3, AV4 }

#[derive(PartialEq, Copy, Clone)]
enum ShippingDateChoice  { SD1, SD2, SD3, SD4, SD5 }

#[derive(Copy, Clone)]
struct ProcessConfiguration {
    invoice_choice: InvoiceChoice,
    shipping_choice: ShippingChoice,
    freight_choice: FreightChoice,
    availability_choice: AvailabilityChoice,
    shippingdate_choice: ShippingDateChoice,
}

impl ProcessConfiguration{
    pub fn new() -> Self {
        ProcessConfiguration {
            invoice_choice: InvoiceChoice::Inv1,
            shipping_choice: ShippingChoice::Sh1,
            freight_choice: FreightChoice::Fr1,
            availability_choice: AvailabilityChoice::AV1,
            shippingdate_choice: ShippingDateChoice::SD1,
        }
    }
}

struct MyDateTime {
    date: DateTime<Utc>,
}

impl MyDateTime {
    pub fn new() -> Self {
        MyDateTime {
            date: Utc::now()
        }
    }
}

#[derive(Copy, Clone)]
struct Customer {}

impl Customer {
    pub fn new() -> Self {
        Customer {

        }
    }
}

struct Order {
    customer: Customer,
    date: MyDateTime,
    cost: f64,
}

impl Order {
    pub fn new() -> Self {
        Order {
            customer: Customer::new(),
            date: MyDateTime::new(),
            cost : 0.,
        }
    }
}

#[derive(Copy, Clone)]
struct Invoice {
    cost: f64,
}

impl Invoice {
    pub fn new() -> Self {
        Invoice {
            cost: 0.,
        }
    }
}

struct Shipping{
    cost: f64,
    shipper_id: i64,
}

impl Shipping {
    pub fn new() -> Self {
        Shipping{
            cost: 0.,
            shipper_id: 0,
        }
    }
}

#[derive(Copy, Clone)]
struct Freight{
    cost: f64,
}

impl Freight {
    pub fn new() ->Self {
        Freight{
            cost: 0.,
        }
    }
}

struct Availability{
    date: MyDateTime,
}

impl Availability {
    pub fn new() -> Self {
        Availability{
            date: MyDateTime::new(),
        }
    }
}

struct ShippingDate {
    date: MyDateTime,
}

impl ShippingDate {
    pub fn new() -> Self {
        ShippingDate {
            date: MyDateTime::new(),
        }
    }
}

#[derive(Copy, Clone)]
struct InvoiceFunctions {}

impl InvoiceFunctions {
    pub fn calc_invoice1(o: &Order) -> Invoice {
        println!("Invoice 1");
        let mut invoice = Invoice::new();
        invoice.cost = o.cost * 1.1;
        invoice
    }
    pub fn calc_invoice2(o: &Order) -> Invoice {
        println!("Invoice 2");
        let mut invoice = Invoice::new();
        invoice.cost = o.cost * 1.2;
        invoice
    }
    pub fn calc_invoice3(o: &Order) -> Invoice {
        println!("Invoice 3");
        let mut invoice = Invoice::new();
        invoice.cost = o.cost * 1.3;
        invoice
    }
    pub fn calc_invoice4(o: &Order) -> Invoice {
        println!("Invoice 4");
        let mut invoice = Invoice::new();
        invoice.cost = o.cost * 1.4;
        invoice
    }
    pub fn calc_invoice5(o: &Order) -> Invoice {
        println!("Invoice 5");
        let mut invoice = Invoice::new();
        invoice.cost = o.cost * 1.5;
        invoice
    }
}

#[derive(Copy, Clone)]
struct ShippingFunctions {}

impl ShippingFunctions{
    pub fn calc_shipping1(i: Invoice) -> Shipping {
        println!("Shipping 1");
        let mut s = Shipping::new();
        s.shipper_id = if i.cost > 1000. { 1 } else { 2 };
        s.cost = i.cost;
        s
    }
    pub fn calc_shipping2(i: Invoice) -> Shipping {
        println!("Shipping 2");
        let mut s = Shipping::new();
        s.shipper_id = if i.cost > 1100. { 1 } else { 2 };
        s.cost = i.cost;
        s
    }
    pub fn calc_shipping3(i: Invoice) -> Shipping {
        println!("Shipping 3");
        let mut s = Shipping::new();
        s.shipper_id = if i.cost > 1200. { 1 } else { 2 };
        s.cost = i.cost;
        s
    }
}

#[derive(Copy, Clone)]
struct FreightFunctions {}

impl FreightFunctions {
    pub fn calc_freight_cost1(s: Shipping) -> Freight {
        println!("Freight 1");
        let mut f = Freight::new();
        f.cost = if s.shipper_id == 1 { s.cost * 0.25 } else { s.cost * 0.5 };
        f
    }
    pub fn calc_freight_cost2(s: Shipping) -> Freight {
        println!("Freight 2");
        let mut f = Freight::new();
        f.cost = if s.shipper_id == 1 { s.cost * 0.28 } else { s.cost * 0.52 };
        f
    }
    pub fn calc_freight_cost3(s: Shipping) -> Freight {
        println!("Freight 3");
        let mut f = Freight::new();
        f.cost = if s.shipper_id == 1 { s.cost * 0.3 } else { s.cost * 0.6 };
        f
    }
    pub fn calc_freight_cost4(s: Shipping) -> Freight {
        println!("Freight 4");
        let mut f = Freight::new();
        f.cost = if s.shipper_id == 1 { s.cost * 0.35 } else { s.cost * 0.65 };
        f
    }
    pub fn calc_freight_cost5(s: Shipping) -> Freight {
        println!("Freight 5");
        let mut f = Freight::new();
        f.cost = if s.shipper_id == 1 { s.cost * 0.15 } else { s.cost * 0.2 };
        f
    }
    pub fn calc_freight_cost6(s: Shipping) -> Freight {
        println!("Freight 6");
        let mut f = Freight::new();
        f.cost = if s.shipper_id == 1 { s.cost * 0.1 } else { s.cost * 0.15 };
        f
    }
}

#[derive(Copy, Clone)]
struct AvailabilityFunctions {}

impl AvailabilityFunctions {
    pub fn calc_availability1(o: &Order) -> Availability {
        println!("Availability 1");
        let mut a = Availability::new();
        match o.date.date.with_day(o.date.date.day() + 3){
            Some(x) => a.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        a
    }
    pub fn calc_availability2(o: &Order) -> Availability {
        println!("Availability 2");
        let mut a = Availability::new();
        match o.date.date.with_day(o.date.date.day() + 2){
            Some(x) => a.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        a
    }
    pub fn calc_availability3(o: &Order) -> Availability {
        println!("Availability 3");
        let mut a = Availability::new();
        match o.date.date.with_day(o.date.date.day() + 1){
            Some(x) => a.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        a
    }
    pub fn calc_availability4(o: &Order) -> Availability {
        println!("Availability 4");
        let mut a = Availability::new();
        match o.date.date.with_day(o.date.date.day() + 4){
            Some(x) => a.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        a
    }
}

#[derive(Copy, Clone)]
struct ShippingDateFunctions {}

impl ShippingDateFunctions {
    pub fn calc_shippingdate1(a: Availability) -> ShippingDate {
        println!("ShippingDate 1");
        let mut s = ShippingDate::new();
        match a.date.date.with_day(a.date.date.day() + 1) {
            Some(x) => s.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        s
    }
    pub fn calc_shippingdate2(a: Availability) -> ShippingDate {
        println!("ShippingDate 2");
        let mut s = ShippingDate::new();
        match a.date.date.with_day(a.date.date.day() + 2) {
            Some(x) => s.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        s
    }
    pub fn calc_shippingdate3(a: Availability) -> ShippingDate {
        println!("ShippingDate 3");
        let mut s = ShippingDate::new();
        match a.date.date.with_hour(a.date.date.hour() + 14) {
            Some(x) => s.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        s
    }
    pub fn calc_shippingdate4(a: Availability) -> ShippingDate {
        println!("ShippingDate 4");
        let mut s = ShippingDate::new();
        match a.date.date.with_hour(a.date.date.hour() + 20) {
            Some(x) => s.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        s
    }
    pub fn calc_shippingdate5(a: Availability) -> ShippingDate {
        println!("ShippingDate 5");
        let mut s = ShippingDate::new();
        match a.date.date.with_hour(a.date.date.hour() + 10) {
            Some(x) => s.date.date = x,
            None => panic!("Failed to increment date!"),
        }
        s
    }
}

struct InvoiceChooser {
    invoice_choice: InvoiceChoice,
    calc_invoice: for<'a> fn(&'a Order) -> Invoice,
}

impl InvoiceChooser {
    pub fn new(choice: InvoiceChoice, f: for <'a> fn(&'a Order)->Invoice) -> Self
    {
        InvoiceChooser {
            invoice_choice: choice,
            calc_invoice: f,
        }
    }
}

struct ShippingChooser {
    shipping_choice: ShippingChoice,
    calc_shipping: fn(Invoice) -> Shipping,
}

impl ShippingChooser {
    pub fn new(choice: ShippingChoice, f: fn(Invoice)->Shipping) -> Self {
        ShippingChooser {
            shipping_choice: choice,
            calc_shipping: f,
        }
    }
}

struct FreightChooser {
    freight_choice: FreightChoice,
    calc_freight: fn(Shipping) -> Freight,
}

impl FreightChooser {
    pub fn new(choice: FreightChoice, f: fn(Shipping) -> Freight) -> Self {
        FreightChooser {
            freight_choice: choice,
            calc_freight: f,
        }
    }
}

#[derive(Copy, Clone)]
struct InvoicingPath {} 

impl InvoicingPath {
    pub fn new() -> Self{
        InvoicingPath{}
    }
    pub fn invoice_functions(&self) -> Vec<InvoiceChooser> {
        vec![InvoiceChooser::new(InvoiceChoice::Inv1, InvoiceFunctions::calc_invoice1),
                InvoiceChooser::new(InvoiceChoice::Inv2, InvoiceFunctions::calc_invoice2),
                InvoiceChooser::new(InvoiceChoice::Inv3, InvoiceFunctions::calc_invoice3),
                InvoiceChooser::new(InvoiceChoice::Inv4, InvoiceFunctions::calc_invoice4),
                InvoiceChooser::new(InvoiceChoice::Inv5, InvoiceFunctions::calc_invoice5),
        ]
    }
    pub fn shipping_functions(&self) -> Vec<ShippingChooser> {
        vec![ShippingChooser::new(ShippingChoice::Sh1, ShippingFunctions::calc_shipping1),
                ShippingChooser::new(ShippingChoice::Sh2, ShippingFunctions::calc_shipping2),
                ShippingChooser::new(ShippingChoice::Sh3, ShippingFunctions::calc_shipping3),
        ]
    }
    pub fn freight_functions(&self) -> Vec<FreightChooser> {
        vec![FreightChooser::new(FreightChoice::Fr1, FreightFunctions::calc_freight_cost1),
                FreightChooser::new(FreightChoice::Fr2, FreightFunctions::calc_freight_cost2),
                FreightChooser::new(FreightChoice::Fr3, FreightFunctions::calc_freight_cost3),
                FreightChooser::new(FreightChoice::Fr4, FreightFunctions::calc_freight_cost4),
                FreightChooser::new(FreightChoice::Fr5, FreightFunctions::calc_freight_cost5),
                FreightChooser::new(FreightChoice::Fr6, FreightFunctions::calc_freight_cost6),
        ]
    }
}

struct AvailabilityChooser {
    availability_choice: AvailabilityChoice,
    calc_availability: for <'a> fn(&'a Order) -> Availability,
}

impl AvailabilityChooser {
    pub fn new(choice: AvailabilityChoice, f: for <'a> fn(&'a Order) -> Availability) -> Self {
        AvailabilityChooser {
            availability_choice: choice,
            calc_availability: f,
        }
    }
}

struct ShippingDateChooser {
    shippingdate_choice: ShippingDateChoice,
    calc_shippingdate: fn(Availability) -> ShippingDate,
}

impl ShippingDateChooser {
    pub fn new(choice: ShippingDateChoice, f: fn(Availability) -> ShippingDate) -> Self {
        ShippingDateChooser {
            shippingdate_choice: choice,
            calc_shippingdate: f,
        }
    }
}

#[derive(Copy, Clone)]
struct AvailabilityPath {}

impl AvailabilityPath {
    pub fn new() -> Self {
        AvailabilityPath{}
    }
    pub fn availability_functions(&self) -> Vec<AvailabilityChooser> {
        vec![
            AvailabilityChooser::new(AvailabilityChoice ::AV1, AvailabilityFunctions::calc_availability1),
            AvailabilityChooser::new(AvailabilityChoice ::AV2, AvailabilityFunctions::calc_availability2),
            AvailabilityChooser::new(AvailabilityChoice ::AV3, AvailabilityFunctions::calc_availability3),
            AvailabilityChooser::new(AvailabilityChoice ::AV4, AvailabilityFunctions::calc_availability4),
        ]
    }
    pub fn shippingdata_functions(&self) -> Vec<ShippingDateChooser> {
        vec![
            ShippingDateChooser::new(ShippingDateChoice::SD1, ShippingDateFunctions::calc_shippingdate1),
            ShippingDateChooser::new(ShippingDateChoice::SD2, ShippingDateFunctions::calc_shippingdate2),
            ShippingDateChooser::new(ShippingDateChoice::SD3, ShippingDateFunctions::calc_shippingdate3),
            ShippingDateChooser::new(ShippingDateChoice::SD4, ShippingDateFunctions::calc_shippingdate4),
            ShippingDateChooser::new(ShippingDateChoice::SD5, ShippingDateFunctions::calc_shippingdate5),
        ]
    }
}

#[derive(Copy, Clone)]
struct Application {}

impl Application {
    pub fn new() -> Self{
        Application{}
    }

    fn invoice_path_function<'a>(config: &ProcessConfiguration, inv_path: &InvoicingPath) 
    -> impl Fn(&'a Order)->Freight {
            match inv_path.invoice_functions().iter()
                .where_by(|x| x.invoice_choice == config.invoice_choice)
                .select(|x| x.calc_invoice)
                .first() {
                    Some(x) => x,
                    None => panic!("There's no function to calculate the Invoice!"),
            }.compose(
            match inv_path.shipping_functions().iter()
                .where_by(|x| x.shipping_choice == config.shipping_choice)
                .select(|x| x.calc_shipping)
                .first(){
                    Some(x) => x,
                    None => panic!("There's no function to calculate the Shipping!"),
            }).compose(
            match inv_path.freight_functions().iter()
                .where_by(|x| x.freight_choice == config.freight_choice)
                .select(|x| x.calc_freight)
                .first(){
                    Some(x) => x,
                    None => panic!("There's no function to calculate the Freight!"),
            })
    }

    fn availability_path_function<'a>(config: &ProcessConfiguration,  av_path: &AvailabilityPath) 
    -> impl Fn(&'a Order)->ShippingDate {
            match av_path.availability_functions().iter()
                .where_by(|x| x.availability_choice == config.availability_choice)
                .select(|x| x.calc_availability)
                .first() {
                    Some(x) => x,
                    None => panic!("There's no function to calculate the Availability!"),
            }.compose(
            match av_path.shippingdata_functions().iter()
                .where_by(|x| x.shippingdate_choice == config.shippingdate_choice)
                .select(|x| x.calc_shippingdate)
                .first() {
                    Some(x) => x,
                    None => panic!("There's no function to calculate the ShippingDate!"),
            })
    }

    pub fn calc_adjusted_cost<'a>(&self, config: ProcessConfiguration, inv_path: InvoicingPath, av_path: AvailabilityPath)
     -> impl Fn(&'a Order)->f64 {

        let adjust_cost = | r: &'a Order, fr: &dyn Fn(&'a Order)->Freight, sh: &dyn Fn(&'a Order)-> ShippingDate | 
        -> f64 
        {
            let f = fr(r);
            let s = sh(r);

            let day = s.date.date.date().weekday();

            println!("\n\nDate of shipping: {0}", day);

            return if day == Weekday::Mon { f.cost + 1000. } else { f.cost + 500. };
        };

        move |r: &'a Order| -> f64 {
            adjust_cost(r, 
            &Application::invoice_path_function(&config, &inv_path),
            &Application::availability_path_function(&config, &av_path))
        }
    }
}


fn main() {
    let app = Application::new();

    let mut config              = ProcessConfiguration::new();
    config.invoice_choice       = InvoiceChoice::Inv3;
    config.shipping_choice      = ShippingChoice::Sh2;
    config.freight_choice       = FreightChoice::Fr3;
    config.availability_choice  = AvailabilityChoice::AV2;
    config.shippingdate_choice  = ShippingDateChoice::SD2;

    let customer        = Customer::new();
    let mut order       = Order::new();

    order.customer      = customer;
    order.date.date     = Utc.ymd(2021, 3, 16).and_hms(12, 0, 0);
    order.cost          = 2000.;

    let cost_of_order   = app.calc_adjusted_cost(config, InvoicingPath::new(), AvailabilityPath::new());
    
    println!("Cost of order: {0}", cost_of_order(&order));
}

#![feature(test)]

use mysqrt::MySqrt;

mod mysqrt;

fn main() {
    println!("enter numbers line by line. Enter empty line to finish");
    let mut vec: Vec<f64> = Vec::new();
    let mut buf;
    loop {
        buf = String::new();
        std::io::stdin().read_line(&mut buf).unwrap();
        if let Some(num) = buf.trim().parse::<f64>().ok() {
            vec.push(num);
        } else {
            break;
        }
    }

    println!("{:20}\t{:20}\t{:20}", "input", "my_sqrt_impl", "sqrt");
    let sqrts = vec.iter().map(|num| {
        let sqrts = (num, num.goldschmidt_sqrt_reg(), num.sqrt());
        println!("{:20}\t{:20}\t{:20}", sqrts.0, sqrts.1, sqrts.2);
        sqrts
    });

    let total_err = sqrts.filter(|(num, my_sqrt, sqrt)| !(sqrt.is_nan() || my_sqrt.is_nan() || num.is_infinite())).map(|(_num, my_sqrt, sqrt)| (sqrt - my_sqrt).abs()).sum::<f64>();

    println!("total err: \t{total_err}");
}
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

#[cfg(test)]
mod tests {
    extern crate test;

    use std::sync::{Arc, Mutex};
    use test::Bencher;

    #[bench]
    fn single_thread(b: &mut Bencher) {
        b.iter(|| {
            let n = std::hint::black_box(1_000_000);
            let mut q = Vec::with_capacity(n);
            (0..n).for_each(|e| q.push(e));
            assert_eq!(q.len(), 1_000_000);
            assert_eq!(q[12_345], 12_345);
        });
    }

    #[bench]
    fn two_thread_naive(b: &mut Bencher) {
        b.iter(|| {
            let n = std::hint::black_box(1_000_000);
            let mut arc_q = Arc::new(Mutex::new(Vec::with_capacity(n)));

            (0..2).map(|i| {
                let q = arc_q.clone();
                std::thread::spawn(move || {
                    ((i * n / 2)..((i + 1) * n / 2)).for_each(|e| q.lock().unwrap().push(e));
                })
            }).for_each(|h| h.join().unwrap());

            assert_eq!(arc_q.lock().unwrap().len(), 1_000_000);
        });
    }

    #[bench]
    fn two_thread_append(b: &mut Bencher) {
        b.iter(|| {
            let n = std::hint::black_box(1_000_000);
            let mut arc_q = Arc::new(Mutex::new(Vec::with_capacity(n)));

            (0..2).map(|i| {
                let mut vec = Vec::with_capacity(n / 2);
                let q = arc_q.clone();
                std::thread::spawn(move || {
                    ((i * n / 2)..((i + 1) * n / 2)).for_each(|e| vec.push(e));
                    q.lock().unwrap().append(&mut vec);
                })
            }).for_each(|h| h.join().unwrap());

            assert_eq!(arc_q.lock().unwrap().len(), 1_000_000);
        });
    }
}
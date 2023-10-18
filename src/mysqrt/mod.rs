use std::f64::consts::SQRT_2;

const EXP_MASK: u64 = 0x7ff0000000000000;
const MAN_MASK: u64 = 0x000fffffffffffff;
const EXP_SHIFT: u32 = f64::MANTISSA_DIGITS - 1;
const EXP_OFF: i32 = f64::MAX_EXP - 1;

pub trait MySqrt {
    fn inv_sqrt_approx(self) -> f64;
    fn halley_sqrt(self) -> f64;
    fn goldschmidt_sqrt(self) -> f64;
    fn halley_sqrt_seprt(self) -> f64;
    fn goldschmidt_sqrt_seprt(self) -> f64;
    fn halley_sqrt_reg(self) -> f64;
    fn goldschmidt_sqrt_reg(self) -> f64;
}

/// Returns a tuple of sqrt of exponent, separate mantissa, and error of sqrt exponent
///
fn separate_exp_man(num: f64) -> (f64, f64, f64) {
    if num - 0. <= f64::EPSILON { return (0., 0., 0.); }
    let num_bits = num.to_bits();
    let exp = ((num_bits & EXP_MASK) >> EXP_SHIFT) as i32 - EXP_OFF;
    let man = num_bits & MAN_MASK | 1f64.to_bits();
    let sqrt_exp = f64::from_bits((((exp >> 1) + EXP_OFF) as u64) << EXP_SHIFT);
    (sqrt_exp, f64::from_bits(man), if exp % 2 == 0 { 1f64 } else { SQRT_2 })
}

impl MySqrt for f64 {
    /// Returns an approximation of inverse sqrt
    ///
    /// # Safety
    ///
    /// Calling method on ['num < 0'] is *[undefined behavior]
    ///
    #[inline]
    fn inv_sqrt_approx(self) -> f64 {
        f64::from_bits(0x5FE6EB50C7B537A9 - (self.to_bits() >> 1))
    }

    /// Returns a sqrt with small error
    ///
    /// Returns NaN uf `self` is negative or Inf or Nan
    ///
    /// Uses Halley's method for iteration
    ///
    fn halley_sqrt(self) -> f64 {
        if self < 0. || self.is_nan()  { return f64::NAN; }
        if self.is_infinite() {return f64::INFINITY;}
        let mut inv_sqrt_approx = self.inv_sqrt_approx();
        let mut tmp;
        for _ in 0..5 {
            tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
            inv_sqrt_approx *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        }
        self * inv_sqrt_approx
    }

    /// Returns a sqrt with small error
    ///
    /// Returns NaN uf `self` is negative or Inf or Nan
    ///
    /// Uses Goldschmidt's method for iteration
    ///
    fn goldschmidt_sqrt(self) -> f64 {
        if self < 0. || self.is_nan()  { return f64::NAN; }
        if self.is_infinite() {return f64::INFINITY;}
        let mut sqrt = self * self.inv_sqrt_approx();
        let mut h = self.inv_sqrt_approx() * 0.5;
        let mut r = sqrt.mul_add(-h, 0.5);
        for _ in 0..5 {
            sqrt = r.mul_add(sqrt, sqrt);
            h = r.mul_add(h, h);
            r = sqrt.mul_add(-h, 0.5);
        }
        r.mul_add(sqrt, sqrt)
    }

    /// Returns an approximation of sqrt by separating mantissa and exponent
    /// with Halley's method
    ///
    #[inline]
    fn halley_sqrt_seprt(self) -> f64 {
        if self < 0. || self.is_nan()  { return f64::NAN; }
        if self.is_infinite() {return f64::INFINITY;}
        let (sqrt_exp, manf, correction) = separate_exp_man(self);
        manf.halley_sqrt() * correction * sqrt_exp
    }

    /// Returns an approximation of sqrt by separating mantissa and exponent
    /// with Goldschmidt's method
    ///
    #[inline]
    fn goldschmidt_sqrt_seprt(self) -> f64 {
        if self < 0. || self.is_nan()  { return f64::NAN; }
        if self.is_infinite() {return f64::INFINITY;}
        let (sqrt_exp, manf, correction) = separate_exp_man(self);
        manf.goldschmidt_sqrt() * correction * sqrt_exp
    }

    /// Returns a sqrt with small error
    ///
    /// Returns NaN uf `self` is negative or Inf or Nan
    ///
    /// Uses Halley's method for iteration without mul_add operation
    ///
    #[inline]
    fn halley_sqrt_reg(self) -> f64 {
        if self < 0. || self.is_nan()  { return f64::NAN; }
        if self.is_infinite() {return f64::INFINITY;}
        let mut inv_sqrt_approx = self.inv_sqrt_approx();
        let mut tmp;
        for _ in 0..5 {
            tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
            inv_sqrt_approx *= (tmp * 3f64 / 8f64 + 10f64 / 8f64) * tmp + 15f64 / 8f64;
        }
        self * inv_sqrt_approx
    }

    /// Returns a sqrt with small error
    ///
    /// Returns NaN uf `self` is negative or Inf or Nan
    ///
    /// Uses Goldschmidt's method for iteration without mul_add operation
    ///
    #[inline]
    fn goldschmidt_sqrt_reg(self) -> f64 {
        if self < 0. || self.is_nan()  { return f64::NAN; }
        if self.is_infinite() {return f64::INFINITY;}
        let mut sqrt = self * self.inv_sqrt_approx();
        let mut h = self.inv_sqrt_approx() * 0.5;
        let mut r = 0.5 - sqrt * h;
        for _ in 0..5 {
            sqrt = r * sqrt + sqrt;
            h = r * h + h;
            r = 0.5 - sqrt * h;
        }
        r * sqrt + sqrt
    }
}

#[cfg(test)]
mod tests {
    extern crate test;

    use test::Bencher;

    use plotters::prelude::*;

    use super::*;

    #[test]
    fn draw_sqrt() -> Result<(), Box<dyn std::error::Error>> {
        let map = (0..=2_000_000).map(|x| (x as f64) / 1024f64).map(|x| x * x);
        let root = BitMapBackend::new("0.png", (640, 480)).into_drawing_area();
        root.fill(&WHITE)?;
        let mut chart = ChartBuilder::on(&root).caption("mysqrt()", ("sans-serif", 50).into_font()).margin(5).x_label_area_size(30).y_label_area_size(30).build_cartesian_2d(1f64..1024f64, 0f64..64f64)?;


        chart.configure_mesh().draw()?;
        chart.draw_series(LineSeries::new(
            map.clone().map(|x| (x, x.sqrt())),
            &BLUE,
        ))?.label("y = x-^.5").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &BLUE));
        chart.draw_series(LineSeries::new(
            map.clone().map(|x| (x, x.goldschmidt_sqrt())),
            &RED,
        ))?.label("y ~= x^-.5").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &RED));

        chart.configure_series_labels().background_style(&WHITE.mix(0.8)).border_style(&BLACK).draw()?;
        root.present()?;

        assert!(true);
        Ok(())
    }

    #[test]
    fn test_correctnes() {
        let map = (0..=2_000_000).map(|x| (x as f64) / 1024f64).map(|x| x * x);

        println!("\nhalley_sqrt");
        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.halley_sqrt().to_bits() as i128)).filter(|&x| x != 0);
        println!("{} different out of {}", fmap.clone().count(), map.clone().count());
        println!("approx bit wrongs: {}", fmap.clone().sum::<i128>());
        println!("max difference: {}", fmap.max().unwrap());

        println!("\ngoldschmidt_sqrt");
        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.goldschmidt_sqrt().to_bits() as i128)).filter(|&x| x != 0);
        println!("{} different out of {}", fmap.clone().count(), map.clone().count());
        println!("approx bit wrongs: {}", fmap.clone().sum::<i128>());
        println!("max difference: {}", fmap.max().unwrap());

        println!("\nhalley_sqrt_seprt");
        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.halley_sqrt_seprt().to_bits() as i128)).filter(|&x| x != 0);
        println!("{} different out of {}", fmap.clone().count(), map.clone().count());
        println!("approx bit wrongs: {}", fmap.clone().sum::<i128>());
        println!("max difference: {}", fmap.max().unwrap());

        println!("\ngoldschmidt_sqrt_seprt");
        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.goldschmidt_sqrt_seprt().to_bits() as i128)).filter(|&x| x != 0);
        println!("{} different out of {}", fmap.clone().count(), map.clone().count());
        println!("approx bit wrongs: {}", fmap.clone().sum::<i128>());
        println!("max difference: {}", fmap.max().unwrap());

        println!("\nhalley_sqrt_reg");
        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.halley_sqrt_reg().to_bits() as i128)).filter(|&x| x != 0);
        println!("{} different out of {}", fmap.clone().count(), map.clone().count());
        println!("approx bit wrongs: {}", fmap.clone().sum::<i128>());
        println!("max difference: {}", fmap.max().unwrap());

        println!("\ngoldschmidt_sqrt_reg");
        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.goldschmidt_sqrt_reg().to_bits() as i128)).filter(|&x| x != 0);
        println!("{} different out of {}", fmap.clone().count(), map.clone().count());
        println!("approx bit wrongs: {}", fmap.clone().sum::<i128>());
        println!("max difference: {}", fmap.max().unwrap());

        assert!(true);
    }


    // fn get_list(f: fn(f64) -> f64) -> f64 {
    //     (0..=100_000u64).map(|x| (x as f64) / 1_000f64).map(|x| f(x)).sum()
    // }
    //
    // #[bench]
    // fn bench_sqrt(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::sqrt));
    // }
    //
    // #[bench]
    // fn bench_halley_sqrt(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::halley_sqrt));
    // }
    //
    // #[bench]
    // fn bench_goldschmidt_sqrt(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::goldschmidt_sqrt));
    // }
    //
    // #[bench]
    // fn bench_halley_sqrt_seprt(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::halley_sqrt_seprt));
    // }
    //
    // #[bench]
    // fn bench_goldschmidt_sqrt_seprt(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::goldschmidt_sqrt_seprt));
    // }
    //
    // #[bench]
    // fn bench_halley_sqrt_reg(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::halley_sqrt_reg));
    // }
    //
    // #[bench]
    // fn bench_goldschmidt_sqrt_reg(b: &mut Bencher) {
    //     b.iter(|| get_list(f64::goldschmidt_sqrt_reg));
    // }
}

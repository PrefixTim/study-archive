use std::f64::consts::SQRT_2;

const EXP_MASK: u64 = 0x7ff0000000000000;
const MAN_MASK: u64 = 0x000fffffffffffff;
const EXP_SHIFT: u32 = f64::MANTISSA_DIGITS - 1;
const EXP_OFF: i32 = f64::MAX_EXP - 1;

pub trait MySqrt {
    fn my_sqrt_a(self) -> f64;
    fn my_sqrt_b(self) -> f64;
    fn my_sqrt_c(self) -> f64;
    fn my_sqrt_d(self) -> f64;
}

pub fn inv_sqrt_approx(num: f64) -> f64 {
    f64::from_bits(0x5FE6EB50C7B537A9 - (num.to_bits() >> 1))
}

impl MySqrt for f64 {
    #[inline]
    fn my_sqrt_a(self) -> f64 {
        if self.is_sign_negative() || self.is_nan() || self.is_infinite() { return f64::NAN; }
        let num_bits = self.to_bits();
        let exp = ((num_bits & EXP_MASK) >> EXP_SHIFT) as i32 - EXP_OFF;
        let man = num_bits & MAN_MASK | 1f64.to_bits();
        let sqrt_exp = f64::from_bits((((exp >> 1) + EXP_OFF) as u64) << EXP_SHIFT);
        let inv_sqrt_man = f64::from_bits(0x5FE6EB50C7B537A9 - (man >> 1));
        let manf = f64::from_bits(man);

        let mut sqrt = manf * inv_sqrt_man;
        let mut h = inv_sqrt_man * 0.5;
        let mut r;
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);
        h = r.mul_add(h, h);
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);
        h = r.mul_add(h, h);
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);
        h = r.mul_add(h, h);
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);

        sqrt * if exp % 2 == 0 { 1f64 } else { SQRT_2 } * sqrt_exp
    }

    #[inline]
    fn my_sqrt_b(self) -> f64 {
        if self.is_sign_negative() || self.is_nan() || self.is_infinite() { return f64::NAN; }
        let num_bits = self.to_bits();
        let exp = ((num_bits & EXP_MASK) >> EXP_SHIFT) as i32 - EXP_OFF;
        let man = num_bits & MAN_MASK | 1f64.to_bits();
        let sqrt_exp = f64::from_bits((((exp >> 1) + EXP_OFF) as u64) << EXP_SHIFT);
        let mut inv_sqrt_man = f64::from_bits(0x5FE6EB50C7B537A9 - (man >> 1));
        let manf = f64::from_bits(man);

        let tmp = -manf * inv_sqrt_man * inv_sqrt_man;
        inv_sqrt_man *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        let tmp = -manf * inv_sqrt_man * inv_sqrt_man;
        inv_sqrt_man *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        let tmp = -manf * inv_sqrt_man * inv_sqrt_man;
        inv_sqrt_man *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        let tmp = -manf * inv_sqrt_man * inv_sqrt_man;
        inv_sqrt_man *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);

        manf * inv_sqrt_man * if exp % 2 == 0 { 1f64 } else { SQRT_2 } * sqrt_exp
    }

    #[inline]
    fn my_sqrt_c(self) -> f64 {
        let mut inv_sqrt_approx = inv_sqrt_approx(self);
        let mut tmp;
        tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
        inv_sqrt_approx *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
        inv_sqrt_approx *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
        inv_sqrt_approx *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
        inv_sqrt_approx *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        tmp = -self * inv_sqrt_approx * inv_sqrt_approx;
        inv_sqrt_approx *= tmp.mul_add(3f64 / 8f64, 10f64 / 8f64).mul_add(tmp, 15f64 / 8f64);
        self * inv_sqrt_approx
    }

    #[inline]
    fn my_sqrt_d(self) -> f64 {
        let inv_sqrt_approx = inv_sqrt_approx(self);
        let mut sqrt = self * inv_sqrt_approx;
        let mut h = inv_sqrt_approx * 0.5;
        let mut r;
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);
        h = r.mul_add(h, h);
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);
        h = r.mul_add(h, h);
        r = sqrt.mul_add(-h, 0.5);
        sqrt = r.mul_add(sqrt, sqrt);
        h = r.mul_add(h, h);
        r = sqrt.mul_add(-h, 0.5);
        r.mul_add(sqrt, sqrt)
    }
}

#[cfg(test)]
mod tests {
    extern crate test;

    use test::Bencher;

    use plotters::prelude::*;

    use super::*;

    #[test]
    fn test() -> Result<(), Box<dyn std::error::Error>> {
        let map = (1..=2_000_000).map(|x| (x as f64) / 1024f64).map(|x| x * x);
        let root = BitMapBackend::new("0.png", (640, 480)).into_drawing_area();
        root.fill(&WHITE)?;
        let mut chart = ChartBuilder::on(&root).caption("mysqrt()", ("sans-serif", 50).into_font()).margin(5).x_label_area_size(30).y_label_area_size(30).build_cartesian_2d(1f64..1024f64, 0f64..64f64)?;


        chart.configure_mesh().draw()?;
        chart.draw_series(LineSeries::new(
            map.clone().map(|x| (x, x.sqrt())),
            &BLUE,
        ))?.label("y = x-^.5").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &BLUE));
        chart.draw_series(LineSeries::new(
            map.clone().map(|x| (x, x.my_sqrt_c())),
            &RED,
        ))?.label("y ~= x^-.5").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &RED));

        chart.configure_series_labels().background_style(&WHITE.mix(0.8)).border_style(&BLACK).draw()?;

        root.present()?;

        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.my_sqrt_a().to_bits() as i128)).filter(|&x| x != 0);
        println!("{}/{}", fmap.clone().count(), map.clone().count());
        println!("{}", fmap.clone().sum::<i128>());
        println!("{}", fmap.clone().max().unwrap());

        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.my_sqrt_b().to_bits() as i128)).filter(|&x| x != 0);
        println!("{}/{}", fmap.clone().count(), map.clone().count());
        println!("{}", fmap.clone().sum::<i128>());
        println!("{}", fmap.clone().max().unwrap());

        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.my_sqrt_c().to_bits() as i128)).filter(|&x| x != 0);
        println!("{}/{}", fmap.clone().count(), map.clone().count());
        println!("{}", fmap.clone().sum::<i128>());
        println!("{}", fmap.clone().max().unwrap());

        let fmap = map.clone().map(|x| i128::abs(x.sqrt().to_bits() as i128 - x.my_sqrt_d().to_bits() as i128)).filter(|&x| x != 0);
        println!("{}/{}", fmap.clone().count(), map.clone().count());
        println!("{}", fmap.clone().sum::<i128>());
        println!("{}", fmap.clone().max().unwrap());
        assert!(true);
        Ok(())
    }

    fn get_list(f: fn(f64) -> f64) -> f64 {
        (1..=100_000u64).map(|x| (x as f64) / 1_000f64).map(|x| f(x)).sum()
    }

    #[bench]
    fn bench_sqrt(b: &mut Bencher) {
        b.iter(|| get_list(f64::sqrt));
    }

    #[bench]
    fn bench_sqrta(b: &mut Bencher) {
        b.iter(|| get_list(f64::my_sqrt_a));
    }

    #[bench]
    fn bench_sqrtb(b: &mut Bencher) {
        b.iter(|| get_list(f64::my_sqrt_b));
    }

    #[bench]
    fn bench_sqrtc(b: &mut Bencher) {
        b.iter(|| get_list(f64::my_sqrt_c));
    }

    #[bench]
    fn bench_sqrtd(b: &mut Bencher) {
        b.iter(|| get_list(f64::my_sqrt_d));
    }
}

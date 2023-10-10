use plotters::prelude::*;

use mysqrt::*;

mod mysqrt {
    use std::f64::consts::SQRT_2;

    const EXP_MASK: u64 = 0x7ff0000000000000;
    const MAN_MASK: u64 = 0x000fffffffffffff;
    const EXP_SHIFT: u32 = f64::MANTISSA_DIGITS - 1;
    const EXP_OFF: i32 = f64::MAX_EXP - 1;

    pub trait MySqrt {
        fn my_sqrt_a(self) -> f64;
        fn my_sqrt_b(self) -> f64;
        fn my_sqrt_c(self) -> f64;
    }

    pub fn inv_sqrt_approx_a(num: f64) -> f64 {
        f64::from_bits(0x5FE6EB50C7B537A9 - (num.to_bits() >> 1))
    }
    // float invSqrt(float x) {
    //     float xhalf = 0.5f * x;
    //     union {
    //         float x;
    //         int i;
    //     } u;
    //     u.x = x;
    //     u.i = 0x5f375a86 - (u.i >> 1);
    //     /* The next line can be repeated any number of times to increase accuracy */
    //     u.x = u.x * (1.5f - xhalf * u.x * u.x);
    //     return u.x;
    // }
    pub fn inv_sqrt_approx_b(num: f64) -> f64 {
        let half = 0.5 * num;
        let mut aprox = inv_sqrt_approx_a(num);
        aprox *= 1.5 - half * aprox *  aprox;
        aprox *= 1.5 - half * aprox *  aprox;
        aprox *= 1.5 - half * aprox *  aprox;
        aprox *= 1.5 - half * aprox *  aprox;
        aprox *= 1.5 - half * aprox *  aprox;
        aprox *= 1.5 - half * aprox *  aprox;
        aprox
    }

    impl MySqrt for f64 {
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

        fn my_sqrt_c(self) -> f64 {
            self * inv_sqrt_approx_b(self)
        }
    }
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let map = (0..=20000).map(|x| x as f64 / 10000f64);
    // let root = BitMapBackend::new("0.png", (640, 480)).into_drawing_area();
    // root.fill(&WHITE)?;
    // let mut chart = ChartBuilder::on(&root).caption("sqrt()", ("sans-serif", 50).into_font()).margin(5).x_label_area_size(30).y_label_area_size(30).build_cartesian_2d(1f64..20f64, 0f64..1.5f64)?;
    //
    //
    // chart.configure_mesh().draw()?;
    // chart.draw_series(LineSeries::new(
    //     map.clone().map(|x| (x, x.sqrt())),
    //     &BLUE,
    // ))?.label("y = x-^.5").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &BLUE));
    // chart.draw_series(LineSeries::new(
    //     map.clone().map(|x| (x, x*inv_sqrt_approx_b(x))),
    //     &RED,
    // ))?.label("y ~= x^-.5").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &RED));
    //
    // chart.configure_series_labels().background_style(&WHITE.mix(0.8)).border_style(&BLACK).draw()?;
    //
    // root.present()?;
    let map = map.clone().filter(|x| f64::abs(x.sqrt() - x.my_sqrt_c()) > f64::EPSILON);
    // map.clone().for_each(|x| {
    //     println!("{:#b}", x.sqrt().to_bits());
    //     println!("{:#b}", x.my_sqrt_c().to_bits());
    //     println!("{:#b}", (x.sqrt() - x.my_sqrt_c()).to_bits());
    //     println!();
    // });
    // println!("{}", map.clone().map(|x| f64::abs(x.sqrt() - x.my_sqrt_c())).sum::<f64>());
    // map.for_each(|x| if f64::abs(x.sqrt() - x.my_sqrt_c()) > f64::EPSILON {println!("{}", x)});
    println!("{}", map.count());

    // let x = 199.997f64;
    // println!("{}", x.sqrt());
    // println!("{}", x.my_sqrt());
    // println!("{:#b}", x.sqrt().to_bits());
    // println!("{:#b}", x.my_sqrt().to_bits());
    Ok(())
}

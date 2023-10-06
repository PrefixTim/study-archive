use std::f64::consts::LN_2;
fn sqrt() {
    let num = 1.21f64;
    let num_bits = num.to_bits();
    const SGN_MASK: u64 = 0x8000000000000000;
    const EXP_MASK: u64 = 0x7ff0000000000000;
    const MAN_MASK: u64 = 0x000fffffffffffff;
    let sgn = num_bits & SGN_MASK;
    let exp = (num_bits & EXP_MASK >> f64::MANTISSA_DIGITS) as i32 - f64::MAX_EXP;
    let man = num_bits & MAN_MASK;
    let exp0 = exp & 0x1;
    // println!("{:#66b}", num_bits);
    // println!("{:#66b}", sgn);
    // println!("{:#66b}", num_bits & EXP_MASK);
    // println!("{:#66b} ExpMAsk", EXP_MASK);
    // println!("{:#66b}", man);
    // println!("{:#66b} ManMask", MAN_MASK);

    // println!("{:#b}", num_bits & EXP_MASK >> f64::MANTISSA_DIGITS);
    // println!("{}", exp);
    // print!("{exp0}");
    // let exp =  (num_bits & EXP_MASK >> (f64::MANTISSA_DIGITS - 1)) as i64 + 1 - f64::MAX_EXP as i64;
    // 0b11 1111 1111
    // let man = num_bits & MAN_MASK;
    // println!("E:{}, M:{}", exp, man);
    // std::fmt::
    // let exp =( s.to_bits() >>  f64::MANTISSA_DIGITS) as i32 - bias;
    // print!("{}= 2^{}",2 << exp, exp)
    // f64::EPSILON
    // println!("{:#b}",  s.to_bits())
    let m = f64::from_bits(man | 0x3ff0000000000000);
    println!("{}", m);

    let tmp_ln = (-2.2833333333333332149095440399833023548126220703125 + m * (2.5 + m * (-1.25 + m * (0.416666666666666685170383743752609007060527801513671875 + (-0.078125 + m * 0.00625000000000000034694469519536141888238489627838134765625) * m))) + LN_2) * 0.5;
    // let tmp_ln = 0.5 * (-1.7417939 + (2.8212026 + (-1.4699568 + (0.44717955 - 0.056570851 * m) * m) * m) * m);
    println!("{:#b}", tmp_ln.to_bits());
    println!("{:#b}", (0.5 * m.ln()).to_bits());

    let tmp_exp = 1. + tmp_ln * (1. + tmp_ln * (0.5 + tmp_ln * (0.16666666666666666 + (0.041666666666666664 + tmp_ln * 0.008333333333333333) * tmp_ln)));
    println!("{:#b}", tmp_exp.to_bits());
    println!("{:#b}", m.sqrt().to_bits());
    println!("{}", tmp_exp);
    println!("{}", m.sqrt());

}

fn main() {
    sqrt()
}

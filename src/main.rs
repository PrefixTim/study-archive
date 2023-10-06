fn sqrt() {
    let num = 2f64;
    let num_bits = num.to_bits();
    const SGN_MASK: u64 = 0x8000000000000000;
    const EXP_MASK: u64 = 0x7ff0000000000000;
    const MAN_MASK: u64 = 0x000fffffffffffff;
    let sgn = num_bits & SGN_MASK;
    let exp = (num_bits & EXP_MASK >> f64::MANTISSA_DIGITS) as i32 - f64::MAX_EXP;
    let man = num_bits & MAN_MASK;
    let exp0 = exp & 0x1;
    println!("{:#66b}", num_bits);
    // println!("{:#66b}", sgn);
    println!("{:#b}", num_bits & EXP_MASK);
    println!("{:#b}", num_bits & EXP_MASK >> f64::MANTISSA_DIGITS);
    println!("{}", exp);
    // println!("{:#66b}", man);

    print!("{exp0}");
    // let exp =  (num_bits & EXP_MASK >> (f64::MANTISSA_DIGITS - 1)) as i64 + 1 - f64::MAX_EXP as i64;
    // let man = num_bits & MAN_MASK;
    // println!("E:{}, M:{}", exp, man);
    // std::fmt
    // let exp =( s.to_bits() >>  f64::MANTISSA_DIGITS) as i32 - bias;
    // print!("{}= 2^{}",2 << exp, exp)
    // f64::EPSILON
    // println!("{:#b}",  s.to_bits())
    // let m = 0;
    // let tmp_ln = -1.7417939+(2.8212026+(-1.4699568+(0.44717955-0.056570851*m)*m)*m)*m;
    // let tmp_exp = 1 + tmp_ln (1 + tmp_ln (0.5 + tmp_ln (0.16666666666666666 + (0.041666666666666664 + tmp_ln*0.008333333333333333) *tmp_ln)));
}

fn main() {
    println!("Hello, world!");
}

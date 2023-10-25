use itertools::Itertools;
use std::cmp::Ordering;
use std::collections::HashMap;
use rand::Rng;
use std::{fs::File, io::Write};

use super::N;

lazy_static! {
    // static ref VEC_ODD: Vec<i32> = vec_with_n(1, 2);
    // static ref VEC_EVN: Vec<i32> = vec_with_n(0, 2);
    static ref VEC_ALL: Vec<i32> = vec_with_n(0, 1);
}

fn vec_with_n(offset: usize, step: usize) -> Vec<i32> {
    let mut i = offset;
    let mut res = Vec::new();
    while i < N {
        res.push(i as i32);
        i += step;
    }
    res
}

pub fn verify(data: &[i32; N], head: usize) -> bool {
    let mut data = data.to_vec();
    data.sort();
    let counts: HashMap<&i32, usize> = data.iter().counts();
    let mut outliers = counts.iter().filter(|(_, &x)| x != 1).collect_vec();
    outliers.sort_by(|x, y| x.0.cmp(y.0));
    if !outliers.is_empty() {
        // most of the errors

        let mut missing = Vec::new();
        let mut i = 0;
        VEC_ALL.iter().for_each(|num| loop {
            match data[i].cmp(num) {
                Ordering::Less => {
                    i += 1;
                }
                Ordering::Equal => {
                    i += 1;
                    break;
                }
                Ordering::Greater => {
                    missing.push(num);
                    break;
                }
            }
        });
        let mut file = File::create(format!(
            "FaileLog{}.txt",
            rand::thread_rng().gen_range(0..100)
        ))
        .unwrap();
        file.write_fmt(format_args!(
            "head: {}\nOutliers\n{:?}\nMisssing\n{:?}\n---\n",
            head, outliers, missing
        ))
        .unwrap();
    }
    outliers.is_empty()
}

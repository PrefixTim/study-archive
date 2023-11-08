use itertools::Itertools;
use rand::Rng;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::fs::create_dir_all;
use std::{fs::File, io::Write};

use super::N;

/// A verification function chechks if an array have all numbers from 0 to its size
/// Logs missing or duplicated values
/// 
pub fn verify(data: &[i32; N], head: usize, total: usize) -> bool {
    let mut data = data[0..total].to_vec();
    data.sort();

    let counts: HashMap<&i32, usize> = data.iter().counts();
    let mut outliers = counts.iter().filter(|(_, &x)| x != 1).collect_vec();
    outliers.sort_by(|x, y| x.0.cmp(y.0));
    
    if !outliers.is_empty() { //catches majority of the errors
        let mut i = 0;
        let missing = (0..total as i32).into_iter().filter_map(|num| loop {
            match data[i].cmp(&num) {
                Ordering::Less => {
                    i += 1;
                }
                Ordering::Equal => {
                    i += 1;
                    break None;
                }
                Ordering::Greater => {
                    break Some(num);
                }
            }
        }).collect_vec();
        create_dir_all("./Log").unwrap();
        let mut file = File::create(format!(
            "./Log/FaileLog{}.txt",
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

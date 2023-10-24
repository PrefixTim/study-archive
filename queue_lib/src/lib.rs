#[macro_use]
extern crate lazy_static;

use itertools::Itertools;
use std::{fs::File, io::Write};
use std::collections::HashMap;

pub const N: usize = 1_000_000;
pub const N2: usize = N;
lazy_static!{
    static ref VEC_ODD: Vec<i32> = vec_with_n(1, 2);
    static ref VEC_EVN: Vec<i32> = vec_with_n(0, 2);
}

fn vec_with_n(offset: usize, step: usize) -> Vec<i32>{
    let mut i = offset;
    let mut res = Vec::new();
    while i < N {
        res.push(i as i32);
        i += step;
    }
    res
}

#[repr(C)]
#[derive(Debug)]
struct Stack {
    head: usize,
    data: [i32; N2],
}

impl Stack {
    #[no_mangle]
    pub extern "C" fn new_stack() -> Self {
        Self {
            head: 0,
            data: [i32::MAX; N2],
        }
    }

    #[no_mangle]
    pub extern "C" fn peak(&self) -> i32 {
        self.data[self.head - 1]
    }

    #[no_mangle]
    pub extern "C" fn push(&mut self, el: i32) {
        self.data[self.head] = el;
        self.head += 1;
    }

    #[no_mangle]
    pub extern "C" fn pop(&mut self) -> i32 {
        self.head -= 1;
        self.data[self.head]
    }

    #[no_mangle]
    pub extern "C" fn verify(&self) -> bool {
        let counts: HashMap<&i32, usize> = self.data.iter().counts();
        let mut outliers = counts
            .iter()
            .filter(|(_, &x)| x != 1)
            .collect_vec(); 
        outliers.sort_by(|x, y| x.0.cmp(y.0));
        if !outliers.is_empty() { // most of the errors

            // let missing = ALL_COUNTS.iter().filter(|(num, &count)| counts.contains_key(num) && counts[num] == count).collect_vec();
            let mut file = File::create("FaileLog.txt").unwrap();
            file.write_fmt(format_args!("head{}\n{:?}\n---\n", self.head, outliers))
                .unwrap();
        }
        outliers.is_empty()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let mut stack = Stack::new_stack();
        stack.push(1);
        assert_eq!(stack.peak(), 1);
        stack.push(4);
        assert_eq!(stack.peak(), 4);
        assert_eq!(stack.pop(), 4);
        assert_eq!(stack.pop(), 4);
    }
}

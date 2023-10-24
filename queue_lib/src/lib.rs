use itertools::Itertools;
use std::{fs::File, io::Write, iter, cmp::Reverse};

pub const N: usize = 1_000;
pub const N2: usize = N;

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
        let mut counts = vec![0; N];
        self.data.iter().for_each(|&e| counts[e as usize] += 1);
        let outliers = counts.iter().enumerate().filter(|(_, &x)| x != 1).collect_vec();//.map(|(n, counts)|)
    
        if !outliers.is_empty() {
            let mut file = File::create("FaileLog.txt").unwrap();
            file.write_fmt(format_args!("head{}\n{:?}\n---\n", self.head, outliers)).unwrap();
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

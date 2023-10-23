use itertools::Itertools;
use std::{fs::File, io::Write};

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
        let mut res = true;
        let mut cloned = self.data.clone();
        cloned.sort();
        cloned.iter().scan(0, |state, el| {
            res &= *el == *state;
            Some(*state + 1)
        }).collect_vec();
        if !res {
            let mut file = File::create("FaileLog.txt").unwrap();
            print!("{}", self.data.iter().all_unique());
            // file.write_fmt(fmt)

            file.write_fmt(format_args!(
                "head{}\n{:?}\n---\n",
                self.head,
                cloned
                    .iter()
                    .counts()
                    .iter()
                    .filter(|(_, p)| **p != 1)
                    .collect_vec()
            ))
            .unwrap();
        }
        res
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

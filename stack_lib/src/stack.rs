use super::N;

#[repr(C)]
#[derive(Debug)]
pub struct Stack {
    head: usize,
    data: [i32; N],
}

impl Stack {
    #[no_mangle]
    pub extern "C" fn new_stack() -> Self {
        Self {
            head: 0,
            data: [i32::MAX; N],
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
}

#[no_mangle]
pub extern "C" fn verify_stack(stack: &Stack, total: usize) -> bool {
    super::verify::verify(&stack.data, stack.head, total)
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
        assert_eq!(stack.pop(), 1);
    }
}

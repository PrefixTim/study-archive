use super::N;
use std::sync::atomic::{AtomicUsize, Ordering};

#[derive(Debug)]
pub struct AtomicStack {
    head: AtomicUsize,
    data: [i32; N],
}

impl AtomicStack {
    fn new() -> Self {
        Self {
            head: AtomicUsize::new(0),
            data: [i32::MAX; N],
        }
    }

    fn peak(&self) -> i32 {
        self.data[self.head.load(Ordering::Acquire) - 1]
    }

    fn push(&mut self, el: i32) {
        self.data[self.head.fetch_add(1, Ordering::SeqCst)] = el;
    }

    fn pop(&mut self) -> i32 {
        self.data[self.head.fetch_sub(1, Ordering::SeqCst) - 1]
    }
}

mod c_compat {
    use super::*;

    #[no_mangle]
    pub extern "C" fn atomic_stack_new() -> *mut AtomicStack {
        Box::into_raw(Box::new(AtomicStack::new()))
    }

    #[no_mangle]
    pub extern "C" fn atomic_stack_drop(ptr: *mut AtomicStack) {
        if ptr.is_null() {
            return;
        }
        unsafe {
            let _ = Box::from_raw(ptr);
        }
    }

    #[no_mangle]
    pub extern "C" fn atomic_stack_peak(ptr: *mut AtomicStack) -> i32 {
        let stack = unsafe {
            assert!(!ptr.is_null());
            &mut *ptr
        };
        stack.peak()
    }

    #[no_mangle]
    pub extern "C" fn atomic_stack_push(ptr: *mut AtomicStack, elem: i32) {
        let stack = unsafe {
            assert!(!ptr.is_null());
            &mut *ptr
        };
        stack.push(elem)
    }

    #[no_mangle]
    pub extern "C" fn atomic_stack_pop(ptr: *mut AtomicStack) -> i32 {
        let stack = unsafe {
            assert!(!ptr.is_null());
            &mut *ptr
        };
        stack.pop()
    }

    #[no_mangle]
    pub extern "C" fn verify_atomic_stack(stack: &AtomicStack) -> bool {
        crate::verify::verify(&stack.data, stack.head.load(Ordering::Acquire))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let mut stack = AtomicStack::new();
        stack.push(1);
        assert_eq!(stack.peak(), 1);
        stack.push(4);
        assert_eq!(stack.peak(), 4);
        assert_eq!(stack.pop(), 4);
        assert_eq!(stack.pop(), 1);
    }
}

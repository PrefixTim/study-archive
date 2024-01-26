use std::sync::Mutex;

use super::N;

#[derive(Debug)]
pub struct MutexStack {
    head: Mutex<usize>,
    data: [i32; N],
}

// A stack with head wraped in a mutex
// Exactly like naive stack but the need to unlock head for each operation
impl MutexStack {
    fn new() -> Self {
        Self {
            head: Mutex::new(0),
            data: [i32::MAX; N],
        }
    }

    fn peak(&self) -> i32 {
        self.data[*self.head.lock().unwrap() - 1]
    }

    fn push(&mut self, el: i32) {
        let mut head = self.head.lock().unwrap();
        self.data[*head] = el;
        *head += 1;
    }

    fn pop(&mut self) -> i32 {
        let mut head = self.head.lock().unwrap();
        *head -= 1;
        self.data[*head]
    }
}

// A compatability stuff for C
mod c_compat {
    use super::*;

    #[no_mangle]
    pub extern "C" fn mutex_stack_new() -> *mut MutexStack {
        Box::into_raw(Box::new(MutexStack::new()))
    }

    #[no_mangle]
    pub extern "C" fn mutex_stack_drop(ptr: *mut MutexStack) {
        if ptr.is_null() {
            return;
        }
        unsafe {
            let _ = Box::from_raw(ptr);
        }
    }

    #[no_mangle]
    pub extern "C" fn mutex_stack_peak(ptr: *mut MutexStack) -> i32 {
        let stack = unsafe {
            assert!(!ptr.is_null());
            &mut *ptr
        };
        stack.peak()
    }

    #[no_mangle]
    pub extern "C" fn mutex_stack_push(ptr: *mut MutexStack, elem: i32) {
        let stack = unsafe {
            assert!(!ptr.is_null());
            &mut *ptr
        };
        stack.push(elem)
    }

    #[no_mangle]
    pub extern "C" fn mutex_stack_pop(ptr: *mut MutexStack) -> i32 {
        let stack = unsafe {
            assert!(!ptr.is_null());
            &mut *ptr
        };
        stack.pop()
    }

    #[no_mangle]
    pub extern "C" fn verify_mutex_stack(stack: &MutexStack, total: usize) -> bool {
        crate::verify::verify(&stack.data, *stack.head.lock().unwrap(), total)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let mut stack = MutexStack::new();
        stack.push(1);
        assert_eq!(stack.peak(), 1);
        stack.push(4);
        assert_eq!(stack.peak(), 4);
        assert_eq!(stack.pop(), 4);
        assert_eq!(stack.pop(), 1);
    }
}

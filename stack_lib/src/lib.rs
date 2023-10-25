#[macro_use]
extern crate lazy_static;

mod stack;
mod atomic_stack;
mod mutex_stack;
mod verify;

pub const N: usize = 100_000;

// pub use stack::*;
// pub use atomic_stack::*;
// pub use mutex_stack::*;
// pub use verify::*;

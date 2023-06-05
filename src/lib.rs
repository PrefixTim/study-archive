mod evaluate;
mod search;
mod classifier;

use ndarray::{ArrayBase, OwnedRepr, Dim};

pub use search::{backward_elim, forward_sel};
pub use evaluate::Evaluator;
pub use classifier::{NNClassifier, Classifier};


pub type Arr1f32 = ArrayBase<OwnedRepr<f64>, Dim<[usize; 1]>>;
pub type Arr2f32 = ArrayBase<OwnedRepr<f64>, Dim<[usize; 2]>>;
pub type Arr3f32 = ArrayBase<OwnedRepr<f64>, Dim<[usize; 3]>>;
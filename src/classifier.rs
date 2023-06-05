use ndarray::{Array, ArrayBase, Dim};

use super::{Arr1f32, Arr2f32, Arr3f32};

pub trait Classifier: Clone {
    fn test(&self, instid: usize, fset: &Arr1f32) -> f64;
    fn train(&mut self, train_data: Arr1f32);
}

#[derive(Clone)]
pub struct NNClassifier {
    distances: Arr3f32,
    label: Arr1f32,
    train_data: Arr1f32,
}

impl NNClassifier {
    pub fn new((label, features): &(Arr1f32, Arr2f32)) -> Self {
        let train_data: Arr1f32 = Array::ones(label.len());
        let label: Arr1f32 = label.clone();
        let nlabel = features.shape()[0];
        let nfeatures = features.shape()[1];

        let features: Arr2f32 = features
            .clone()
            .to_shape((nfeatures * nlabel, 1))
            .unwrap()
            .to_owned();
        let tmp1 = features
            .dot(&Array::ones((1, label.len())))
            .t()
            .to_shape((nlabel, nlabel, nfeatures))
            .unwrap()
            .to_owned();

        let mut tmp = tmp1.clone();
        tmp.swap_axes(0, 1);

        let distances = (tmp1 - tmp).mapv(|v| v * v);
        NNClassifier {
            distances,
            label,
            train_data,
        }
    }
}

impl Classifier for NNClassifier {
    fn test(&self, instid: usize, fset: &Arr1f32) -> f64 {

        // let other = &self.data[instid];
        // self.train_data
        //     .iter()
        //     .map(|i| &self.data[*i])
        //     .map(|d| {
        //         let tmp = fset.get_features();
        //         (
        //             tmp
        //                 .iter()
        //                 .map(|&i| {
        //                     let tmp = other.features[i] - d.features[i];
        //                     tmp * tmp
        //                 })
        //                 .sum::<f64>()
        //                 .sqrt(),
        //             d.label,
        //         )
        //     })
        //     .min_by(|a, b| a.0.total_cmp(&b.0))
        //     .unwrap_or_default()
        //     .1;
        todo!()
    }

    fn train(&mut self, train_data: Arr1f32) {
        self.train_data = train_data;
    }
}

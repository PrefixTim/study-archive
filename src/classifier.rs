use super::feature::FeatureSet;
use super::instance::{InstanceArena, InstanceId, Label};
use itertools::Itertools;

pub trait Classifier {
    fn test(&self, instid: InstanceId, fset: &FeatureSet) -> Label;
    fn train(&mut self, train_data: Vec<usize>);
}

pub struct NNClassifier<'a> {
    data: &'a InstanceArena,
    train_data: Vec<usize>,
}

impl<'a> NNClassifier<'a> {
    pub fn new(data: &'a InstanceArena) -> Self {
        let train_data: Vec<usize> = Vec::with_capacity(0);
        NNClassifier { data, train_data }
    }
}

impl Classifier for NNClassifier<'_> {
    fn test(&self, instid: InstanceId, fset: &FeatureSet) -> Label {
        let other = &self.data[instid];
        self.train_data
            .iter()
            .map(|i| &self.data[*i])
            .map(|d| {
                let tmp = fset.get_features();
                (
                    tmp.iter()
                        .map(|&i| {
                            let tmp = other.features[i] - d.features[i];
                            tmp * tmp
                        })
                        .sum::<f64>()
                        .sqrt(),
                    d.label,
                )
            })
            .min_by(|a, b| a.0.total_cmp(&b.0))
            .unwrap_or_default()
            .1
    }

    fn train(&mut self, train_data: Vec<usize>) {
        self.train_data = train_data;
    }
}

pub struct KNNClassifier<'a> {
    data: &'a InstanceArena,
    train_data: Vec<usize>,
    k: usize,
}

impl<'a> KNNClassifier<'a> {
    pub fn new(data: &'a InstanceArena, k: usize) -> Self {
        let train_data: Vec<usize> = Vec::with_capacity(0);
        Self {
            data,
            train_data,
            k,
        }
    }
}

impl Classifier for KNNClassifier<'_> {
    fn test(&self, instid: InstanceId, fset: &FeatureSet) -> Label {
        let other = &self.data[instid];
        *self.train_data
            .iter()
            .map(|i| &self.data[*i])
            .map(|d| {
                let tmp = fset.get_features();
                (
                    tmp.iter()
                        .map(|&i| {
                            let tmp = other.features[i] - d.features[i];
                            tmp * tmp
                        })
                        .sum::<f64>()
                        .sqrt(),
                    d.label as i32,
                )
            })
            .sorted_by(|a, b| a.0.total_cmp(&b.0))
            .zip((0..self.k).into_iter())
            .counts_by(|e| e.0 .1)
            .iter()
            .max_by(|a, b| a.1.cmp(&b.1))
            .unwrap()
            .0 as f64
    }

    fn train(&mut self, train_data: Vec<usize>) {
        self.train_data = train_data;
    }
}

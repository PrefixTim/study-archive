use super::instance::{InstanceArena, InstanceId, Label};
use super::feature::FeatureSet;

pub trait Classifier {
    fn test(&self, instid: InstanceId, fset: &FeatureSet) -> Label;
    fn train(&mut self, train_data: Vec<usize>);
}

struct NNClassifier<'a> {
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
                fset.get_features()
                    .iter()
                    .map(|&i| {
                        let tmp = other.features[i] - d.features[i];
                        tmp * tmp
                    })
                    .sum::<f64>()
                    .sqrt()
            })
            .max_by(f64::total_cmp)
            .unwrap_or_default()
    }

    fn train(&mut self, train_data: Vec<usize>) {
        self.train_data = train_data;
    }
}

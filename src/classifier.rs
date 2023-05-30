use super::feature::FeatureSet;
use super::instance::{InstanceArena, InstanceId, Label};

pub trait Classifier: Clone {
    fn test(&self, instid: InstanceId, fset: &FeatureSet) -> Label;
    fn train(&mut self, train_data: Vec<usize>);
}

#[derive(Clone)]
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
                    tmp
                        .iter()
                        .map(|&i| {
                            let tmp = other.features[i] - d.features[i];
                            tmp * tmp
                        })
                        .sum::<f64>()
                        .sqrt(),
                    d.label,
                )
            })
            .max_by(|a, b| a.0.total_cmp(&b.0))
            .unwrap_or_default()
            .1
    }

    fn train(&mut self, train_data: Vec<usize>) {
        self.train_data = train_data;
    }
}

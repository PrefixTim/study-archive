use super::{classifier::Classifier, feature::FeatureSet, instance::InstanceArena};

pub struct Evaluator {}
impl Evaluator {
    pub fn eval_node(
        &self,
        fset: &FeatureSet,
        classifier: &impl Classifier,
        data: &InstanceArena,
    ) -> f64 {
        let mut classifier = (*classifier).clone();
        if fset.is_empty() {
            0.
        } else {
            let ids: Vec<usize> = (0..data.len()).collect();
            ids.iter()
                .filter(|i| {
                    let mut train_data = ids.clone();
                    train_data.remove(**i);
                    classifier.train(train_data);
                    let tmp =classifier.test(**i, fset);
                    let tmp = tmp == data[**i].label;
                    tmp
                })
                .count() as f64
                / (data.len() as f64)
        }
    }

    pub fn eval_nodes<'a>(
        &'a self,
        fsets: &'a Vec<FeatureSet>,
        classifier: &impl Classifier,
        data: &InstanceArena,
    ) -> (usize, f64) {
        fsets
            .iter()
            .enumerate()
            .map(|(i, fset)| {
                let eval = self.eval_node(fset, classifier, data);
                println!(
                    "Using feature(s) {:?} accuracy is {}%",
                    fset.get_features(),
                    eval
                );
                (i, eval)
            })
            .max_by(|a, b| a.1.partial_cmp(&b.1).unwrap())
            .unwrap()
    }
}

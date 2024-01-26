use super::{classifier::Classifier, feature::FeatureSet, instance::InstanceArena};

pub struct LooEvaluator {
    k: usize
}

impl LooEvaluator {
    pub fn new(kfold: usize) -> Self { Self { k: kfold } }
}
pub trait Evaluator {
    fn eval_node(
        &self,
        fset: &FeatureSet,
        classifier: &mut dyn Classifier,
        data: &InstanceArena,
    ) -> f64;
    
    fn eval_nodes<'a>(
        &'a self,
        fsets: &'a Vec<FeatureSet>,
        classifier: &mut dyn Classifier,
        data: &InstanceArena,
    ) -> (usize, f64);
}

impl Evaluator for LooEvaluator {
    fn eval_node(
        &self,
        fset: &FeatureSet,
        classifier: &mut dyn Classifier,
        data: &InstanceArena,
    ) -> f64 {
        if fset.is_empty() {
            0.
        } else {
            let batch_size = data.len()/self.k;
            (0..self.k).map(|i| {
                let ids: Vec<usize> = ((batch_size*i)..(batch_size*(i+1))).collect();
                ids.iter()
                .filter(|&j| {
                    let j: usize = *j - i*batch_size;
                    let mut train_data = ids.clone();
                    train_data.remove(j);
                    classifier.train(train_data);
                    let tmp = classifier.test(j, fset);
                    tmp == data[j].label
                })
                .count() as f64
                / (ids.len() as f64)
                * 100f64
            }).sum::<f64>() / self.k as f64
        }
    }

    fn eval_nodes<'a>(
        &'a self,
        fsets: &'a Vec<FeatureSet>,
        classifier: &mut dyn Classifier,
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

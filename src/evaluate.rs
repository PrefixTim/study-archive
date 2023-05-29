use super::feature::FeatureSet;

pub struct Evaluator {}
impl Evaluator {
    pub fn eval_node(&self, node: &FeatureSet) -> f64 {
        if node.is_empty() {
            50.
        } else {
            rand::random::<f64>() * 100.
        }
    }

    pub fn eval_nodes<'a>(&'a self, nodes: &'a Vec<FeatureSet>) -> (usize, f64) {
        nodes
            .iter()
            .enumerate()
            .map(|(i, e)| {
                let eval = self.eval_node(e);
                println!(
                    "Using feature(s) {:?} accuracy is {}%",
                    e.get_features(),
                    eval
                );
                (i, eval)
            })
            .max_by(|a, b| a.1.partial_cmp(&b.1).unwrap())
            .unwrap()
    }
}
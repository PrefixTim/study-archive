pub type Featuer = usize;

#[derive(Debug, Clone)]
pub struct Node {
    feat_state: Vec<bool>,
}

impl Node {
    pub fn new_full(size: usize) -> Self {
        Self {
            feat_state: vec![true; size],
        }
    }
    pub fn new_empty(size: usize) -> Self {
        Self {
            feat_state: vec![false; size],
        }
    }

    // fn is_feat_enbl(&self, feat: Featuer) -> bool {
    //     self.feat_state[feat as usize]
    // }

    fn set_feat(&mut self, feat: &Featuer, state: bool) {
        self.feat_state[*feat as usize] = state;
    }

    fn set_feat_clone(&mut self, feat: &Featuer, state: bool) -> Self {
        let mut node = self.clone();
        node.feat_state[*feat as usize] = state;
        node
    }

    fn is_empty(&self) -> bool {
        !self.feat_state.contains(&true)
    }

    fn get_features(&self) -> Vec<Featuer> {
        self.feat_state
            .iter()
            .enumerate()
            .filter_map(|(i, e)| {
                if *e {
                    Some(i)
                } else {
                    {
                        None
                    }
                }
            })
            .collect()
    }
}
pub struct Evaluator {}
impl Evaluator {
    fn eval_node(&self, node: &Node) -> f64 {
        if node.is_empty() {
            50.
        } else {
            rand::random::<f64>() * 100.
        }
    }

    fn eval_nodes<'a>(&'a self, nodes: &'a Vec<Node>) -> (usize, f64) {
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

pub fn forward_sel(evalr: Evaluator, n_featurees: usize) -> Node {
    let mut unused: Node = Node::new_full(n_featurees);
    let mut max_node = Node::new_empty(n_featurees);
    let mut max_eval = evalr.eval_node(&max_node);
    let mut next_node = max_node.clone();

    println!(
        "Using no features and “random” evaluation, I get an accuracy of {}%",
        max_eval
    );
    println!("\nBeginning search.\n");

    while !unused.is_empty() {
        let feats = unused
            .get_features();
        let nodes = feats
            .iter()
            .map(|e| next_node.set_feat_clone(e, true))
            .collect::<Vec<Node>>();
        let (i, eval) = evalr.eval_nodes(&nodes);
        unused.set_feat(&feats[i], false);
        next_node.set_feat(&feats[i], true);
        if max_eval < eval {
            max_eval = eval;
            max_node = next_node.clone();
            println!("\nFeature set {:?} was best, accuracy is {}%\n", max_node.get_features(), max_eval);
        } else {
            println!("\n(Warning, Accuracy has decreased!)%\n");
        }
    }
    println!("\nFinished search!! The best feature subset is  {:?}, which has an accuracy of {}%\n", max_node.get_features(), max_eval);

    max_node
}

pub fn backward_elim(evalr: Evaluator, n_featurees: usize) -> Node {
    let mut max_node = Node::new_full(n_featurees);
    let mut max_eval = evalr.eval_node(&max_node);
    let mut next_node = max_node.clone();

    println!(
        "Using no features and “random” evaluation, I get an accuracy of {}%",
        max_eval
    );
    println!("\nBeginning search.\n");

    while !next_node.is_empty() {
        let feats = next_node
            .get_features();
        let nodes = feats
            .iter()
            .map(|e| next_node.set_feat_clone(e, false))
            .collect::<Vec<Node>>();
        let (i, eval) = evalr.eval_nodes(&nodes);
        next_node.set_feat(&feats[i], false);
        if max_eval < eval {
            max_eval = eval;
            max_node = next_node.clone();
            println!("\nFeature set {:?} was best, accuracy is {}%\n", max_node.get_features(), max_eval);
        } else {
            println!("\n(Warning, Accuracy has decreased!)%\n");
        }
    }
    println!("\nFinished search!! The best feature subset is  {:?}, which has an accuracy of {}%\n", max_node.get_features(), max_eval);

    max_node
}


use super::{evaluate::Evaluator, classifier::Classifier, instance::InstanceArena, feature::FeatureSet};

pub fn forward_sel(evalr: Evaluator, classifier: &impl Classifier, data: &InstanceArena) -> FeatureSet {
    let mut unused: FeatureSet = FeatureSet::new_full(data.len());
    let mut max_node = FeatureSet::new_empty(data.len());
    let mut max_eval = evalr.eval_node(&max_node, classifier, data);
    let mut next_node = max_node.clone();

    println!(
        "Using no features and “random” evaluation, I get an accuracy of {}%",
        max_eval
    );
    println!("\nBeginning search.\n");

    while !unused.is_empty() {
        let feats = unused.get_features();
        let nodes = feats
            .iter()
            .map(|e| next_node.set_feat_clone(e, true))
            .collect::<Vec<FeatureSet>>();
        let (i, eval) = evalr.eval_nodes(&nodes, classifier, data);
        unused.set_feat(&feats[i], false);
        next_node.set_feat(&feats[i], true);
        if max_eval < eval {
            max_eval = eval;
            max_node = next_node.clone();
            println!(
                "\nFeature set {:?} was best, accuracy is {}%\n",
                max_node.get_features(),
                max_eval
            );
        } else {
            println!("\n(Warning, Accuracy has decreased!)%\n");
        }
    }
    println!(
        "\nFinished search!! The best feature subset is  {:?}, which has an accuracy of {}%\n",
        max_node.get_features(),
        max_eval
    );

    max_node
}

pub fn backward_elim(evalr: Evaluator, classifier: &impl Classifier, data: &InstanceArena) -> FeatureSet {
    let mut max_node = FeatureSet::new_full(data.len());
    let mut max_eval = evalr.eval_node(&max_node, classifier, data);
    let mut next_node = max_node.clone();

    println!("\nBeginning search.\n");
    println!(
        "Using feature(s) {:?} accuracy is {}%",
        max_node.get_features(),
        max_eval
    );
    println!(
        "\nFeature set {:?} was best, accuracy is {}%\n",
        max_node.get_features(),
        max_eval
    );

    while !next_node.is_empty() {
        let feats = next_node.get_features();
        let nodes = feats
            .iter()
            .map(|e| next_node.set_feat_clone(e, false))
            .collect::<Vec<FeatureSet>>();
        let (i, eval) = evalr.eval_nodes(&nodes, classifier, data);
        next_node.set_feat(&feats[i], false);
        if max_eval < eval {
            max_eval = eval;
            max_node = next_node.clone();
            println!(
                "\nFeature set {:?} was best, accuracy is {}%\n",
                max_node.get_features(),
                max_eval
            );
        } else {
            println!("\n(Warning, Accuracy has decreased!)%\n");
        }
    }
    println!(
        "\nFinished search!! The best feature subset is  {:?}, which has an accuracy of {}%\n",
        max_node.get_features(),
        max_eval
    );

    max_node
}

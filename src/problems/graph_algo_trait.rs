use super::node_wrap::LightNode;
use super::problem_trait::{Problem, SearchNode, SolutionStats};
use std::collections::BTreeSet;

pub fn graph_search<'a>(
    problem: &mut impl Problem<'a>,
) -> Result<(SolutionStats, LightNode), ()> {
    let mut frontier: BTreeSet<LightNode> = BTreeSet::new();
    let mut visited: BTreeSet<LightNode> = BTreeSet::new();

    frontier.insert(problem.get_initial_node().clone().into());

    println!("Expanding state:");
    // p.print_node(frontier.);
    //         println!();

    let mut expanded = 0;
    let mut max_queue = 0;
    loop {
        if frontier.is_empty() {
            return Err(());
        }

        expanded += 1;
        max_queue = max_queue.max(frontier.len() as i64);

        let node = frontier.pop_first().unwrap().into();

        //             self.print_expand(&node);

        if problem.is_goal_node(&node) {
            // println!("Goal!!!");
            return Ok((
                SolutionStats::new(expanded, max_queue, node.get_depth()),
                node.into(),
            ));
        }
        let nodes = problem
            .expand(&node)
            .into_iter()
            .map(|e| e.into())
            .filter(|n| !visited.contains(n))
            .filter(|n| !frontier.contains(n))
            .collect::<Vec<LightNode>>();
        nodes.into_iter().for_each(|e| {frontier.insert(e);});
        visited.insert(node.into());
    }
}

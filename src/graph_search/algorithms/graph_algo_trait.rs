use super::node_wrap::NodeWrapper;
use super::problem_trait::{Problem, SearchNode, SolutionStats};
use std::collections::BTreeSet;

pub fn graph_search<'a>(problem: &mut impl Problem<'a>) -> Result<(SolutionStats, NodeWrapper<'a>), ()> {
    let mut frontier: BTreeSet<NodeWrapper> = BTreeSet::new();
    let mut visited: BTreeSet<NodeWrapper> = BTreeSet::new();

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
            println!("Goal!!!");
            return Ok((
                SolutionStats::new(expanded, max_queue, node.get_depth()),
                node.into(),
            ));
        }

        .into_iter()
//             .filter(|n| !self.visited.contains(n))
//             .filter(|n| !frontier.contains(n))
//             .collect::<Vec<Node>>()
        problem.expand(&node).into_iter().for_each(|e| {
            frontier.insert(e.into());
        });
        visited.insert(node.into());
    }
}

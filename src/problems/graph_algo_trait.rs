use crate::problems::problem_trait::Node;

use super::node_wrap::LightNode;
use super::problem_trait::{Problem, SolutionStats};
use std::collections::BinaryHeap;

pub fn graph_search<'a>(problem: &mut impl Problem<'a>) -> Result<(SolutionStats, LightNode), ()> {
    let mut frontier: BinaryHeap<LightNode> = BinaryHeap::new();
    let mut visited: Vec<LightNode> = Vec::new();

    frontier.push((&problem.get_node(0).clone()).into());

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

        let node = problem.get_node(frontier.pop().unwrap().get_id()).clone();

        problem.print_expand(&node);

        if problem.is_goal_node(&node) {
            println!("Goal!!!");
            return Ok((
                SolutionStats::new(expanded, max_queue, node.get_depth()),
                (&node).into(),
            ));
        }

        problem
            .expand(&node)
            .into_iter()
            .map(|n| n.into())
            .filter(|n| !visited.contains(n))
            .for_each(|n| {
                frontier.push(n);
            });
        visited.push((&node).into());
    }
}

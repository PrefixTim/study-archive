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
    let mut best_goal = Some(problem.get_node(1).clone());
    best_goal = None;
    loop {
        if frontier.is_empty() {
            if let Some(goal) = &best_goal {
                println!("Goal!!!");
                let node = best_goal.unwrap();
                return Ok((
                    SolutionStats::new(expanded, max_queue, node.get_depth()),
                    (&node).into(),
                ));
            }
            return Err(());
        }

        max_queue = max_queue.max(frontier.len() as i64);
        
        let node = problem.get_node(frontier.pop().unwrap().get_id()).clone();
        if let Some(goal) = &best_goal {
            if goal.get_cost() <= node.get_cost() {
                println!("Goal!!!");
                let node = best_goal.unwrap();
                return Ok((
                    SolutionStats::new(expanded, max_queue, node.get_depth()),
                    (&node).into(),
                ));
            }
        }
        
        expanded += 1;
        problem.print_expand(&node);

        if problem.is_goal_node(&node) {
            if let Some(goal) = &best_goal {
                if goal.get_cost() > node.get_cost() {
                    best_goal = Some(node);
                }
            } else {
                best_goal = Some(node);
            }
            continue;
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

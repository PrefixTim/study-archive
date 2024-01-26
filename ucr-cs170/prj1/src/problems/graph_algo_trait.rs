use super::problem_trait::{Node, Problem, Solution, SolutionStats};
use std::collections::{HashSet, VecDeque};

pub fn graph_search<P>(problem: &P, print: bool) -> Result<Solution<<P as Problem>::Node>, ()>
where
    P: Problem,
{
    let mut frontier: VecDeque<<P as Problem>::Node> = VecDeque::new();
    let mut visited: HashSet<<P as Problem>::Node> = HashSet::new();

    frontier.push_back(problem.get_init_node());

    if print {
        println!("Expanding state:");
    }

    let mut expanded = 0;
    let mut max_queue = 0;
    loop {
        if frontier.is_empty() {
            return Err(());
        }

        expanded += 1;
        max_queue = max_queue.max(frontier.len() as i64);

        let node = frontier.pop_back().unwrap();

        if print {
            node.print_expand();
        }

        if problem.is_goal_node(&node) {
            if print {
                println!("Goal!!!");
            };
            let depth = node.get_depth();
            let mut trace = vec![node.clone()];
            while let Some(parent) = trace.last().unwrap().get_parent_state() {
                trace.push(visited.get(&parent.into()).unwrap().clone());
            }
            return Ok(Solution {
                trace,
                stats: SolutionStats::new(expanded, max_queue, depth),
            });
        }

        problem
            .expand(&node)
            .into_iter()
            .filter(|n| !visited.contains(n))
            .for_each(|n| {
                let res = frontier.binary_search(&n);
                if let Err(pos) = res {
                    frontier.insert(pos, n);
                }
            });
        visited.insert(node);
    }
}

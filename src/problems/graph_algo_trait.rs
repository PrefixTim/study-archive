use std::collections::VecDeque;

use super::problem_trait::{Node, Problem, Solution, SolutionStats};

pub fn graph_search<P>(problem: &P, print: bool) -> Result<Solution<<P as Problem>::Node>, ()>
where
    P: Problem,
{
    let mut frontier: VecDeque<<P as Problem>::Node> = VecDeque::new();
    let mut visited: Vec<<P as Problem>::Node> = Vec::new();

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
            while  trace.last().unwrap().get_pos() != 0{
                let parent = visited.get(trace.last().unwrap().get_pos()).unwrap().clone();
                trace.push(parent);
            }
            trace.push(problem.get_init_node());
            return Ok(Solution {
                trace,
                stats: SolutionStats::new(expanded, max_queue, depth),
            });
        }

        problem
            .expand(&node)
            .into_iter()
            .filter(|n| visited.binary_search(n).is_err())
            .for_each(|mut n| {
                n.set_pos(visited.len());
                let res = frontier.binary_search(&n);
                if let Err(pos) = res{
                    frontier.insert(pos, n);
                }
            });
        visited.push(node);
    }
}

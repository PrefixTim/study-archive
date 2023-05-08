use super::problem_trait::{BSearchNode, Problem, SearchNode, SolutionStats};
use std::collections::BTreeSet;
pub trait GraphSearchAlgorim<'a> {
    fn new() -> Self;
    fn search(&mut self, p: &mut impl Problem<'a>) -> Result<SolutionStats, ()> {
        let mut frontier: BTreeSet<BSearchNode> = BTreeSet::new();
        let mut visited: BTreeSet<BSearchNode<'a>> = BTreeSet::new();

        frontier.insert(Box::new(p.get_initial_node() as &dyn SearchNode));

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
            max_queue = max_queue.max(frontier.len() as i32);

            let node = frontier.pop_first().unwrap();

            //             self.print_expand(&node);

            //             if self.get_problem().get_goal_node() == node {
            //                 println!("Goal!!!");
            //                 let cost = node.cost;
            //                 let mut trace = vec![node.clone()];
            //                 while let Some(node) = self.trace(trace.last().unwrap()) {
            //                     trace.push(node.clone());
            //                 }
            //                 return Ok(Solution::new(self.get_problem().get_trace(&trace), expanded, max_queue, cost as i32));
            //             }

            //             frontier.append(self.queueingf(&node, &frontier).as_mut());
            //             frontier.sort_by(|a, b| (a.cost+ a.heuristic).partial_cmp(&(b.cost + b.heuristic)).unwrap());
            //             self.mark_visited(node);
        }
    }

    //     fn get_problem(&self) -> &Box<dyn Problem>;
    //     fn reset(&mut self);
    //     fn mark_visited(&mut self, node: Node);
    //     fn queueingf(&mut self, node: &Node, frontier: &Vec<Node>) -> Vec<Node>;
    //     fn trace(&self, node: &Node) -> Option<&Node> ;
    //     fn print_expand(&self, node: &Node) {
    //         println!(
    //             "The best state to expand with g(n) = {} and h(n) = {} is...",
    //             node.cost, node.heuristic
    //         );
    //         self.get_problem().print_node(&node);
    //         println!("Expanding this node\n");
    //     }
}

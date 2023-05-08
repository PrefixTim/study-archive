use super::problem_trait::{Node, Problem, Solution};
pub trait GraphSearchAlgorim {
    fn new(problem: Box<dyn Problem>) -> Self;

    fn search(&mut self) -> Result<Solution, ()> {
        let mut frontier: Vec<Node> = vec![self.get_problem().get_initial_node()];

        loop {
            if frontier.is_empty() {
                return Err(());
            }
            let node = frontier.pop().unwrap();
            if self.get_problem().get_goal_node() == node {
                return Ok(Solution::new(Vec::new()));
            }
            frontier.append(self.queueingf(&node, &frontier).as_mut());
            self.mark_visited(node);
        }
    }

    fn get_problem(&self) -> &Box<dyn Problem>;
    fn reset(&mut self);
    fn mark_visited(&mut self, node: Node);
    fn queueingf(&mut self, node: &Node, frontier: &Vec<Node>) -> Vec<Node>;
}

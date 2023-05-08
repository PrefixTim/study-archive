use super::problem_trait::{Problem, Node, Solution};
trait GraphSearchAlgorim {
    fn new(problem: &Box<dyn Problem>) -> Self;

    fn search(&mut self) -> Result<Solution, ()> {
        let problem = self.get_problem();
        let mut frontier : Vec<Node> = vec![problem.get_initial_state()];
        
        loop {
            if frontier.is_empty() {return Err(());}
            let node = frontier.pop().unwrap();
            if problem.get_goal_state() == node {return Ok(Solution::new(Vec::new()));}
            frontier.append(self.queueingf(&node).as_mut());
            self.mark_visited(node);
        }
    }

    fn get_problem(&self) -> &Box<dyn Problem>;
    fn set_problem(&self, problem: &Box<dyn Problem>);
    fn clear_visited(&mut self);
    fn mark_visited(&self, node: Node);
    fn queueingf(&self, node: &Node) -> Vec<Node>;
}
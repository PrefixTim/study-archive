pub trait Node<'a>: Clone {
    type State;
    fn get_state(&self) -> &Self::State;
    fn get_id(&self) -> usize;
    fn get_cost(&self) -> f64;
    fn get_depth(&self) -> i64;
    fn partial_clone(&self) -> Self;
    fn print(&self);
    fn print_line(&self, i: usize);

}
pub trait Problem<'a> {
    type State;
    type Node: Node<'a, State = Self::State>;
    type Solution: Solution<Node = Self::Node>;

    fn solve(&mut self) -> Result<Self::Solution, ()>;
    fn get_node(&self, id: usize) -> &Self::Node;
    fn get_node_parent(&self, node: &Self::Node) -> Option<& Self::Node>;
    fn is_goal_node(&self, node: &Self::Node) -> bool;
    fn expand(&mut self, node: &Self::Node) -> Vec<&Self::Node>;

    fn print_expand(&self, node: &Self::Node);
}

pub trait Solution {
    type Node;
    fn get_trace(&self) -> &Vec<Self::Node>;
    fn get_stats(&self) -> &SolutionStats;
}
#[derive(Debug)]
pub struct SolutionStats {
    pub expanded: i64,
    pub max_queue: i64,
    pub goal_depth: i64,
}

impl SolutionStats {
    pub fn new(expanded: i64, max_queue: i64, goal_depth: i64) -> Self {
        Self {
            expanded,
            max_queue,
            goal_depth,
        }
    }

    pub fn print_stats(&self) {
        println!(
            "To solve this problem the search algorithm expanded a total of {} nodes.",
            self.expanded
        );
        println!(
            "The maximum number of nodes in the queue at any one time:  {}.",
            self.max_queue
        );
        println!("The depth of the goal node was {}.", self.goal_depth);
    }
}


pub trait PrintTrace {
    fn print_trace(&self);
}
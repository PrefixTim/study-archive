use std::hash::Hash;

pub trait Node: Clone + Ord + Hash + From<Self::State>{
    type State: Hash;
    fn get_state(&self) -> &Self::State;
    fn get_cost(&self) -> f64;
    fn get_depth(&self) -> i64;
    fn get_parent_state(&self) -> Option<Self::State>;
    fn print(&self);
    fn print_line(&self, i: usize);
    fn print_expand(&self);
}
pub trait Problem {
    type State: Hash;
    type Node: Node<State = Self::State>;
    fn solve(&self) -> Result<Solution<Self::Node>, ()>;
    fn expand(&self, node: &Self::Node) -> Vec<Self::Node>;
    fn is_goal_node(&self, node: &Self::Node) -> bool;
    fn get_init_node(&self) -> Self::Node;
}

pub struct Solution<N>
where
    N: Node,
{
    pub trace: Vec<N>,
    pub stats: SolutionStats,
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

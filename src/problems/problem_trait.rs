use super::node_wrap::NodeWrapper;

pub trait Node<'a>: SearchNode + Into<NodeWrapper<'a>> + Clone + From<NodeWrapper<'a>> {
    type State: Ord;
    fn get_state() -> Self::State;
    fn get_parent() -> Option<&'a Self>;
}

pub trait SearchNode {
    fn get_cost(&self) -> f64;
    fn get_depth(&self) -> i64;
}
pub trait Problem<'a> {
    type State;
    type Node: Node<'a, State = Self::State>;
    type Solution;
    fn solve() -> Solution<Self::State>;

    fn get_initial_node(&self) -> &Self::Node;
    fn is_goal_node(&self, node: &Self::Node) -> bool;
    fn expand(&mut self, node: &Self::Node) -> Vec<Self::Node>;
    fn print_node(&self, node: &Self::Node);
}

pub struct Solution<T> {
    pub sol: Vec<T>,
    pub stats: SolutionStats,
}

impl<T> Solution<T> {
    pub fn new(sol: Vec<T>, stats: SolutionStats) -> Self {
        Self { sol, stats }
    }
}

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
}

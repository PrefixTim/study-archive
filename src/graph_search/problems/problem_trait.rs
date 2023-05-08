pub trait Node<'a, State>: SearchNode {
    fn get_state() -> State;
    fn get_parent() -> Option<&'a Self>;
}

pub type BSearchNode<'a> = Box<&'a dyn SearchNode>;
pub trait SearchNode: Ord {
    fn get_cost(&self) -> f64;
    fn get_depth(&self) -> i64;
}
pub trait Problem<'a> {
    type State;
    type Node: Node<'a, Self::State>;
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
    pub fn new(sol: Vec<T>, stats: SolutionStats) -> Self { Self { sol, stats } }
}

pub struct SolutionStats{
    pub expanded: i32,
    pub max_queue: i32,
    pub goal_depth: i32,
}

impl SolutionStats {
    pub fn new(expanded: i32, max_queue: i32, goal_depth: i32) -> Self { Self { expanded, max_queue, goal_depth } }
}


// impl Solution {
//     pub fn new(sol: Vec<Vec<i32>>, expanded: i32, max_queue: i32, goal_depth: i32) -> Self {
//         Self {
//             sol,
//             expanded,
//             max_queue,
//             goal_depth,
//         }
//     }
//     pub fn print(&self) {
//         println!(
//             "To solve this problem the search algorithm expanded a total of {} nodes.",
//             self.expanded
//         );
//         println!(
//             "The maximum number of nodes in the queue at any one time:  {}.",
//             self.max_queue
//         );
//         println!("The depth of the goal node was {}.", self.goal_depth);
//     }
// }
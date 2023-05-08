#[derive(Clone, Debug)]
pub struct Node {
    pub id: usize,
    pub cost: f64,
    pub heuristic: f64,
    pub parent: Option<usize>,
}

impl Node {
    pub fn new(id: usize, cost: f64, parent: Option<usize>) -> Self {
        Self {
            id: id,
            cost: cost,
            heuristic: 0f64,
            parent: parent,
        }
    }

    pub fn set_heuristic(&mut self, heuristic: f64) {
        self.heuristic = heuristic;
    }
}

impl PartialEq for Node {
    fn eq(&self, other: &Self) -> bool {
        self.id.eq(&other.id)
    }
}

pub trait Problem {
    fn print_node(&self, node: &Node);
    fn get_initial_node(&self) -> Node;
    fn get_goal_node(&self) -> Node;
    fn expand(&mut self, node: &Node) -> Vec<Node>;
    fn get_heuristic_node(&self, node: &Node) -> f64;
    fn get_trace(&self, nodes: &Vec<Node>) -> Vec<Vec<i32>>;
}

pub struct Solution {
    pub sol: Vec<Vec<i32>>,
    pub expanded: i32,
    pub max_queue: i32,
    pub goal_depth: i32,
}

impl Solution {
    pub fn new(sol: Vec<Vec<i32>>, expanded: i32, max_queue: i32, goal_depth: i32) -> Self {
        Self {
            sol,
            expanded,
            max_queue,
            goal_depth,
        }
    }
    pub fn print(&self) {
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

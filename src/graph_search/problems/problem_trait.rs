#[derive( Clone, Debug)]
pub struct Node {
    pub id: usize,
    pub cost: f64,
    pub heuristic: f64,
}

impl Node {
    pub fn new(id: usize, cost: f64) -> Self {
        Self {
            id: id,
            cost: cost,
            heuristic: 0f64,
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

pub type Operator = fn(&Node) -> Vec<&Node>;
pub trait Problem {
    fn print_node(&self, node: &Node);
    fn get_initial_node(&self) -> Node;
    fn get_goal_node(&self) -> Node;
    // fn get_operators(&self) -> &Vec<Operator>;
    fn expand(&mut self, node: &Node) -> Vec<Node>; //{
                                                    // self.get_operators().iter().flat_map(|op| op(state).into_iter()).collect::<Vec<&Node>>()
                                                    // }
    fn get_heuristic_node(&self, node: &Node) -> f64;
}

pub struct Solution {
    sol: Vec<usize>,
    expanded: i32,
    max_queue: i32,
    goal_depth: i32,
}

impl Solution {
    pub fn new(sol: Vec<usize>, expanded: i32, max_queue: i32, goal_depth: i32) -> Self {
        Self {
            sol,
            expanded,
            max_queue,
            goal_depth,
        }
    }
    pub fn print(&self) {
        println!("\nTo solve this problem the search algorithm expanded a total of {} nodes.\nThe maximum number of nodes in the queue at any one time:  {}.\nThe depth of the goal node was {}.", self.expanded, self.max_queue, self.goal_depth);
    }
}

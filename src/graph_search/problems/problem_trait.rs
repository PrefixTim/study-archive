
#[derive(Eq, PartialEq, Clone, Hash)]
pub struct Node {
    pub id: usize
}

impl Node {
    pub fn new(state: usize) -> Self { Self { id: state } }
}

impl From<usize> for Node {
    fn from(value: usize) -> Self {
        Node::new(value)
    }
}

pub type Operator = fn(&Node) -> Vec<&Node>;
pub trait Problem {
    fn get_initial_node(&self) -> &Node;
    fn get_goal_node(&self) -> &Node;
    // fn get_operators(&self) -> &Vec<Operator>;
    fn expand(&self, node: &Node) -> Vec<Node>; //{
        // self.get_operators().iter().flat_map(|op| op(state).into_iter()).collect::<Vec<&Node>>()
    // }
    fn get_value_node(&self, node: &Node) -> i32;
    fn get_heuristic_node(&self, node: &Node) -> i32;
}


pub struct Solution(Vec<i32>);

impl Solution {
    pub fn new(sol: Vec<i32>) -> Self {
        Solution(sol)
    }
}

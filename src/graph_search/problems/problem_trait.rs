
#[derive(Eq, PartialEq)]
pub struct Node {
    state: i32
}

impl std::hash::Hash for Node {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.state.hash(state);
    }
}

type Operator = fn(&Node) -> Vec<Node>;
pub trait Problem {
    fn get_initial_state(&self) -> Node;
    fn get_goal_state(&self) -> Node;
    fn get_operators(&self) -> &Vec<Operator>;
    fn expand(&self, state: &Node) -> Vec<Node> {
        self.get_operators().iter().flat_map(|op| op(state).into_iter()).collect::<Vec<Node>>()
    }
}


pub struct Solution(Vec<i32>);

impl Solution {
    pub fn new(sol: Vec<i32>) -> Self {
        Solution(sol)
    }
}

use std::cmp::{Ordering, Reverse};

use super::problem_trait::Node;
#[derive(Debug, Clone)]
pub struct LightNode {
    id: usize,
    round_cost: i64,
}

impl LightNode {
    pub fn new(id: usize, cost: f64) -> Self {
        let round_cost = (cost * 1000.) as i64;
        Self { id, round_cost }
    }

    pub fn get_id(&self) -> usize{
        self.id
    }
}

impl<'a, T> From<&T> for LightNode where T: Node<'a>{
    fn from(value: &T) -> Self {
        LightNode::new(value.get_id(), value.get_cost())
    }
}

impl Ord for LightNode {
    fn cmp(&self, other: &Self) -> Ordering {
        Reverse(self.round_cost).cmp(&Reverse(other.round_cost))
    }
}

impl PartialOrd for LightNode {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for LightNode {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl Eq for LightNode {}

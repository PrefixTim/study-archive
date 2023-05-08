use std::cmp::Ordering;

use super::problem_trait::SearchNode;

pub struct NodeWrapper<'a> {
    pub node: Box<&'a dyn SearchNode>,
    round_cost: i64,
}

impl<'a> NodeWrapper<'a> {
    pub fn new(node: Box<&'a dyn SearchNode>) -> Self {
        let round_cost = (node.get_cost() * 1000.) as i64;
        Self { node, round_cost }
    }
}

// impl<'a, B> From<NodeWrapper<'a>> for &<B as Problem<'a>>::Node where B:Problem<'a>{
//     fn from(val: NodeWrapper) -> Self {
//         todo!()
//     }
// }

impl Ord for NodeWrapper<'_> {
    fn cmp(&self, other: &Self) -> Ordering {
        self.round_cost.cmp(&other.round_cost)
    }
}

impl PartialOrd for NodeWrapper<'_> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for NodeWrapper<'_> {
    fn eq(&self, other: &Self) -> bool {
        self.round_cost == other.round_cost
    }
}

impl Eq for NodeWrapper<'_> {}

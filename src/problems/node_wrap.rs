use std::cmp::Ordering;
pub struct LightNode {
    id: i64,
    round_cost: i64,
}

impl<'a> LightNode {
    pub fn new(id: i64, cost: f64) -> Self {
        let round_cost = (cost * 1000.) as i64;
        Self { id, round_cost }
    }
}

impl Ord for LightNode {
    fn cmp(&self, other: &Self) -> Ordering {
        self.round_cost.cmp(&other.round_cost)
    }
}

impl PartialOrd for LightNode {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for LightNode {
    fn eq(&self, other: &Self) -> bool {
        self.round_cost == other.round_cost
    }
}

impl Eq for LightNode {}

pub mod graph_algo_trait;
pub mod uni_cost_search;

use super::problems::problem_trait;

struct AstarMth;
struct AstarEdh;

pub fn zero_heuristic(state: &Vec<i32>) -> i32 {
    0
}
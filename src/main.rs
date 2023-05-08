
mod graph_search;

use graph_search::{problems::npuzle::{ Npuzle}, algorithms::{zero_heuristic, uni_cost_search::UniCostSearch, graph_algo_trait::GraphSearchAlgorim}};
fn main() {
    let init_s = vec![1, 2, 3, 4, 8, 0, 7, 6, 5];
    let puzle = Npuzle::new(init_s, zero_heuristic).unwrap();
    let mut ucs = UniCostSearch::new(Box::new(puzle));
    ucs.search();
}







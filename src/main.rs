
mod graph_search;

use graph_search::{problems::npuzle::{ Npuzle, misplaced_tile_heuristic, zero_heuristic}, algorithms::{ astar::Astar, graph_algo_trait::GraphSearchAlgorim}};
fn main() {
    let init_s = vec![1, 2, 3, 4, 8, 0, 7, 6, 5];
    let puzle = Npuzle::new(init_s, zero_heuristic).unwrap();
    let mut ucs = Astar::new(Box::new(puzle));
    if let Ok(sol) = ucs.search() {
        sol.print();
    } else {
        println!("Failed to reach a goal state");
    }
}







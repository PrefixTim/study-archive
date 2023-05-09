pub mod problems;

use problems::{npuzle::{NpState, Npuzle, NpSolution}, problem_trait::{Problem}};

pub fn npuzzle_searh(init_s: Vec<i64>, heuristic: fn(&NpState) -> f64) -> Result<NpSolution, ()> {
    Npuzle::new(init_s, heuristic, true).unwrap().solve()
}
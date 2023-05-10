use problems::{
    npuzzle::{
        np_node::{NpNode, NpState},
        np_problem::Npuzle,
    },
    problem_trait::{Problem, Solution},
};

pub mod problems;

pub fn npuzzle_searh(
    init_s: Vec<i64>,
    heuristic: fn(&NpState) -> f64,
) -> Result<Solution<NpNode>, ()> {
    let puzzle = Npuzle::new(init_s, heuristic, true).unwrap();
    puzzle.solve()
}

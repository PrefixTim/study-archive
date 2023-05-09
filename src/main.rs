mod problems;

use problems::npuzle::{
    euclidean_distance_heuristic, misplaced_tile_heuristic, zero_heuristic, NpState, Npuzle,
};
use std::io::{self};

use crate::problems::problem_trait::{PrintTrace, Problem, Solution};

//1, 2, 3, 4, 8, 0, 7, 6, 5
//1, 0, 3, 4, 2, 6, 7, 5, 8
//1, 0, 3, 4, 2, 6, 7, 5, 8
//1, 2, 3, 4, 5, 6, 7, 8, 0
//1, 2, 0, 4, 5, 6, 7, 8, 6
//8, 7, 1, 6, 0, 2, 5, 4, 3
//1, 2, 3, 4, 5, 6, 7, 0, 8
//0, 1, 2, 4, 5, 3, 7, 8, 6
//1, 2, 3, 4, 5, 6, 8, 7, 0

// To solve this problem the search algorithm expanded a total of 87048 nodes.
// The maximum number of nodes in the queue at any one time:  25040.
// The depth of the goal node was 22.
fn main() {
    println!("Welcome to 862311452; 8 puzzle solver. Enter your own puzzle");
    let (init_s, heuristic) = get_input();

    let mut search = Npuzle::new(init_s, heuristic).unwrap();

    if let Ok(sol) = search.solve() {
        sol.get_stats().print_stats();
        sol.get_trace().print_trace();
    } else {
        println!("Failed to reach a goal state");
    }
}

fn get_input() -> (NpState, fn(&NpState) -> f64) {
    let mut buffer = String::new();

    io::stdin().read_line(&mut buffer).unwrap();
    let init_s = buffer
        .trim()
        .split(", ")
        .map(|s| s.parse().unwrap())
        .collect::<Vec<i64>>();

    println!(
        "Enter your choice of algorithm
    Uniform Cost Search
    A* with the Misplaced Tile heuristic.
    A* with the Euclidean distance heuristic."
    );

    buffer = String::new();
    io::stdin().read_line(&mut buffer).unwrap();

    println!("{}", buffer);
    let heuristic = match buffer.trim().parse::<i32>().unwrap() {
        1 => zero_heuristic,
        2 => misplaced_tile_heuristic,
        3 => euclidean_distance_heuristic,
        _ => {
            panic!("Invalid Choise!")
        }
    };
    (init_s, heuristic)
}

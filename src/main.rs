mod graph_search;

use graph_search::{
    algorithms::{astar::Astar, graph_algo_trait::GraphSearchAlgorim},
    problems::npuzle::{
        euclidean_distance_heuristic, misplaced_tile_heuristic, zero_heuristic, Npuzle,
    },
};
use std::io;

//1, 2, 3, 4, 8, 0, 7, 6, 5
fn main() -> io::Result<()> {

    println!("Welcome to  8 puzzle solver. Enter your own puzzle");

    let mut buffer = String::new();

    io::stdin().read_line(&mut buffer)?;
    let init_s = buffer
        .trim()
        .split(", ")
        .map(|s| s.parse().unwrap())
        .collect::<Vec<i32>>();

    println!("Enter your choice of algorithm
    Uniform Cost Search
    A* with the Misplaced Tile heuristic.
    A* with the Euclidean distance heuristic.");

    buffer = String::new();
    io::stdin().read_line(&mut buffer)?;
    println!("{}", buffer);
    let heu = match buffer.trim().parse::<i32>().unwrap() {
        1 => zero_heuristic,
        2 => misplaced_tile_heuristic,
        3 => euclidean_distance_heuristic,
        _ => {
            println!("Invalid choise!");
            return Ok(());
        }
    };

    let puzle = Npuzle::new(init_s, heu).unwrap();

    let mut search = Astar::new(Box::new(puzle));

    if let Ok(sol) = search.search() {
    sol.print();
    } else {
        println!("Failed to reach a goal state");
    }

    Ok(())
}

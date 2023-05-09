mod problems;

use problems::npuzle::{
    euclidean_distance_heuristic, misplaced_tile_heuristic, zero_heuristic, Npuzle,
};
use std::io;

use crate::problems::problem_trait::Problem;

//1, 2, 3, 4, 8, 0, 7, 6, 5
fn main() -> io::Result<()> {
    println!("Welcome to 862311452; 8 puzzle solver. Enter your own puzzle");

    let mut buffer = String::new();

    io::stdin().read_line(&mut buffer)?;
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

    // let n: usize = (init_s.len() as f64).sqrt() as usize;
    let mut search = Npuzle::new(init_s, heu).unwrap();
    let res = search.solve().unwrap();
    println!("{:?}", res);
    // if let Ok(sol) = search.search() {
    //     sol.print();
    //     for i in 0..n {
    //         for s in (&sol.sol).iter().rev() {
    //             for j in 0..n {
    //                 print!("{} ", s[i * n + j]);
    //             }
    //             print!("   ");
    //         }
    //         println!("   ");
    //     }
    // } else {
    //     println!("Failed to reach a goal state");
    // }

    Ok(())
}

//     fn print_expand(&self, node: &Node) {
//         println!(
//             "The best state to expand with g(n) = {} and h(n) = {} is...",
//             node.cost, node.heuristic
//         );
//         self.get_problem().print_node(&node);
//         println!("Expanding this node\n");
//     }

//     pub fn print(&self) {
//         println!(
//             "To solve this problem the search algorithm expanded a total of {} nodes.",
//             self.expanded
//         );
//         println!(
//             "The maximum number of nodes in the queue at any one time:  {}.",
//             self.max_queue
//         );
//         println!("The depth of the goal node was {}.", self.goal_depth);
//     }

//     fn print_node(&self, node: &Node) {
//         let s = self.state_set.get(node.id as usize).unwrap().clone();
//         for i in 0..self.n {
//             for j in 0..self.n {
//                 print!("{} ", s[i * self.n + j]);
//             }
//             println!("");
//         }
//     }

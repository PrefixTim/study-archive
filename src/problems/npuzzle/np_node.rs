use std::{
    cmp::{Ordering, Reverse},
    hash::Hash,
};

use crate::problems::problem_trait::{Node, PrintTrace};

pub type NpState = Vec<i64>;
#[derive(Clone)]
pub struct NpNode {
    state: NpState,
    depth: i64,
    heuristic: f64,
    round_cost: i64,
    pos: usize
}

impl NpNode {
    pub fn new(heuristic: f64, state: NpState, depth: i64) -> Self {
        let round_cost = depth * 1000 + (heuristic * 1000.) as i64;
        Self {
            state,
            depth,
            heuristic,
            round_cost,
            pos: 0
        }
    }

    pub fn get_heuristic(&self) -> f64 {
        self.heuristic
    }
}

impl Ord for NpNode {
    fn cmp(&self, other: &Self) -> Ordering {
        Reverse(self.round_cost).cmp(&Reverse(other.round_cost))
    }
}

impl PartialOrd for NpNode {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl PartialEq for NpNode {
    fn eq(&self, other: &Self) -> bool {
        self.state == other.state
    }
}

impl Eq for NpNode {}

impl Node for NpNode {
    type State = NpState;

    fn get_state(&self) -> &Self::State {
        &self.state
    }

    fn get_cost(&self) -> f64 {
        self.depth as f64 + self.heuristic
    }

    fn get_depth(&self) -> i64 {
        self.depth
    }

    fn print(&self) {
        let n = (self.state.len() as f64).sqrt() as usize;
        for i in 0..n {
            for j in 0..n {
                print!("{} ", self.state[i * n + j]);
            }
            print!("\n");
        }
    }

    fn print_line(&self, i: usize) {
        let n = (self.state.len() as f64).sqrt() as usize;
        for j in 0..n {
            print!("{} ", self.state[i * n + j]);
        }
    }

    fn print_expand(&self) {
        print!(
            "The best state to expand with g(n) = {} and h(n) = {} is...\n",
            self.depth, self.heuristic
        );
        self.print();
        print!("Expanding this node\n\n");
    }

    fn get_pos(&self) -> usize {
        self.pos
    }

    fn set_pos(&mut self, pos: usize) {
        self.pos = pos;
    }
}

impl Hash for NpNode {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.state.hash(state);
    }
}

impl<'a> PrintTrace for &Vec<NpNode> {
    fn print_trace(&self) {
        let n = (self.first().unwrap().get_state().len() as f64).sqrt() as usize;
        for i in 0..n {
            for s in self.iter().rev() {
                s.print_line(i);
                if i != n - 1 {
                    print!("    ")
                } else {
                    print!(" => ")
                }
            }
            println!();
        }
    }
}


pub fn zero_heuristic(_state: &NpState) -> f64 {
    0f64
}

pub fn misplaced_tile_heuristic(state: &NpState) -> f64 {
    state
        .iter()
        .enumerate()
        .filter(|(i, &e)| e != (((i + 1) % state.len()) as i64))
        .count() as f64
}

pub fn euclidean_distance_heuristic(state: &NpState) -> f64 {
    let n: i64 = (state.len() as f64).sqrt() as i64;
    state
        .iter()
        .enumerate()
        .map(|(i, &e)| {
            let (x1, y1, x2, y2);
            x1 = e % n;
            y1 = e / n;
            x2 = (i + 1) as i64 % n;
            y2 = (i + 1) as i64 / n;
            (((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) as f64).sqrt()
        })
        .sum()
}

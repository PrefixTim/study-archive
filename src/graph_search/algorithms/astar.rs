use crate::graph_search::problems::problem_trait::{Node, Problem};

use super::graph_algo_trait::GraphSearchAlgorim;

pub struct Astar {
    problem: Box<dyn Problem>,
    visited: Vec<Node>,
}

impl GraphSearchAlgorim for Astar {
    fn new(problem: Box<dyn Problem>) -> Self {
        Self {
            problem: problem,
            visited: Vec::new(),
        }
    }

    fn get_problem(&self) -> &Box<dyn Problem> {
        &self.problem
    }

    fn reset(&mut self) {
        self.visited = Vec::new();
    }

    fn mark_visited(&mut self, node: Node) {
        self.visited.push(node);
    }

    fn queueingf(&mut self, node: &Node, frontier: &Vec<Node>) -> Vec<Node> {
        self.problem
            .expand(node)
            .into_iter()
            .filter(|n| !self.visited.contains(n))
            .filter(|n| !frontier.contains(n))
            .collect::<Vec<Node>>()
    }
}

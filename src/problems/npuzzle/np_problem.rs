use crate::problems::{
    graph_algo_trait::graph_search,
    problem_trait::{Node, Problem, Solution},
};

use super::np_node::{NpNode, NpState};

pub struct Npuzle {
    n: usize,
    heuristic: fn(&NpState, usize) -> f64,
    init_node: NpNode,
    goal_node: NpNode,
    print: bool,
}

impl Npuzle {
    pub fn new(
        init_state: NpState,
        heuristic: fn(&NpState, usize) -> f64,
        print: bool,
    ) -> Result<Self, ()> {
        let n = (init_state.len() as f64).sqrt().floor() as usize;
        let nn = n * n;
        if nn != init_state.len() {
            return Err(());
        }

        let mut goal_state: NpState = (1..(nn as i64)).into_iter().collect();
        goal_state.push(0);

        let goal_node = NpNode::new(heuristic(&goal_state, n), goal_state, 0, None);
        let init_node = NpNode::new(heuristic(&init_state, n), init_state, 0, None);

        Ok(Self {
            n,
            heuristic,
            init_node,
            goal_node,
            print,
        })
    }

    fn oper(&self, node: &NpNode, pos: usize, n_pos: usize) -> NpNode {
        let heuristic = self.heuristic;
        let mut n_state = node.get_state().clone();
        n_state.swap(pos, n_pos);
        NpNode::new(heuristic(&n_state, self.n), n_state, node.get_depth() + 1, Some(node.get_state().clone()))
    }
}

impl Problem for Npuzle {
    type State = NpState;
    type Node = NpNode;

    fn expand(&self, node: &Self::Node) -> Vec<Self::Node> {
        let mut res = Vec::new();
        let pos: usize = node.get_state().iter().position(|n| n == &0).unwrap();

        if pos >= self.n {
            //Up
            res.push(self.oper(node, pos, pos - self.n));
        }
        if pos % self.n != 0 {
            //Left
            res.push(self.oper(node, pos, pos - 1));
        }
        if pos + self.n < self.n * self.n {
            //Down
            res.push(self.oper(node, pos, pos + self.n));
        }
        if pos % self.n != self.n - 1 {
            //Right
            res.push(self.oper(node, pos, pos + 1));
        }
        res
    }

    fn is_goal_node(&self, node: &Self::Node) -> bool {
        &self.goal_node == node
    }

    fn get_init_node(&self) -> Self::Node {
        self.init_node.clone()
    }

    fn solve(&self) -> Result<Solution<Self::Node>, ()> {
        graph_search(self, self.print)
    }
}

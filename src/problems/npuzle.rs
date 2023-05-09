use super::{
    graph_algo_trait::graph_search,
    problem_trait::{Node, PrintTrace, Problem, Solution, SolutionStats},
};

pub type NpState = Vec<i64>;
#[derive(Clone)]
pub struct NpNode {
    id: usize,
    state: NpState,
    depth: i64,
    heuristic: f64,
    parent: Option<usize>,
}

impl NpNode {
    pub fn new(
        id: usize,
        state: NpState,
        depth: i64,
        heuristic: f64,
        parent: Option<usize>,
    ) -> Self {
        Self {
            id,
            state,
            depth,
            heuristic,
            parent,
        }
    }
}

impl<'a> Node<'a> for NpNode {
    type State = NpState;

    fn get_state(&self) -> &Self::State {
        &self.state
    }

    fn get_id(&self) -> usize {
        self.id
    }

    fn get_cost(&self) -> f64 {
        self.depth as f64 + self.heuristic
    }

    fn get_depth(&self) -> i64 {
        self.depth
    }

    fn partial_clone(&self) -> Self {
        NpNode::new(self.id, Vec::new(), self.depth, self.heuristic, None)
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
}

pub struct NpSolution {
    trace: Vec<NpNode>,
    stats: SolutionStats,
}

impl Solution for NpSolution {
    type Node = NpNode;
    fn get_trace(&self) -> &Vec<Self::Node> {
        &self.trace
    }

    fn get_stats(&self) -> &SolutionStats {
        &self.stats
    }
}

pub struct Npuzle {
    n: usize,
    state_set: Vec<NpNode>,
    heuristic: fn(&NpState) -> f64,
}

impl Npuzle {
    pub fn new(init_state: NpState, heuristic: fn(&NpState) -> f64) -> Result<Self, ()> {
        let n = (init_state.len() as f64).sqrt().floor() as usize;
        let nn = n * n;
        if nn != init_state.len() {
            return Err(());
        }

        let init_heuristic = heuristic(&init_state);
        let init_node = NpNode::new(0, init_state, 0, init_heuristic, None);

        let mut tmp: NpState = (1..(nn as i64)).into_iter().collect();
        tmp.push(0);
        let goal_heuristic = heuristic(&tmp);
        let goal_node = NpNode::new(1, tmp, 0, goal_heuristic, None);

        Ok(Self {
            n: n,
            state_set: Vec::from([init_node, goal_node]),
            heuristic: heuristic,
        })
    }

    fn oper(&mut self, node: &NpNode, pos: usize, n_pos: usize) -> Option<usize> {
        let heuristic = self.heuristic;
        let mut n_state = node.state.clone();
        n_state.swap(pos, n_pos);

        let exist = self.state_set.iter().position(|n| n.state == n_state);
        let h_val = heuristic(&n_state);
        if let Some(i) = exist {
            if i != 1 {
                return None;
            }
            let old_node = self.state_set.get_mut(i).unwrap();
            old_node.depth = node.depth + 1;
            old_node.heuristic = h_val;
            old_node.parent = Some(node.id);
            Some(i)
        } else {
            self.state_set.push(NpNode::new(
                self.state_set.len(),
                n_state,
                node.depth + 1,
                h_val,
                Some(node.id),
            ));
            Some(self.state_set.len() - 1)
        }
    }
}

impl<'a> Problem<'a> for Npuzle {
    type State = NpState;
    type Node = NpNode;
    type Solution = NpSolution;

    fn solve(&mut self) -> Result<Self::Solution, ()> {
        let res = graph_search(self)?;

        let mut trace = vec![self.get_node(res.1.get_id()).clone()];
        let mut nd = self.get_node(res.1.get_id());
        while let Some(node) = self.get_node_parent(nd) {
            nd = self.get_node(node.id);
            trace.push(node.clone());
        }
        let sol = NpSolution {
            trace: trace,
            stats: res.0,
        };
        Ok(sol)
    }

    fn get_node(&self, id: usize) -> &Self::Node {
        self.state_set.get(id).unwrap()
    }

    fn get_node_parent(&self, node: &Self::Node) -> Option<&Self::Node> {
        if let Some(parent) = node.parent {
            Some(self.state_set.get(parent).unwrap())
        } else {
            None
        }
    }

    fn expand(&mut self, node: &Self::Node) -> Vec<&Self::Node> {
        let mut res: Vec<usize> = Vec::new();
        let pos: usize = node.state.iter().position(|n| n == &0).unwrap();

        if pos >= self.n {
            //Up
            if let Some(i) = self.oper(node, pos, pos - self.n) {
                res.push(i);
            }
        }
        if pos % self.n != 0 {
            //Left
            if let Some(i) = self.oper(node, pos, pos - 1) {
                res.push(i);
            }
        }
        if pos + self.n < self.n * self.n {
            //Down
            if let Some(i) = self.oper(node, pos, pos + self.n) {
                res.push(i);
            }
        }
        if pos % self.n != self.n - 1 {
            //Right
            if let Some(i) = self.oper(node, pos, pos + 1) {
                res.push(i);
            }
        }
        res.into_iter()
            .map(|e| self.state_set.get(e).unwrap())
            .collect()
    }

    fn is_goal_node(&self, node: &Self::Node) -> bool {
        node.id == 1
    }

    fn print_expand(&self, node: &Self::Node) {
        print!(
            "The best state to expand with g(n) = {} and h(n) = {} is...\n",
            node.depth, node.heuristic
        );
        node.print();
        print!("Expanding this node\n\n");
    }
}

impl PrintTrace for &Vec<NpNode> {
    fn print_trace(&self) {
        let n = (self.first().unwrap().state.len() as f64).sqrt() as usize;
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

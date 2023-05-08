use super::{
    node_wrap::LightNode,
    problem_trait::{Node, Problem, SearchNode},
};

#[derive(Clone)]
pub struct NpNode {}

impl<'a> Node<'a> for NpNode {
    type State = Vec<i32>;

    fn get_state() -> Self::State {
        todo!()
    }

    fn get_parent() -> Option<&'a Self> {
        todo!()
    }
}

impl<'a> Into<LightNode> for NpNode {
    fn into(self) -> LightNode {
        todo!()
    }
}

impl<'a> From<LightNode> for NpNode {
    fn from(value: LightNode) -> Self {
        todo!()
    }
}

impl SearchNode for NpNode {
    fn get_cost(&self) -> f64 {
        todo!()
    }

    fn get_depth(&self) -> i64 {
        todo!()
    }
}

pub struct Npuzle {
    n: usize,
    // state_set: Vec<State>,
    heuristic: fn(&Vec<i32>) -> f64, // ops: Vec<Operator>
}

impl Npuzle {
    pub fn new(init_state: Vec<i32>, heuristic: fn(&Vec<i32>) -> f64) -> Result<Self, ()> {
        let n = (init_state.len() as f64).sqrt().floor() as usize;
        if n * n != init_state.len() {
            return Err(());
        }

        let is = init_state;
        let mut tmp: Vec<i32> = (1..(is.len() as i32)).into_iter().collect();
        tmp.push(0);
        let gs = tmp;
        Ok(Self {
            n: n,
            // state_set: Vec::from([is, gs]),
            heuristic: heuristic, // ops: Vec::new()
        })
    }
    // fn init_ops(&mut self) {
    //     self.ops.append(self.up);
    // }
    // fn up(&self, node: &Node) -> Vec<&Node> {
    //     // let v =
    //     todo!()
    // }
}

// impl Problem for Npuzle {
//     fn get_initial_node(&self) -> Node {
//         let mut n: Node = Node::new(0, 0f64, None);
//         n.set_heuristic(self.get_heuristic_node(&n));
//         n
//     }

//     fn get_goal_node(&self) -> Node {
//         let mut n: Node = Node::new(1, 0f64, None);
//         n.set_heuristic(self.get_heuristic_node(&n));
//         n
//     }

//     fn expand(&mut self, node: &Node) -> Vec<Node> {
//         // up
//         let s = self.state_set.get(node.id as usize).unwrap().clone();
//         let mut res = Vec::new();
//         let pos: usize = s.iter().position(|n| n == &0).unwrap();
//         let mut n: Node;
//         if pos >= self.n {
//             let mut n_state = s.clone();
//             n_state.swap(pos, pos - self.n);
//             let exist = self.state_set.iter().position(|n| n == &n_state);
//             if let Some(i) = exist {
//                 n = Node::new(i, node.cost + 1f64, Some(node.id));
//             } else {
//                 n = Node::new(self.state_set.len(), node.cost + 1f64, Some(node.id));
//                 self.state_set.push(n_state);
//             }
//             n.set_heuristic(self.get_heuristic_node(&n));
//             res.push(n)
//         }
//         if pos % self.n != 0 {
//             let mut n_state = s.clone();
//             n_state.swap(pos, pos - 1);
//             let exist = self.state_set.iter().position(|n| n == &n_state);
//             if let Some(i) = exist {
//                 n = Node::new(i, node.cost + 1f64, Some(node.id));
//             } else {
//                 n = Node::new(self.state_set.len(), node.cost + 1f64, Some(node.id));
//                 self.state_set.push(n_state);
//             }
//             n.set_heuristic(self.get_heuristic_node(&n));
//             res.push(n)
//         }
//         if pos + self.n < self.n * self.n {
//             let mut n_state = s.clone();
//             n_state.swap(pos, pos + self.n);
//             let exist = self.state_set.iter().position(|n| n == &n_state);
//             if let Some(i) = exist {
//                 n = Node::new(i, node.cost + 1f64, Some(node.id));
//             } else {
//                 n = Node::new(self.state_set.len(), node.cost + 1f64, Some(node.id));
//                 self.state_set.push(n_state);
//             }
//             n.set_heuristic(self.get_heuristic_node(&n));
//             res.push(n)
//         }
//         if pos % self.n != self.n - 1 {
//             let mut n_state = s.clone();
//             n_state.swap(pos, pos + 1);
//             let exist = self.state_set.iter().position(|n| n == &n_state);
//             if let Some(i) = exist {
//                 n = Node::new(i, node.cost + 1f64, Some(node.id));
//             } else {
//                 n = Node::new(self.state_set.len(), node.cost + 1f64, Some(node.id));
//                 self.state_set.push(n_state);
//             }
//             n.set_heuristic(self.get_heuristic_node(&n));
//             res.push(n)
//         }
//         res
//     }

//     fn get_heuristic_node(&self, node: &Node) -> f64 {
//         let state = self.state_set.get(node.id).unwrap();
//         let h = self.heuristic;
//         h(state)
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

//     fn get_trace(&self, nodes: &Vec<Node>) -> Vec<Vec<i32>>{
//         let mut res = Vec::new();
//         for node in nodes {
//             res.push(self.state_set.get(node.id).unwrap().clone());
//         }
//         res
//     }
// }

pub fn zero_heuristic(state: &Vec<i32>) -> f64 {
    0f64
}

pub fn misplaced_tile_heuristic(state: &Vec<i32>) -> f64 {
    state
        .iter()
        .enumerate()
        .filter(|(i, &e)| e != (((i + 1) % state.len()) as i32))
        .count() as f64
}

pub fn euclidean_distance_heuristic(state: &Vec<i32>) -> f64 {
    let n: i32 = (state.len() as f64).sqrt() as i32;
    state
        .iter()
        .enumerate()
        .map(|(i, &e)| {
            let (x1, y1, x2, y2);
            x1 = e % n;
            y1 = e / n;
            x2 = (i + 1) as i32 % n;
            y2 = (i + 1) as i32 / n;
            (((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) as f64).sqrt()
        })
        .sum()
}

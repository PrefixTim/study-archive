use super::problem_trait::{Node, Problem};

type State = Vec<i32>;
pub struct Npuzle {
    n: usize,
    init_state: State,
    goal_state: State,
    state_set: Vec<State>,
    heuristic: fn(&Vec<i32>) -> i32, // ops: Vec<Operator>
}

impl Npuzle {
    pub fn new(init_state: Vec<i32>, heuristic: fn(&Vec<i32>) -> i32) -> Result<Self, ()> {
        let is = init_state;
        let mut tmp: Vec<i32> = (1..(is.len() as i32)).into_iter().collect();
        tmp.push(0);
        let gs = tmp;
        let tmp = Vec::from([is.clone(), gs.clone()]);
        let n = (is.len() as f64).sqrt().floor() as usize;
        if n * n == is.len() {
            Ok(Self {
                n: n,
                init_state: is,
                goal_state: gs,
                state_set: tmp,
                heuristic: heuristic, // ops: Vec::new()
            })
        } else {
            Err(())
        }
    }
    // fn init_ops(&mut self) {
    //     self.ops.append(self.up);
    // }
    // fn up(&self, node: &Node) -> Vec<&Node> {
    //     // let v =
    //     todo!()
    // }
}

impl Problem for Npuzle {
    fn get_initial_node(&self) -> Node {
        0.into()
    }

    fn get_goal_node(&self) -> Node {
        1.into()
    }

    fn expand(&mut self, node: &Node) -> Vec<Node> {
        // up
        let s = self.state_set.get(node.id as usize).unwrap().clone();
        let mut res = Vec::new();
        let pos: usize = s.iter().position(|n| n == &0).unwrap();
        if pos >= self.n {
            let mut n_state = s.clone();
            n_state.swap(pos, pos - self.n);
            let exist = self.state_set.iter().position(|n| n == &n_state);
            if let Some(i) = exist {
                res.push(i.into());
            } else {
                res.push(self.state_set.len().into());
                self.state_set.push(n_state);
            }
        }
        if pos % self.n != 0 {
            let mut n_state = s.clone();
            n_state.swap(pos, pos - 1);
            let exist = self.state_set.iter().position(|n| n == &n_state);
            if let Some(i) = exist {
                res.push(i.into());
            } else {
                res.push(self.state_set.len().into());
                self.state_set.push(n_state);
            }
        }
        if pos + self.n < self.n * self.n {
            let mut n_state = s.clone();
            n_state.swap(pos, pos + self.n);
            let exist = self.state_set.iter().position(|n| n == &n_state);
            if let Some(i) = exist {
                res.push(i.into());
            } else {
                res.push(self.state_set.len().into());
                self.state_set.push(n_state);
            }
        }
        if pos % self.n != self.n - 1 {
            let mut n_state = s.clone();
            n_state.swap(pos, pos + 1);
            let exist = self.state_set.iter().position(|n| n == &n_state);
            if let Some(i) = exist {
                res.push(i.into());
            } else {
                res.push(self.state_set.len().into());
                self.state_set.push(n_state);
            }
        }
        res
    }

    fn get_value_node(&self, node: &Node) -> i32 {
        0
    }

    fn get_heuristic_node(&self, node: &Node) -> i32 {
        let state = self.state_set.get(node.id).unwrap();
        let h = self.heuristic;
        h(state)
    }
}

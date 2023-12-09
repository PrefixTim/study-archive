use bitvec::prelude::*;
use itertools::Itertools;
use std::{
    collections::{BTreeSet, HashMap},
    usize,
};

#[derive(Debug)]
enum NodeVal {
    Leaf(String),
    Node(usize, usize),
}

#[derive(Debug)]
struct TreeNode(usize, NodeVal, BitVec<u16, Msb0>);

#[derive(Debug)]
pub struct Tree {
    head: usize,
    arena: Vec<TreeNode>,
    table: HashMap<String, BitVec<u16, Msb0>>,
    rev_table: HashMap<BitVec<u16, Msb0>, String>,
}

impl Tree {
    pub fn new(leafs: HashMap<&str, usize>) -> Self {
        let mut arena: Vec<TreeNode> = leafs
            .into_iter()
            .map(|(word, freq)| TreeNode(freq, NodeVal::Leaf(word.to_owned()), BitVec::new()))
            .collect_vec();
        let mut heads: BTreeSet<(usize, usize)> =
            arena.iter().enumerate().map(|(i, el)| (el.0, i)).collect();

        while heads.len() > 1 {
            let (f1, i1) = heads.pop_first().unwrap();
            let (f2, i2) = heads.pop_first().unwrap();
            heads.insert((f1 + f2, arena.len()));
            arena.push(TreeNode(f1 + f2, NodeVal::Node(i1, i2), BitVec::new()));
        }
        let head = heads.first().unwrap().1;
        let mut table = HashMap::new();
        let mut rev_table = HashMap::new();

        let mut to_visit = vec![head];
        while !to_visit.is_empty() {
            let node = arena.get(to_visit.pop().unwrap()).unwrap();
            let v = node.2.clone();
            match &node.1 {
                NodeVal::Leaf(word) => {
                    table.insert(word.clone(), v.clone());
                    let tmp = rev_table.insert(v.clone(), word.clone());
                    if let Some(t) = tmp {
                        panic!("{}", t);
                    }
                }
                NodeVal::Node(l, r) => {
                    let (l, r) = (*l, *r);
                    let mut lv = v.clone();
                    let mut rv = v.clone();
                    lv.push(false);
                    rv.push(true);
                    arena.get_mut(l).unwrap().2.append(&mut lv);
                    arena.get_mut(r).unwrap().2.append(&mut rv);
                    to_visit.push(l);
                    to_visit.push(r);
                }
            }
        }
        // println!("{}", (std::mem::size_of_val(&table) + std::mem::size_of_val(&rev_table) + std::mem::size_of_val(&arena)));
        Tree {
            head,
            arena,
            table,
            rev_table,
        }
    }

    pub fn encode(&self, val: &str) -> Option<BitVec<u16, Msb0>> {
        self.table.get(val).cloned()
    }

    pub fn decode(&self, val: &BitVec<u16, Msb0>) -> Option<&String> {
        self.rev_table.get(val)
    }
}

#[cfg(test)]
mod tests {
    use std::vec;

    use super::*;

    #[test]
    fn create_tree_test() {
        let leafs: Vec<(&str, usize)> = vec![
            ("A", 10),
            ("E", 15),
            ("I", 12),
            ("S", 3),
            ("T", 4),
            ("P", 13),
            ("\n", 1),
        ];
        let tree = Tree::new(leafs.into_iter().collect());
        println!("{:?}", tree);
        let bts = BitVec::<u16, Msb0>::repeat(true, 3);
        let ltr = "A".to_owned();
        assert_eq!(tree.decode(&bts), Some(&ltr));
        assert_eq!(tree.encode("A").unwrap(), bts);
    }
}

pub mod freq;
pub mod tree;

use crate::text::{get_text_combined, load_data, parse_text, store_data};
use bitvec::{order::Msb0, vec::BitVec};
use itertools::Itertools;

use self::{freq::freq_count, tree::Tree};

pub struct Huffman {
    tree: Tree,
}

pub trait Compressor {
     fn new(data: &str, limit: usize) -> Huffman;

     fn encode(&self, data: &str) -> BitVec<u16, Msb0>;

     fn decode(&self, data: BitVec<u16, Msb0>) -> String;

    fn create_table(&mut self);
}

impl Compressor for Huffman {
     fn new(data: &str, limit: usize) -> Huffman {
        let tree = Tree::new(
            freq_count(parse_text(data), limit)
                .into_iter()
                .map(|(w, f)| (w, f))
                .collect_vec(),
        );
        Huffman { tree }
    }

     fn encode(&self, data: &str) -> BitVec<u16, Msb0> {
        let mut enc: BitVec<u16, Msb0> = BitVec::new();
        for el in parse_text(data) {
            if let Some(mut code) = self.tree.encode(el) {
                enc.append(&mut code);
            } else {
                el.split_inclusive(|_| true)
                    .for_each(|c| enc.append(&mut self.tree.encode(c).unwrap()));
            }
        }
        while enc.len() % 8 != 0 {
            enc.push(false);
        }
        enc
    }

     fn decode(&self, mut data: BitVec<u16, Msb0>) -> String {
        let mut data: BitVec<u16, Msb0> = data.into_iter().rev().collect();
        let mut tmp: BitVec<u16, Msb0> = BitVec::new();
        let mut res = String::with_capacity(data.len()/2);
        while !data.is_empty() {
            tmp.push(data.pop().unwrap());
            if let Some(s) = self.tree.decode(&tmp) {
                res += s;
                tmp.clear();
            }
        }
        res
    }

    fn create_table(&mut self) {
        let text = get_text_combined();
    }
}

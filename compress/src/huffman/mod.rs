pub mod freq;
pub mod tree;

use std::io::{Read, Write};

use crate::text::{get_text_combined, load_data, parse_text, store_data};
use bitvec::{order::Msb0, vec::BitVec};
use itertools::Itertools;

use self::{freq::freq_count, tree::Tree};

pub struct Huffman {
    tree: Tree,
}

impl Huffman {
    pub fn new(data: &str, limit: usize) -> Huffman {
        let tree = Tree::new(freq_count(parse_text(data), limit));
        Huffman { tree }
    }

    fn create_table(&mut self) {
        let text = get_text_combined();
    }
}

pub trait Compressor {
    type Enc: Read + Write;
    fn new_buf() -> Self::Enc;
    fn encode(&self, data: &str) -> Self::Enc;
    fn decode(&self, data: Self::Enc) -> String;
}

impl Compressor for Huffman {
    type Enc = BitVec<u16, Msb0>;
    
    fn new_buf() -> Self::Enc {
        BitVec::new()
    }

    fn encode(&self, data: &str) -> Self::Enc {
        let mut enc: Self::Enc = Self::new_buf();
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

    fn decode(&self, data: Self::Enc) -> String {
        let mut data: Self::Enc = data.into_iter().rev().collect();
        let mut tmp = Self::new_buf();
        let mut res = String::with_capacity(data.len() / 2);
        while !data.is_empty() {
            tmp.push(data.pop().unwrap());
            if let Some(s) = self.tree.decode(&tmp) {
                res += s;
                tmp.clear();
            }
        }
        res
    }
}

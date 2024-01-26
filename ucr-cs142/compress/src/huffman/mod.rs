pub mod freq;
pub mod tree;

use std::io::{Read, Write};

use bitvec::{order::Msb0, vec::BitVec};

use self::{freq::freq_count, tree::Tree};

pub struct Huffman {
    tree: Tree,
}

impl Huffman {
    pub fn new(data: &str, limit: usize) -> Huffman {
        let mut parsed = parse_text(data);
        parsed.append(&mut parse_text("A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9 -"));
        let tree = Tree::new(freq_count(&parsed, limit));
        Huffman { tree }
    }

    // fn create_table(&mut self) {
    //     let text = get_text_combined();
    // }
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

        for el in parse_text(data).into_iter() {
            match self.tree.encode(el) {
                Some(mut code) => enc.append(&mut code),
                None => el
                    .split_inclusive(|_| true)
                    .for_each(|c| enc.append(&mut self.tree.encode(c).unwrap())),
            }
        }

        enc.push(true);
        while enc.len() % 8 != 0 {
            enc.push(false);
        }

        enc
    }

    fn decode(&self, mut data: Self::Enc) -> String {
        let mut tmp = Self::new_buf();
        let mut res = String::with_capacity(data.len() / 2);

        while !data.is_empty() && !data.pop().unwrap() {}

        for el in data.into_iter() {
            tmp.push(el);
            if let Some(s) = self.tree.decode(&tmp) {
                res += s;
                tmp.clear();
            }
        }

        res
    }
}

fn parse_text<'a>(data: &'a str) -> Vec<&'a str> {
    let itr = data.match_indices(|val: char| val.is_seperator());
    let mut res = Vec::new();
    let mut last = 0;
    for (i, m) in itr {
        //stackoverflow =)
        if last != i {
            res.push(&data[last..i]);
        }
        res.push(m);
        last = i + m.len();
    }
    if last != data.len() {
        res.push(&data[last..])
    }
    res
    // data.split_whitespace().collect_vec()
}

trait SeperatorTest {
    fn is_seperator(&self) -> bool;
}

impl SeperatorTest for char {
    fn is_seperator(&self) -> bool {
        self.is_whitespace() || self.is_ascii_punctuation()
        // | matches!(*self, '!'..='&')
        // | matches!(*self, '!'..='/')
        // | matches!(*self, ':'..='@')
        // | matches!(*self, '['..='`')
        // | matches!(*self, '{'..='~')
    }
}

#[cfg(test)]
mod test {
    extern crate test;

    use super::*;
    use crate::text::{get_text_combined, get_texts, load_data, store_data, store_txt};
    use rand::{distributions::Alphanumeric, Rng};
    use test::Bencher;
    #[test]
    fn test_huffman() {
        let texts = get_texts();
        let hf = Huffman::new(&get_text_combined(), 1);
        for (i, text) in texts.iter().enumerate() {
            assert!(encdec(&hf, text, i));
            (0..5)
                .for_each(|j: usize| assert!(encdec_rand(&hf, text, texts.len() + i * 5 + j + 6)));
        }
    }

    fn encdec<T: Compressor>(cmpr: &T, text: &str, indx: usize) -> bool {
        let text_enc = cmpr.encode(text);
        store_data(&format!("data/cmpr/test/test_{indx}.cmpr"), text_enc).unwrap();
        let mut data = T::new_buf();
        load_data(&format!("data/cmpr/test/test_{indx}.cmpr"), &mut data).unwrap();
        let text_dec = cmpr.decode(data);
        store_txt(&format!("data/cmpr/test/test_{indx}.txt"), &text_dec).unwrap();
        text == text_dec
    }

    fn encdec_rand<T: Compressor>(cmpr: &T, text: &str, indx: usize) -> bool {
        let len = 1000;
        let rn = rand::thread_rng().gen_range(len..(text.char_indices().count())) - len;
        let mut indices = text.char_indices().skip(rn);
        let beg = indices.next().unwrap().0;
        let end = indices.skip(len).next().unwrap().0;
        encdec(cmpr, &text[beg..end], indx)
    }

    #[bench]
    fn bench_huffmant(b: &mut Bencher) {
        let text: String = rand::thread_rng()
            .sample_iter(&Alphanumeric)
            .take(10000)
            .map(char::from)
            .collect();
        let hf = Huffman::new(&get_text_combined(), 1);
        b.iter(|| encdec_rand(&hf, &text, 0xffff));
    }
}

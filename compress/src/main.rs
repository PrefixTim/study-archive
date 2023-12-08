use std::fs::File;
use std::io::prelude::*;

use bitvec::{vec::BitVec, order::Msb0};
use compress::{huffman::{Huffman, Compressor}, text::{get_text_combined, store_txt, self, store_data, load_data}};
use weezl::{decode::Decoder, encode::Encoder, BitOrder};

// use std::{collections::{BTreeMap, BTreeSet}, cmp::Reverse};


// use itertools::Itertools;
fn main() {
    let text = get_text_combined();
    // let text = "Hello World!\n".to_owned();
    let hf = Huffman::new(&text, 700);
    let text_enc = hf.encode(&text);
    store_data("T.cmp", text_enc).unwrap();
    // let mut data: BitVec<u16, Msb0> = BitVec::new();
    // load_data("T.cmp", &mut data).unwrap();
    // let text_dec = hf.decode(data);
    // store_txt("T.txt", &text_dec).unwrap();
    // println!("{}", text == text_dec);
    // for (i, (e1, e2)) in text.chars().into_iter().zip(text_dec.chars().into_iter()).enumerate() {
    //     if e1 != e2 {
    //         println!("{i}, {e1}, {e2}");
    //         break;
    //     }
    // }
    // let compress = Encoder::new(BitOrder::Msb, 8)
    // .encode(text.as_bytes())
    // .unwrap();
    // println!("original: {}\ncompressed: {}\nratio: {}", text.as_bytes().len(), compress.len(), text.as_bytes().len() as f64 / compress.len() as f64);



    // println!("{:?}", res.iter().filter(|(w, f)| w.len() == 1).collect_vec());
    // println!("{:?}", f64::log2(res.len() as f64));
    // println!("{:}", res.iter().fold(0, |sum, (w, f)| sum + f/w.len()));
    // println!("{:}", res.len());
    // println!("{:}", res.len()as f64 *f64::log2(res.len() as f64));
    // println!("{}", text.len());

    // let mut freq = BTreeSet::new();

    // freq_count(text.first().unwrap().split_whitespace().collect_vec(), None)
    //     .iter()
    //     .for_each(|(word, f)| { freq.insert((Reverse(*f), word.to_string())); });
    // println!("{:?}", freq.iter().map(|(f, word)| f.0).counts().iter().sorted().collect_vec());
    // println!("{:?}", freq.iter().filter(|(f, word)| f.0 == 1).map(|(f, word)| word.len()).counts().iter().sorted().collect_vec());
}

use weezl::{encode::Encoder, decode::Decoder, BitOrder};

// use std::{collections::{BTreeMap, BTreeSet}, cmp::Reverse};

use itertools::Itertools;

use compress::{huffman::{tree::create_tree, freq::freq_count}, text::{get_text_combined, parse_text}};
// use itertools::Itertools;
fn main() {
    let text = get_text_combined();
    // let compress = Encoder::new(BitOrder::Msb, 8)
    // .encode(text.as_bytes())
    // .unwrap();
    // println!("original: {}\ncompressed: {}\nratio: {}", text.as_bytes().len(), compress.len(), text.as_bytes().len() as f64 / compress.len() as f64);
    let res = freq_count(parse_text(&text), 1000);
    println!("{:?}", res.iter().filter(|(w, f)| w.len() == 1).collect_vec());
    println!("{:?}", f64::log2(res.len() as f64));
    println!("{:}", res.iter().fold(0, |sum, (w, f)| sum + f/w.len()));
    println!("{:}", res.len());
    println!("{:}", res.len()as f64 *f64::log2(res.len() as f64));
    println!("{}", text.len());

    // let mut freq = BTreeSet::new();

    // freq_count(text.first().unwrap().split_whitespace().collect_vec(), None)
    //     .iter()
    //     .for_each(|(word, f)| { freq.insert((Reverse(*f), word.to_string())); });
    // println!("{:?}", freq.iter().map(|(f, word)| f.0).counts().iter().sorted().collect_vec());
    // println!("{:?}", freq.iter().filter(|(f, word)| f.0 == 1).map(|(f, word)| word.len()).counts().iter().sorted().collect_vec());

}

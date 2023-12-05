// use weezl::{encode::Encoder, decode::Decoder, BitOrder};

use std::{collections::{BTreeMap, BTreeSet}, cmp::Reverse};

use compress::{huffman::freq_count, text::get_texts};
use itertools::Itertools;
fn main() {
    let text = get_texts();
    println!("{}", text.len());
    let mut freq = BTreeSet::new();

    freq_count(text.first().unwrap())
        .iter()
        .for_each(|(word, f)| { freq.insert((Reverse(*f), word.to_string())); });
    println!("{:?}", freq.iter().map(|(f, word)| f.0).counts().iter().sorted().collect_vec());
    println!("{:?}", freq.iter().filter(|(f, word)| f.0 == 1).map(|(f, word)| word.len()).counts().iter().sorted().collect_vec());

}

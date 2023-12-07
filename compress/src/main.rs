// use weezl::{encode::Encoder, decode::Decoder, BitOrder};

// use std::{collections::{BTreeMap, BTreeSet}, cmp::Reverse};

use itertools::Itertools;

// use compress::{huffman::freq_count, text::get_texts};
// use itertools::Itertools;
fn main() {
    let data = "One Fo%^r #tree\tFo%^r\n5 \n\n\n#tree #tree           Fo%^r         \t\n     \t            Fo%^r";
    let splt = data.split_inclusive(|val: char| val.is_whitespace()).collect_vec();
    println!("{:?}", splt);
    let splt = data.split_terminator(|val: char| val.is_whitespace()).collect_vec();
    println!("{:?}", splt);
    // let text = get_texts();
    // println!("{}", text.len());
    // let mut freq = BTreeSet::new();

    // freq_count(text.first().unwrap().split_whitespace().collect_vec(), None)
    //     .iter()
    //     .for_each(|(word, f)| { freq.insert((Reverse(*f), word.to_string())); });
    // println!("{:?}", freq.iter().map(|(f, word)| f.0).counts().iter().sorted().collect_vec());
    // println!("{:?}", freq.iter().filter(|(f, word)| f.0 == 1).map(|(f, word)| word.len()).counts().iter().sorted().collect_vec());

}

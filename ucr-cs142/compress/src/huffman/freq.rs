use std::collections::HashMap;

use itertools::Itertools;

/// counts occurance of words/symbols
/// returns tuple wher
/// .0 is the cases thate occure more than min
/// .1 is occurance of sybles of combination of words/symbold that occure less than min
pub fn freq_count<'a>(data: &'a Vec<&'a str>, min: usize) -> HashMap<&'a str, usize> {
    let mut counts: HashMap<&str, usize> = HashMap::new();
    data.iter()
        .for_each(|item| *counts.entry(item).or_default() += 1);

    let (mut common, rare): (HashMap<&str, usize>, HashMap<&str, usize>) = counts
        .into_iter()
        // .map(|(&w, f)| (w, f))
        // .map(|(word, f)| (word, f * word.len()))
        .partition(|(_, f)| *f >= min);

    // println!("{}", rare.len());
    rare.into_iter()
        .map(|(word, f)| (word.split_inclusive(|_| true).counts(), f))
        .fold(HashMap::new(), |mut res, (count, freq)| {
            count.into_iter().for_each(|(c, f)| {
                res.insert(c, f * freq + res.get(&c).unwrap_or(&0));
            });
            res
        })
        .into_iter()
        .for_each(|(k, v)| {
            common.insert(&k, v);
        });
    common
}

#[cfg(test)]
mod test {
    extern crate test;
    use crate::text::get_texts;

    use super::super::parse_text;

    use super::*;
    use test::Bencher;

    #[test]
    fn test_freq_count() {
        let data = "One Four #tree\tFour\n5 \n\n\n#tree  tree     twO      Four         \t\n     \t      twO      Four";
        let mut res = HashMap::new();
        res.insert("tree", 3);
        res.insert("Four", 4);
        res.insert("\t", 3);
        res.insert("\n", 5);
        res.insert(" ", 42);
        res.insert("t", 2);
        res.insert("w", 2);
        res.insert("e", 1);
        res.insert("n", 1);
        res.insert("O", 3);
        res.insert("5", 1);
        res.insert("#", 2);
        assert_eq!(freq_count(&parse_text(data), 3), res);
    }
    
    #[bench]
    fn bench_freq_count(b: &mut Bencher) {
        let def_str = "test ".repeat(1000);
        let vec = &get_texts();
        let data: &String = vec.first().unwrap_or(&def_str);
        let pd = parse_text(data);
        b.iter(|| freq_count(&pd, 1))
    }
}

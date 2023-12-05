use std::collections::{HashMap, BTreeMap};

use itertools::Itertools;

// use std::collections::
//My Tree is neeed with arenas


struct Huffman<'a> {
    table: BTreeMap<usize, &'a str>,
    data: String
}

impl<'a> Huffman<'a> {
    fn createTable(&mut self) {
        let freq = 
            freq_count(&self.data).into_iter()
            .sorted_by(|(_, lf), (_, rf)| usize::cmp(lf, rf));

        for (word, f) in freq {
            // self.table.insert(key, value);
        }
    }
}

pub fn freq_count(data: &str) -> HashMap<&str, usize> {
    data.split_whitespace().counts()
}

pub fn freq_count_limited(data: &str, min: usize) -> HashMap<&str, usize> {
    let mut freq = freq_count(data);
    freq.iter()
        .filter_map(|(&word, f)| if min.ge(f) { None } else { Some(word) })
        .collect_vec() // can't modify in the same iter
        .iter()
        .for_each(|k| {
            freq.remove(k);
        });
    freq
}



#[cfg(test)]
mod test {
    extern crate test;
    use super::*;
    use test::Bencher;

    #[test]
    fn test_freq_count() {
        let data = "One Fo%^r #tree\tFo%^r\n5 \n\n\n#tree #tree           Fo%^r         \t\n     \t            Fo%^r";
        let mut res = HashMap::new();
        res.insert("One", 1);
        res.insert("#tree", 3);
        res.insert("Fo%^r", 4);
        res.insert("5", 1);
        assert_eq!(freq_count(data), res);
    }

    #[bench]
    fn bench_freq_count(b: &mut Bencher) {
        let def_str = "test ".repeat(1000);
        let vec = &crate::text::get_texts();
        let data: &String = vec.first().unwrap_or(&def_str);
        b.iter(|| freq_count(data))
    }
}

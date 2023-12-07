use std::collections::{BTreeMap, HashMap};

use itertools::Itertools;

// use std::collections::
//My Tree is neeed with arenas

struct Huffman<'a> {
    table: BTreeMap<usize, &'a str>,
    data: String,
}

impl<'a> Huffman<'a> {
    fn create_table(&mut self) {
        // let tree = freq::freq_count(&self.data, 1)
        //     .into_iter()
        //     .sorted_by(|(_, lf), (_, rf)| usize::cmp(lf, rf));

        // for (word, f) in freq {
        //     // self.table.insert(key, value);
        // }
    }
}

mod freq {
    use std::collections::HashMap;

    use itertools::Itertools;

    /// counts occurance of words/symbols
    /// returns tuple wher
    /// .0 is the cases thate occure more than min
    /// .1 is occurance of sybles of combination of words/symbold that occure less than min
    pub fn freq_count(data: &str, min: usize) -> (HashMap<&str, usize>, HashMap<char, usize>) {
        let (common, rare): (HashMap<&str, usize>, HashMap<&str, usize>) = parse_text(data)
            .into_iter()
            .counts()
            .into_iter()
            .partition(|(_, f)| *f > min);
        let rare = rare
            .into_iter()
            .map(|(word, f)| word.repeat(f).chars())
            .flatten()
            .counts();
        (common, rare)
    }

    fn parse_text<'a>(data: &'a str) -> Vec<&'a str> {
        let mut itr = data.match_indices(|val: char| val.is_seperator());
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
        use test::Bencher;

        #[test]
        fn test_freq_count() {
            let data = "One Four #tree\tFour\n5 \n\n\n#tree  tree     twO      Four         \t\n     \t      twO      Four";
            let mut resl = HashMap::new();
            resl.insert("tree", 3);
            resl.insert("Four", 4);
            resl.insert("\t", 3);
            resl.insert("\n", 5);
            resl.insert(" ", 42);
            let mut resr = HashMap::new();
            resr.insert('t', 2);
            resr.insert('w', 2);
            resr.insert('e', 1);
            resr.insert('n', 1);
            resr.insert('O', 3);
            resr.insert('5', 1);
            resr.insert('#', 2);
            assert_eq!(freq_count(data, 2), (resl, resr));
        }
        #[bench]
        fn bench_freq_count(b: &mut Bencher) {
            let def_str = "test ".repeat(1000);
            let vec = &crate::text::get_texts();
            let data: &String = vec.first().unwrap_or(&def_str);
            b.iter(|| freq_count(data, 1))
        }
    }
}

mod tree {
    use itertools::Itertools;
    use std::{collections::BTreeSet, usize};

    #[derive(Debug)]
    enum NodeVal {
        Leaf(String),
        Node(usize, usize),
    }

    #[derive(Debug)]
    struct TreeNode(usize, NodeVal);

    pub fn create_tree(leafs: Vec<(&str, usize)>) -> (usize, Vec<TreeNode>) {
        let mut arena: Vec<TreeNode> = leafs
            .into_iter()
            .map(|(word, freq)| TreeNode(freq, NodeVal::Leaf(word.to_owned())))
            .collect_vec();
        let mut heads: BTreeSet<(usize, usize)> =
            arena.iter().enumerate().map(|(i, el)| (el.0, i)).collect();

        while heads.len() > 1 {
            let (f1, i1) = heads.pop_first().unwrap();
            let (f2, i2) = heads.pop_first().unwrap();
            heads.insert((f1 + f2, arena.len()));
            arena.push(TreeNode(f1 + f2, NodeVal::Node(i1, i2)));
        }
        (heads.first().unwrap().1, arena)
    }

    #[cfg(test)]
    mod tests {
        use std::vec;

        use super::*;

        #[test]
        fn create_tree_test() {
            let leafs = vec![
                ("A", 10),
                ("E", 15),
                ("I", 12),
                ("S", 3),
                ("T", 4),
                ("P", 13),
                ("\n", 1),
            ];
            let tree_res = create_tree(leafs);
            println!("{:?}", tree_res);
        }
    }
}

use std::collections::{BTreeMap, HashMap};

use itertools::Itertools;

// use std::collections::
//My Tree is neeed with arenas

struct Huffman<'a> {
    table: BTreeMap<usize, &'a str>,
    data: String,
}

impl<'a> Huffman<'a> {
    fn createTable(&mut self) {
        let freq = freq_count(&self.data)
            .into_iter()
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

mod tree {
    use std::{usize, collections::BTreeSet};
    use itertools::Itertools;
    
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
        let mut heads : BTreeSet<(usize, usize)> = arena.iter().enumerate().map(|(i, el)| (el.0, i)).collect();
        
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
            let leafs = vec![("A", 10), ("E", 15), ("I", 12), ("S", 3), ("T", 4), ("P", 13), ("\n", 1)];
            let tree_res = create_tree(leafs);
            println!("{:?}", tree_res);
        }
    }
}

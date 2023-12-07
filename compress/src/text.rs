use std::{
    fs::{self, File},
    os,
    path::{self, PathBuf},
};

use itertools::Itertools;

pub fn get_texts_paths() -> Vec<PathBuf> {
    const TEXT_PATH: &str = "data/text";
    fs::read_dir(TEXT_PATH)
        .expect(&format!("Could not read the dir at path: {}", TEXT_PATH))
        .flatten()
        .map(|e| e.path())
        .collect_vec()
}

pub fn get_texts() -> Vec<String> {
    get_texts_paths().iter().map(|path| fs::read_to_string(path)).flatten().collect_vec()
}

pub fn get_text_combined() -> String {
    get_texts().into_iter().collect()
}

pub fn parse_text<'a>(data: &'a str) -> Vec<&'a str> {
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
// #[cfg(test)]
// mod tests {
//     use super::*;

//     #[test]
//     fn t() {
//         assert!(true)
//     }
// }

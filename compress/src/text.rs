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
// #[cfg(test)]
// mod tests {
//     use super::*;

//     #[test]
//     fn t() {
//         assert!(true)
//     }
// }

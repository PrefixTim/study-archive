use std::{
    fs::{self, File},
    io::{Error, Read, Write},
    path::PathBuf,
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
    get_texts_paths()
        .iter()
        .map(|path| fs::read_to_string(path))
        .flatten()
        .collect_vec()
}

pub fn get_text_combined() -> String {
    get_texts().into_iter().collect()
}

pub fn get_text(path: &str) -> String {
    fs::read_to_string(path).unwrap()
}

pub fn store_txt(path: &str, text: &str) -> Result<(), Error> {
    File::create(path)?.write_all(text.as_bytes())?;
    Ok(())
}

pub fn store_data<R: Read>(path: &str, mut data: R) -> Result<(), Error> {
    std::io::copy(&mut data, &mut File::create(path)?)?;
    Ok(())
}

pub fn load_data<W: Write>(path: &str, data: &mut W) -> Result<(), Error> {
    std::io::copy(&mut File::open(path)?, data)?;
    Ok(())
}

// #[cfg(test)]
// mod tests {
//     use super::*;

//     #[test]
//     fn t() {
//         assert!(true)
//     }
// }

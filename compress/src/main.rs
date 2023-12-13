use std::{env, str::from_utf8};

use compress::{
    huffman::{Compressor, Huffman},
    text::{get_text, get_text_combined, get_texts, load_data, store_data, store_txt},
};
use inflate::inflate_bytes;
use zopfli::{Format, Options};

fn main() {
    let args: Vec<String> = env::args().collect();
    if let Some(path) = args.get(1) {
        let text = get_text(path);
        // let hf = Huffman::new(&text, 1);
        let hf = Huffman::new(&get_text_combined(), 1);
        encdec(&hf, &text, "TEXT");
    } else {
        compress_all();
        zopfli_compress_all();
    }
}

fn compress_all() {
    let text = get_texts();
    let hf = Huffman::new(&get_text_combined(), 1);
    encdec(&hf, &text[0], "data/cmpr/emma");
    encdec(&hf, &text[1], "data/cmpr/paj");
    // encdec_rand(&hf, &text[1]);
}

fn encdec<T: Compressor>(cmpr: &T, text: &str, path: &str) {
    let text_enc = cmpr.encode(text);
    store_data(&format!("{path}.cmpr"), text_enc).unwrap();
    let mut data = T::new_buf();
    load_data(&format!("{path}.cmpr"), &mut data).unwrap();
    let text_dec = cmpr.decode(data);
    store_txt(&format!("{path}.txt"), &text_dec).unwrap();
    assert_eq!(text, text_dec);
}

// fn encdec_rand<T: Compressor>(cmpr: &T, text: &str) {
//     let indx = text.char_indices().map(|(i, _)| i);
//     let tlen = indx.clone().last().unwrap();
//     let len = 1000;
//     let rn = rand::thread_rng().gen_range(0..(tlen - len));
//     let beg = indx.clone().nth(rn).unwrap();
//     let end = indx.clone().nth(rn + len).unwrap();
//     encdec(cmpr, &text[beg..end], "data/cmpr/rand");
// }

fn zopfli_compress_all() {
    let texts = get_texts();
    zopfli_compress(&texts[0], "data/cmpr/Zopfli")
}

fn zopfli_compress(text: &str, path: &str) {
    zopfli::compress(
        Options::default(),
        Format::Deflate,
        text.as_bytes(),
        std::fs::File::create(&format!("{path}.cmpr")).unwrap(),
    )
    .unwrap();
    let mut enc = Vec::new();
    load_data(&format!("{path}.cmpr"), &mut enc).unwrap();
    let res = inflate_bytes(&enc).unwrap();
    assert_eq!(text.as_bytes(), res);
    store_txt(
        &format!("{path}.txt"),
        &from_utf8(&res).unwrap().to_string(),
    )
    .unwrap();
}

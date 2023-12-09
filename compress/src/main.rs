use compress::{
    huffman::{Compressor, Huffman},
    text::{get_text_combined, load_data, store_data, store_txt, get_texts},
};


fn main() {
    let text = get_texts();
    let hf = Huffman::new(&get_text_combined(), 1);
    encdec(&hf, &text[0], "data/cmpr/emma");
    encdec(&hf, &text[1], "data/cmpr/paj");
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
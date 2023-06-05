use ndarray::{prelude::*, OwnedRepr};
use std::{env, fs};

use prj2_lib::*;

fn main() {
    let args: Vec<String> = env::args().collect();

    // let file_path = &args[1];
    let file_path = "./small-test-dataset.txt";
    
    println!("Welcome to Timofey Malko(862311452) Feature Selection Algorithm.");
    println!("Type the number of the algorithm you want to run.\n\n\tForward Selection\n\tBackward Elimination.");
    
    let mut buf = String::new();
    std::io::stdin().read_line(&mut buf).unwrap();
    let func = match buf.trim() {
        "1" => forward_sel,
        "2" => backward_elim,
        _ => {
            println!("Invalid input.");
            return;
        }
    };

    let data = read_file(file_path);
    let eval = Evaluator {};
    let classifier = NNClassifier::new(&data);
    func(eval, &classifier, &data);
}

fn read_file(file_path: &str) -> (Arr1f32, Arr2f32) {
    let content: String =
        fs::read_to_string(file_path).expect("Should have been able to read the file");
    let ninstances: usize = content.lines().count();
    let nfeatures: usize = content
        .lines()
        .nth(0)
        .unwrap()
        .trim()
        .split_whitespace()
        .count() - 1;
    
    let mut max: Vec<f64> = vec![0f64; nfeatures];
    let mut min: Vec<f64> = vec![0f64; nfeatures];
    
    let mut lables: Vec<f64> = vec![0f64; ninstances];
    let fetures: Vec<f64> = content
        .lines()
        .zip(lables.iter_mut())
        .map(|(line, label)| {
            let mut arr: Vec<f64> = line
                .trim()
                .split_whitespace()
                .map(|n: &str| n.parse::<f64>().expect("file parse faile"))
                .collect();
            *label = arr[0];
            arr.remove(0);
            max.iter_mut()
                .zip(arr.iter())
                .for_each(|(m, e)| *m = m.max(*e));
            min.iter_mut()
                .zip(arr.iter())
                .for_each(|(m, e)| *m = m.min(*e));
            arr
        })
        .flatten()
        .collect();

    let min=  Array::from_vec(min);
    let mm = Array::from_vec(max) - min.clone();

    let labels = Array1::from_vec(lables);
    let features = (Array::from_shape_vec((ninstances, nfeatures), fetures).unwrap() - min) / mm;

    (labels, features)
}

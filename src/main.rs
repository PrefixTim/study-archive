use std::{env, fs};

use prj2_lib::{backward_elim, forward_sel, DataInstance, Evaluator, InstanceArena, NNClassifier};

fn main() {
    let _args: Vec<String> = env::args().collect();

    // let file_path = &args[1];
    let file_path = "./small-test-dataset.txt";
    let mut buf = String::new();

    println!("Welcome to Timofey Malko(862311452) Feature Selection Algorithm.");
    // println!("\nPlease enter total number of features:");
    // std::io::stdin().read_line(&mut buf).unwrap();
    // let size: usize = buf.trim().parse().expect("Invalid input.");

    println!("Type the number of the algorithm you want to run.\n\n\tForward Selection\n\tBackward Elimination\n\tBertie’s Special Algorithm.");
    buf.clear();
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

fn read_file(file_path: &str) -> InstanceArena {
    let content = fs::read_to_string(file_path).expect("Should have been able to read the file");
    let mut data: Vec<DataInstance> = content
        .lines()
        .map(|l| {
            let mut splt: Vec<f64> = l
                .trim()
                .split_whitespace()
                .map(|n| n.parse::<f64>().expect("file parse faile"))
                .collect();
            let label = splt[0];
            splt.remove(0);
            DataInstance {
                label,
                features: splt,
            }
        })
        .collect();

    (0..data[0].features.len()).for_each(|i| {
        let min = data
            .iter()
            .min_by(|a, b| a.features[i].total_cmp(&b.features[i]))
            .unwrap()
            .features[i];
        let max = data
            .iter()
            .max_by(|a, b| a.features[i].total_cmp(&b.features[i]))
            .unwrap()
            .features[i];
        let m = max - min;
        data.iter_mut()
            .for_each(|e| e.features[i] = (e.features[i] - min) / m);
    });

    data
}

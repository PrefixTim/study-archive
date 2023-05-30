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
    let size: usize = data[0].features.len();
    let mut m = vec![0.; size];
    let mut sig = vec![0.; size];

    data.iter().for_each(|d| {
        d.features
            .iter()
            .zip(m.iter_mut())
            .for_each(|(f, m)| *m += f)
    });
    m.iter_mut().for_each(|e| *e /= size as f64);
    data.iter_mut().for_each(|d| {
        d.features
            .iter_mut()
            .zip(m.iter().zip(sig.iter_mut()))
            .for_each(|(f, (&m, sd))| {
                *f -= m;
                *sd += *f * *f
            })
    });
    sig.iter_mut().for_each(|e| *e = (*e / size as f64).sqrt());
    data.iter_mut().for_each(|d| {
        d.features
            .iter_mut()
            .zip(sig.iter())
            .for_each(|(f, &sig)| *f /= sig)
    });
    data
}

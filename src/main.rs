use clap::{arg, command, value_parser, Arg, ArgAction};
use prj2_lib::{
    backward_elim, forward_sel, DataInstance, InstanceArena, KNNClassifier, LooEvaluator,
    NNClassifier,
};
use std::{env, fs, path::PathBuf};

fn main() {
    let (file_path, kfold, knear, normalize) = parse_cla();

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

    let data = read_file(&file_path, normalize);
    let eval = LooEvaluator::new(kfold);
    if let Some(k) = knear {
        func(&eval, &mut KNNClassifier::new(&data, k), &data);
    } else {
        func(&eval, &mut NNClassifier::new(&data), &data);
    };
}

fn parse_cla() -> (PathBuf, usize, Option<usize>, bool) {
    let matches = command!()
        .arg(
            arg!(-f --file <Path> "Path to a file with data")
                .value_parser(value_parser!(std::path::PathBuf))
                .required(true),
        )
        .arg(arg!(--kfold <Kfold> "Number for k fold validator").value_parser(value_parser!(usize)))
        .arg(
            arg!(-k --knear <Knn> "Number for k nearest nighbourse classification")
                .value_parser(value_parser!(usize)),
        )
        .arg(
            Arg::new("normalize")
                .short('n')
                .long("normalize")
                .action(ArgAction::SetTrue),
        )
        .get_matches();

    let file_path = matches.get_one::<PathBuf>("file").unwrap().to_owned();
    let kfold = matches.get_one::<usize>("kfold").unwrap_or(&1);
    let knear = matches.get_one::<usize>("knear").copied();
    let normalize = matches.get_flag("normalize");

    (file_path, *kfold, knear, normalize)
}

fn read_file(file_path: &PathBuf, normalize: bool) -> InstanceArena {
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

    if normalize {
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
    }
    data
}

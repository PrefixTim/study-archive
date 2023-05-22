use prj2_lib::{backward_elim, Evaluator, forward_sel};

fn main() {
    let mut buf = String::new();

    println!("Welcome to Timofey Malko(862311452) Feature Selection Algorithm.\nPlease enter total number of features:");

    std::io::stdin().read_line(&mut buf).unwrap();
    let size: usize = buf.trim().parse().expect("Invalid input.");

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

    let eval = Evaluator {};
    func(eval, size);
}

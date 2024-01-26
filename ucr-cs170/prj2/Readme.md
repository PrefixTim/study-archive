# Project 2 Part 3

## Instalation
[Install the offical compiler](https://www.rust-lang.org/tools/install)

## Usage

Run normalized

```bash
cargo run -r -- -n -f=./CS170_Spring_2023_Small_data__53.txt
```

OR

```bash
cargo build -r 
./target/release/prj2  -f=./CS170_Spring_2023_Small_data__53.txt -n 
```

Optional flags could be added/removed

non-normalized with k-5 nn classification

```bash
cargo run -r -- --knear=4 -f=./CS170_Spring_2023_Large_data__53.txt
```

Available flags

```bash
Usage: prj2 [OPTIONS] --file <Path>

Options:
  -f, --file <Path>    Path to a file with data
      --kfold <Kfold>  Number for k fold validator
  -k, --knear <Knn>    Number for k nearest nighbourse classification
  -n, --normalize      
  -h, --help           Print help
  -V, --version        Print version
```

sasd

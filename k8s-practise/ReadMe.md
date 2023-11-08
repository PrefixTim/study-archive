## Build tools
Requires rust.
https://www.rust-lang.org/tools/install

## Run/Test
To run demo
```
cargo run
```
Exapmle in/out
```
1
1.21
121
-1
-0
0
inf
-inf
nan
-nan
1e1

input                   my_sqrt_impl            sqrt                
                   1                       1                       1
                1.21      1.0999999999999999                     1.1
                 121                      11                      11
                  -1                     NaN                     NaN
                  -0                      -0                      -0
                   0                       0                       0
                 inf                     inf                     inf
                -inf                     NaN                     NaN
                 NaN                     NaN                     NaN
                 NaN                     NaN                     NaN
                  10       3.162277660168379      3.1622776601683795
total err:      0.0000000000000006661338147750939

Process finished with exit code 0

```

To see correctness of different implementation
```
cargo test mysqrt::tests::test_correctnes -- --nocapture
```

To see the performance of different implementation
```
cargo bench
```

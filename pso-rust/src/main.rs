#![allow(non_snake_case)] // Disabling snake case warnings

use rand::rng; // Use rng() from rand 0.9.1
use rand_distr::{Distribution, Uniform};

mod point;

fn main() {
    let EPOCH = 100;
    let POINTSAMOUNT = 2000;
    let BOUND: (f64, f64) = (-10.0, 10.0);
    let ALPHA = 0.5;
    let BETA = 0.3;
    let POINTDIMENSION = 8; // Set to 2 for func1, to 4 for func2, 8 for func3
    let SAME_GRADE_EPOCHS = 10;

    let mut rng = rng(); 

    let distr = Uniform::new(BOUND.0, BOUND.1).expect("Invalid bounds: lower must be less than upper");

    let valueRandom: f64 = distr.sample(&mut rng);
    println!("Random value: {:.6}", valueRandom);
}
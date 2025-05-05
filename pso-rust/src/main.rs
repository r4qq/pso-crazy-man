#![allow(non_snake_case)] //disabling damn snake case warnings

mod point;
use point::Point;
fn main() 
{
    
    

    println!("Hello, world!");

    let x =  5;

    let mut y = 10;

    y += x;

    println!("y is {}", y);

    let s1 = String::from("Hello");
    let s2 = s1;
    println!("{}", s2);
}

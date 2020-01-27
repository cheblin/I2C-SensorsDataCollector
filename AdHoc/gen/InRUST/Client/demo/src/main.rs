
#![feature(const_transmute)]
#![feature(const_fn)]
//=======================================
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(unused_imports)]
//#![feature(untagged_unions)]
//#![feature(const_transmute)]
#![feature(box_syntax)]

use ad_hoc_gen::util_;

#[cfg(test)]
#[path = "demo.rs"]
mod demo;

#[cfg(test)]
#[path = "test.rs"]
mod test;

#[cfg(not(test))]
fn main() { println!("run") }

#[global_allocator]
static GLOBAL: jemallocator::Jemalloc = jemallocator::Jemalloc;

use rand::{seq::SliceRandom, thread_rng};
use rayon::prelude::*;
use rayon_adaptive::merge_sort_adaptive;
use rayon_try_fold::{iter_par_sort, slice_par_sort};
use std::env;
use std::time::Instant;

const NUM_RUNS: usize = 25;

fn mean_time<F: FnMut() -> std::time::Duration>(mut bench_function: F) -> std::time::Duration {
    (0..NUM_RUNS)
        .map(|_| bench_function())
        .sum::<std::time::Duration>()
        .div_f64(NUM_RUNS as f64)
}
macro_rules! bench_sort {
    ($inp: ident, $sortexp: expr) => {
        mean_time(|| {
            $inp.shuffle(&mut thread_rng());
            let start = Instant::now();
            $sortexp;
            start.elapsed()
        })
    };
}

fn main() {
    let args = env::args().collect::<Vec<_>>();
    //command line arguments should be problem_size run_sequential.
    //latter is 1 for baselines and 0 for the parallel algorithms
    assert_eq!(args.len(), 3);
    let problem_size: u32 = args[1].parse().unwrap();
    let run_sequential: u32 = args[2].parse().unwrap();
    let mut input: Vec<u32> = (0..problem_size).map(|_| rand::random()).collect();
    if run_sequential > 0 {
        {
            //Sequential stable sort
            let measured_time = bench_sort!(input, input.sort());
            println!("seq stable\t{:?}", measured_time.as_secs_f64());
        }
        {
            //Sequential unstable sort
            let measured_time = bench_sort!(input, input.sort_unstable());
            println!("seq unstable\t{:?}", measured_time.as_secs_f64());
        }
    } else {
        {
            //Rayon stable
            let measured_time = bench_sort!(input, input.par_sort());
            println!("rayon stable\t{:?}", measured_time.as_secs_f64());
        }
        {
            //Rayon unstable
            let measured_time = bench_sort!(input, input.par_sort_unstable());
            println!("rayon unstable\t{:?}", measured_time.as_secs_f64());
        }
        {
            //Rayon Adaptive stable iterator
            let measured_time = bench_sort!(input, merge_sort_adaptive(&mut input));
            println!("adaptive iter\t{:?}", measured_time.as_secs_f64());
        }
        {
            //Try fold slice manual sort
            let measured_time = bench_sort!(input, slice_par_sort(&mut input));
            println!("try_fold slice\t{:?}", measured_time.as_secs_f64());
        }
        {
            //Try fold iterator sort
            let measured_time = bench_sort!(input, iter_par_sort(&mut input));
            println!("try_fold iter\t{:?}", measured_time.as_secs_f64());
        }
    }
}

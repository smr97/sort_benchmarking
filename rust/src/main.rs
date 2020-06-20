use rand::{seq::SliceRandom, thread_rng};
use rayon::prelude::*;
use rayon_adaptive::merge_sort_adaptive;
use rayon_try_fold::{iter_par_sort, slice_par_sort};
use std::env;
use std::time::Instant;

const NUM_RUNS: usize = 100;

fn mean_time<F: FnMut() -> std::time::Duration>(mut bench_function: F) -> std::time::Duration {
    (0..NUM_RUNS)
        .map(|_| bench_function())
        .sum::<std::time::Duration>()
        .div_f64(NUM_RUNS as f64)
}
macro_rules! bench_sort {
    ($inp: ident, $sorter: ident) => {
        mean_time(|| {
            $inp.shuffle(&mut thread_rng());
            let start = Instant::now();
            $inp.$sorter();
            start.elapsed()
        })
    };
    ($tp: ident, $inp: ident, $sortexp: expr) => {
        mean_time(|| {
            $tp.install(|| {
                $inp.shuffle(&mut thread_rng());
                let start = Instant::now();
                $sortexp;
                start.elapsed()
            })
        })
    };
}

fn main() {
    let args = env::args().collect::<Vec<_>>();
    assert_eq!(args.len(), 3);
    println!("********** Rust sorts **********");
    let problem_size: u64 = args[1].parse().unwrap();
    let num_threads: usize = args[2].parse().unwrap();
    let tp = rayon::ThreadPoolBuilder::new()
        .num_threads(num_threads)
        .build()
        .expect("build pool failed");
    let mut input: Vec<u64> = (0..problem_size).map(|_| rand::random()).collect();
    {
        //Sequential stable sort
        let measured_time = bench_sort!(input, sort);
        println!("seq stable\t{:?}", measured_time);
    }
    {
        //Sequential unstable sort
        let measured_time = mean_time(|| {
            input.shuffle(&mut thread_rng());
            let start = Instant::now();
            input.sort_unstable();
            start.elapsed()
        });
        println!("seq unstable\t{:?}", measured_time);
    }
    {
        //Rayon stable
        let measured_time = bench_sort!(tp, input, input.par_sort());
        println!("rayon stable\t{:?}", measured_time);
    }
    {
        //Rayon unstable
        let measured_time = bench_sort!(tp, input, input.par_sort_unstable());
        println!("rayon unstable\t{:?}", measured_time);
    }
    {
        //Rayon Adaptive stable iterator
        let measured_time = bench_sort!(tp, input, merge_sort_adaptive(&mut input));
        println!("adaptive iter\t{:?}", measured_time);
    }
    {
        //Try fold slice manual sort
        let measured_time = bench_sort!(tp, input, slice_par_sort(&mut input));
        println!("try_fold slice\t{:?}", measured_time);
    }
    {
        //Try fold iterator sort
        let measured_time = bench_sort!(tp, input, iter_par_sort(&mut input));
        println!("try_fold iter\t{:?}", measured_time);
    }
}

use kvik::{iter_par_sort, slice_par_sort};
use rand::{seq::SliceRandom, thread_rng};
use rayon::prelude::*;
use std::env;
use std::fs::File;
use std::io::prelude::*;
use std::{path::Path, time::Instant};

const NUM_RUNS: usize = 100;

fn write_to_file<P: AsRef<Path>>(name: P, input: Vec<f64>) -> std::io::Result<()> {
    let mut buffer = File::create(name)?;
    let mean_value = input.iter().cloned().sum::<f64>() / input.iter().len() as f64;
    input
        .into_iter()
        .try_for_each(|something| writeln!(&mut buffer, "{}", something))?;
    writeln!(&mut buffer, "mean_time:{}", mean_value)?;
    Ok(())
}

fn measure_times<F: FnMut() -> std::time::Duration>(mut bench_function: F) -> Vec<f64> {
    (0..NUM_RUNS)
        .map(|_| bench_function())
        .map(|dur| dur.as_secs_f64())
        .collect()
}

macro_rules! bench_sort {
    ($inp: ident, $sortexp: expr) => {
        measure_times(|| {
            $inp.shuffle(&mut thread_rng());
            let start = Instant::now();
            $sortexp;
            start.elapsed()
        })
    };
}

fn main() -> std::io::Result<()> {
    let args = env::args().collect::<Vec<_>>();
    //command line arguments should be problem_size run_sequential.
    //latter is 1 for baselines and 0 for the parallel algorithms
    assert_eq!(args.len(), 3);
    let problem_size: u32 = args[1].parse().unwrap();
    let num_threads: u32 = args[2].parse().unwrap();
    let mut input: Vec<u32> = (0..problem_size).map(|_| rand::random()).collect();
    if num_threads == 1 {
        {
            //Sequential stable sort
            let times = bench_sort!(input, input.sort());
            write_to_file("rustsort_stable_1", times)?;
        }
    } else {
        {
            //Rayon stable
            let times = bench_sort!(input, input.par_sort());
            write_to_file(format!("rustsort_rayon_{}", num_threads), times)?;
        }
        {
            //Try fold slice manual sort
            let times = bench_sort!(input, slice_par_sort(&mut input));
            write_to_file(format!("rustsort_slice_{}", num_threads), times)?;
        }
        {
            //Try fold iterator sort
            let times = bench_sort!(input, iter_par_sort(&mut input));
            write_to_file(format!("rustsort_iter_{}", num_threads), times)?;
        }
    }
    Ok(())
}

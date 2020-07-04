import java.util.Arrays;
import java.util.Random;
import java.util.stream.*;


public class Bench {
    final static int NUM_RUNS = 100;
    public static void main(String[] args) {
        int len = 0;
        int run_sequential = 0;
        try {
            len = Integer.parseInt(args[0]);
            run_sequential = Integer.parseInt(args[1]);
        } catch (NumberFormatException nfe) {
            System.out.println("Usage: problem_size run_sequential: where run_sequential is 1+ for running only baselines, and is 0 for parallel algorithms.");
            System.exit(1);
        }

        int[] arr = new int[len];
        if (run_sequential > 0) {
            {
                long[] run_times = new long[NUM_RUNS];
                for(int r = 0; r < NUM_RUNS+20; r++){
                    Random rnd = new Random();
                    for (int i = 0; i < len; ++i) {
                        arr[i] = rnd.nextLong();
                    }

                    long start = System.currentTimeMillis();
                    Arrays.sort(arr);
                    long end = System.currentTimeMillis();
                    if (r>19) {
                        run_times[r-20] = end - start;
                    }
                }
                long sum_times = LongStream.of(run_times).sum();

                System.out.println("Arrays.sort          " + ((double)(sum_times)/(double)(NUM_RUNS)) + " ms");
            }
        } else {
            {
                long[] run_times = new long[NUM_RUNS];
                for(int r = 0; r < NUM_RUNS+20; r++){
                    Random rnd = new Random();
                    for (int i = 0; i < len; ++i) {
                        arr[i] = rnd.nextLong();
                    }

                    long start = System.currentTimeMillis();
                    Arrays.parallelSort(arr);
                    long end = System.currentTimeMillis();
                    if (r>19) {
                        run_times[r-20] = end - start;
                    }
                }
                long sum_times = LongStream.of(run_times).sum();

                System.out.println("Arrays.parallelSort  " + ((double)(sum_times)/(double)(NUM_RUNS)) + " ms");

            }
        }
    }
}

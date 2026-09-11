# v0-naive serial baseline profiling

Reference wall-clock profiling for the naive serial N-body direct-summation
kernel (`src/serial/v0-naive/nbody_direct_serial.c`), profiler enabled
(`--profiler 1`).

## Run configuration

- Initial conditions: Plummer sphere
- N = 50000
- nsteps = 50
- dt = 1e-4
- eps = 0.05
- G = 1
- mass = 1

## Per-run results

| run | force median (s) | total_step median (s) | drift | status |
|---|---|---|---|---|
| 1 | 4.324889 | 4.325213 | 3.6995e-07 | OK |
| 2 | 4.323072 | 4.323544 | 3.6995e-07 | OK |
| 3 | 4.274639 | 4.274846 | 3.6995e-07 | OK |
| 4 | 4.217922 | 4.218128 | 3.6995e-07 | OK |
| 5 | 4.215870 | 4.216047 | 3.6995e-07 | OK |

Values extracted from `run1.log`...`run5.log` (`--- per-step phases ---`
section and the `# final: ...` line); all five confirmed against the raw
per-step samples in `profile_run1.txt`...`profile_run5.txt`.

## Aggregate force-phase statistics (across the 5 run medians)

- median ≈ 4.2746 s
- mean ≈ 4.2713 s
- std ≈ 0.048 s (~1.1% relative variation)

This is the reference wall-clock baseline for the force kernel. Every future
serial optimization (rsqrt, blocking, Newton's third law, loop unrolling)
will be compared against it to compute speedup.

## Observations

- The force phase accounts for essentially all of `total_step` time in every
  run (>99.98%, i.e. ~99.99%): the first/second drift and kick phases are
  ~1e-4-1e-5 s, negligible by comparison. This confirms the O(N^2) naive
  kernel is completely force-bound, as expected.
- All 5 runs show bit-identical energy drift (3.6995e-07), as expected since
  the computation is fully deterministic given identical inputs.

## Runner

These runs were produced by `runners/run-experiment.sh`, which can be reused
unchanged for future variants (v1-rsqrt, v2-blocking, ...) by pointing it at
their binary and variant name.

# aos-vs-soa: -O2 sub-experiment

Reference wall-clock profiling for the AoS-vs-SoA serial N-body benchmark
(`src/serial/experiments/aos-vs-soa/nbody_direct_serial.c`), built with a
conservative, portable compiler configuration (no target-specific
vectorization), profiler enabled (`--profiler 1`).

## Build configuration

```
-O2 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=199309L -DNBODY_USE_DOUBLE
```

No `-march=native`, no `-O3`.

## Run configuration

- Initial conditions: Plummer sphere
- N = 50000
- nsteps = 50
- dt = 1e-4
- eps = 0.05
- G = 1
- mass = 1
- Cluster: Orfeo, GENOA partition

## Results (5 repetitions each)

Values extracted from the `total step` line of `run1.log`...`run5.log` in
`data/raw/aos-vs-soa-aos-O2/` and `data/raw/aos-vs-soa-soa-O2/`. "median"
and "mean" are the median/mean of the 5 per-run medians; "std" is the mean
of the 5 per-run std values reported by the profiler.

| Layout | total_step median (s) | mean (s) | std (s) |
|---|---|---|---|
| AoS | 8.6133 | 8.6115 | 0.0012 |
| SoA | 8.7392 | 8.7454 | 0.0004 |

**Note:** the AoS std column was cross-checked against the reference value
`~0.0013` — the value actually computed from the logs (mean of the 5
per-run std values: 1.233771e-03, 9.101536e-04, 1.207036e-03, 1.353828e-03,
1.472431e-03) is **0.0012**, not 0.0013. This is flagged rather than
silently reconciled; the median and mean columns for both layouts, and the
SoA std, all matched the supplied reference values exactly.

## Finding

Contrary to the expected outcome (SoA faster than AoS due to better cache
locality and auto-vectorization potential), SoA is here **~1.5% slower**
than AoS, consistently across all 5 repetitions of each layout. The
variance is very low for both layouts (std well under 0.15% of the
median), so this is not measurement noise.

## Hypothesis

With `-O2` and no `-march=native`, the compiler is unlikely to
auto-vectorize the force loop for either layout, since it has no specific
SIMD ISA to target. SoA's advantage over AoS comes specifically from
enabling contiguous SIMD loads across particles — without vectorization
happening at all, that advantage doesn't materialize, and the residual
difference may instead reflect SoA's less favorable access pattern in this
specific unvectorized scalar scenario (9 separate memory streams instead
of 1 contiguous stream of particle structs).

## Correctness

Energy drift is bit-identical between AoS and SoA in all 10 runs
(`max_relative_energy_drift=3.699530700e-07`, i.e. 3.6995e-07 — verified
against all 10 `profile_run*.txt` files), confirming this is purely a
performance effect, not a correctness difference. All 10 runs report
`status=OK`.

## Follow-up

This motivated re-running the experiment with `-O3 -march=native -flto`;
see `data/raw/aos-vs-soa-aos/` and `data/raw/aos-vs-soa-soa/` (without the
`-O2` suffix) for the results with those flags, used as the "official"
AoS-vs-SoA comparison in the report.

# Serial baseline reference runs

Reference logs for the naive serial N-body direct-summation kernel
(`src/serial/v0-naive/nbody_direct_serial.c`).

| file | IC model | N | nsteps | drift | tolerance | status |
|---|---|---|---|---|---|---|
| plummer-n10k-steps100-seed42.log | Plummer | 10000 | 100 | 8.0826006609817023e-08 | 0.001 | OK |
| plummer-n10k-steps1000-seed42.log | Plummer | 10000 | 1000 | 2.6955108887248195e-07 | 0.001 | OK |
| plummer-n100k-steps50-seed42.log | Plummer | 100000 | 50 | 5.2526553073927085e-07 | 0.001 | OK |
| ball-n10k-steps100-seed43.log | Maxwell ball | 10000 | 100 | 6.5599556097156777e-06 | 0.001 | OK |

`drift` is `max_relative_energy_drift` parsed from each log's final line.

## Purpose

These runs are the reference (ground truth) for energy conservation against
which all subsequent serial kernel optimizations (SoA, rsqrt, blocking,
Newton's third law, unrolling) will be compared. If a future optimized
version shows a worse drift than the corresponding run here under identical
parameters, it must be investigated before proceeding.

## Observation: ball vs. Plummer drift

`ball-n10k-steps100-seed43.log` (Maxwell ball) shows a drift ~80x larger than
`plummer-n10k-steps100-seed42.log` (Plummer) at identical N and step count.
This is plausibly because the uniform ball is not a dynamical equilibrium
configuration (unlike the Plummer sphere), so in the first few steps the
system settles, producing larger instantaneous accelerations. It still
remains ~150x below the required tolerance.

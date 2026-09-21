# Methodology / design decisions log

A running log of methodology and design decisions made in this project

## Restrict/const qualifiers and the -ffast-math dependency

I added `restrict` and `const` to the force kernel's pointer parameters
across baseline, rsqrt, and aos-vs-soa (SoA side only — not applicable to the AoS kernel's single interleaved-struct pointer), plus `static` linkage where missing.

These qualifiers carry zero numerical trade-off (they document facts
already true about the code, they don't change any computed value),
unlike rsqrt or -ffast-math which introduce deliberate approximations.
Zero-trade-off changes are treated as build-quality corrections applied
uniformly; approximations are isolated, measured experiments.

A vectorization report (-fopt-info-vec on gcc /
-Rpass=loop-vectorize on clang) shows all three force kernels still fail
to auto-vectorize even with restrict+const+-O3+-march=native. The
compiler's own diagnostic identifies the cause explicitly: it cannot
prove the floating-point accumulation (`axi += dx * s`) is safe to
reorder under strict IEEE semantics, and suggests -ffast-math as the fix.
This confirms the blocker was never pointer aliasing (which restrict
would have solved) — it's IEEE FP-reduction ordering.

-ffast-math is a necessary condition for vectorizing this reduction pattern with the current loop structure.

### Isolated measurement

Before deciding whether to adopt -ffast-math permanently, I measured its
effect in isolation: same source, same -O3 -march=native -flto build,
only -ffast-math added via a one-off compile (Makefile left unchanged),
tested against the aos-vs-soa SoA kernel. 5 repetitions each, N=50000,
50 steps, GENOA partition, compared via analysis/parser.py.

| Build                | force trimmed_mean (s) | std      |
|-----------------------|------------------------|----------|
| without -ffast-math   | 4.2129                 | 0.0316   |
| with -ffast-math       | 1.9163                 | 0.0299   |

Speedup: 2.198x ± 0.038 — close to the ~2.5x reported by the reference
project (Savorgnan) for the same flag on their kernels, confirming
-ffast-math (not restrict) is the dominant factor behind the earlier gap
between our AoS-vs-SoA speedup (~2.30x, without -ffast-math) and theirs
(~4.3x, with -ffast-math already included in their default build flags).

Energy drift impact: negligible. A single-run correctness check
(N=128, 10 steps) showed max_relative_energy_drift changing from
4.4164728144891537e-06 to 4.4164728095902277e-06 — a relative
difference on the order of 1e-9, several orders of magnitude below the
1e-3 tolerance and below the drift itself. Every intermediate step's
kinetic/potential/total energy differed only in the last 1-2
significant digits, consistent with ordinary floating-point reordering
noise, not a correctness regression.

### Decision: adopted permanently

Given the large, reproducible speedup and the negligible impact on
correctness, -ffast-math was added permanently to all three Makefiles
(baseline, rsqrt, aos-vs-soa), alongside the existing -O3 -march=native
-flto -D_POSIX_C_SOURCE=199309L flags. This reverses the earlier
deliberate choice to exclude it (made before the isolated measurement
was available).

Consequence: all previously-collected "official" Orfeo results (baseline,
rsqrt, aos-vs-soa-aos, aos-vs-soa-soa) were re-run under the final build
configuration (restrict/const/static + -O3 -march=native -flto
-ffast-math) to keep every experiment comparable on the same basis going
forward. The pre-ffast-math results were preserved as a separate,
explicitly-labelled archive (data/raw/*-no-ffastmath/) rather than
discarded, since they remain a valid, documented data point (the
counter-intuitive -O2 AoS-vs-SoA result, and the isolated flag
comparison above, both rely on having that earlier configuration
preserved for comparison).

Going forward, -ffast-math is part of the standard build for every new
serial experiment (blocking, Newton's third law, loop unrolling),
rather than being re-isolated each time — it is now treated as part of
the project's baseline compiler configuration, not as a variable under
test in later experiments.

## Blocking/tiling alone: no benefit (confirmed against reference project)

Implemented blocking/tiling on the force kernel's double loop
(BLOCK_SIZE=128 default, overridable via `make BLOCK_SIZE=<n>`), tiling
both the i and j loops so that a block of source particles is reused
across a block of target particles before moving to the next source
block. Accumulation order across j is unchanged from the naive kernel
(still strictly 0..n-1 per i, just grouped into blocks), so this is a
pure memory-access-pattern change, not a numerical one.

### Result

5 repetitions each, N=50000, 50 steps, GENOA partition, same build as
baseline (-restrict -O3 -march=native -flto -ffast-math):

| Kernel   | total_step median (s) |
|----------|------------------------|
| baseline | ~1.91                  |
| blocking | ~1.97-1.99             |

Blocking alone is ~3-4% *slower* than baseline, not faster. Energy
drift unaffected in all runs (status=OK), confirming this is purely a
performance effect.

### Independent confirmation

The reference project (Savorgnan)'s own kernel experiment shows the
identical qualitative result: naive ~7.576s vs blocked ~7.619s per
step (+0.6%), and their report states explicitly:

> "the blocks kernel by itself does not improve the computation time
> with respect to the naïve kernel, mainly because the prefetcher is
> good enough to vectorize perfectly the baseline code"

Two independent implementations, on the same hardware family (GENOA),
converge on the same conclusion: at these problem sizes, the working
set is small enough (or the hardware prefetcher good enough) that
manual tiling adds bookkeeping overhead (block-boundary computation,
per-block accumulator reset) without a corresponding cache-miss
reduction large enough to offset it.

### Why this isn't a failed experiment

This is documented as a genuine, useful negative result, not omitted.
It also motivates the next planned experiment: the reference project
found blocking becomes valuable specifically when *combined* with
Newton's third law, whose skipped/non-sequential access pattern
(j >= i+1 instead of 0..n-1) breaks the prefetcher's ability to
predict access, in a way blocking's explicit locality can recover.
Their combined "Blocks+Rsqrt+Third Law" kernel was their best serial
result (2.77x), versus blocks alone (1.01x) or third law alone (1.80x).
This is consistent with this project's planned Step 3bis (a dedicated
combination experiment, not just summing isolated speedups), rather
than assuming techniques compose additively.
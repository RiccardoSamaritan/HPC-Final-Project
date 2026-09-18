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

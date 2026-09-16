#!/bin/bash
# runners/run-experiment.sh
#
# Runs a serial N-body variant N times with profiling enabled, and archives
# everything (stdout log + profiling file) into data/raw/<variant>/.
#
# By default, --energy-every is set equal to --nsteps, so the energy check
# only fires once (at the last step) instead of polluting the timing
# measurement with an O(N^2) check on every step. Pass --energy-every
# explicitly in the extra args to override this (e.g. for a dedicated
# correctness-validation run with --energy-every 1).
#
# Usage:
#   ./runners/run-experiment.sh <binary> <variant_name> <ic_file> [options...]
#
# Example (performance run, energy checked only at the end):
#   ./runners/run-experiment.sh \
#       src/serial/baseline/nbody_direct_serial \
#       baseline \
#       plummer_50k.bin \
#       --nsteps 50 --dt 1e-4 --eps 0.05 --mass 1.0
#
# Example (explicit override, e.g. a validation run):
#   ./runners/run-experiment.sh \
#       src/serial/baseline/nbody_direct_serial \
#       baseline-validation \
#       plummer_50k.bin \
#       --nsteps 50 --dt 1e-4 --eps 0.05 --mass 1.0 --energy-every 1

set -euo pipefail

BINARY="$1"; shift
VARIANT="$1"; shift
IC_FILE="$1"; shift
EXTRA_ARGS=("$@")

REPS="${REPS:-5}"
OUTDIR="data/raw/${VARIANT}"

mkdir -p "$OUTDIR"

# --- Auto-default --energy-every to --nsteps, unless already given ---
HAS_ENERGY_EVERY=0
NSTEPS=""
for ((i = 0; i < ${#EXTRA_ARGS[@]}; i++)); do
    if [[ "${EXTRA_ARGS[$i]}" == "--energy-every" ]]; then
        HAS_ENERGY_EVERY=1
    fi
    if [[ "${EXTRA_ARGS[$i]}" == "--nsteps" ]]; then
        NSTEPS="${EXTRA_ARGS[$((i + 1))]}"
    fi
done

if [[ "$HAS_ENERGY_EVERY" -eq 0 ]]; then
    if [[ -z "$NSTEPS" ]]; then
        echo "error: --nsteps not found in arguments, cannot auto-default --energy-every" >&2
        exit 1
    fi
    echo "note: --energy-every not specified, defaulting to --nsteps ($NSTEPS)" \
         "(energy checked only at the last step; pass --energy-every explicitly to override)"
    EXTRA_ARGS+=(--energy-every "$NSTEPS")
fi

echo "=== Running ${VARIANT} (${REPS} repetitions) ==="
echo "Binary:  ${BINARY}"
echo "IC file: ${IC_FILE}"
echo "Args:    ${EXTRA_ARGS[*]}"
echo "Output:  ${OUTDIR}/"
echo

for i in $(seq 1 "$REPS"); do
    LOG_PATH="${OUTDIR}/run${i}.log"
    PROFILE_PATH="${OUTDIR}/profile_run${i}.txt"

    echo "[run ${i}/${REPS}] -> ${LOG_PATH}"

    "$BINARY" --input "$IC_FILE" "${EXTRA_ARGS[@]}" \
        --profiler 1 --profiler-path "$PROFILE_PATH" \
        > "$LOG_PATH" 2>&1

    if ! grep -q "status=OK" "$LOG_PATH"; then
        echo "  WARNING: run ${i} did not report status=OK, check ${LOG_PATH}"
    fi
done

echo
echo "=== Done. Archived in ${OUTDIR}/ ==="
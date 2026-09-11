#!/bin/bash
# runners/run-experiment.sh
#
# Runs a serial N-body variant N times with profiling enabled, and archives
# everything (stdout log + profiling file) into data/raw/<variant>/.
#
# Usage:
#   ./runners/run-experiment.sh <binary> <variant_name> <ic_file> [options...]
#
# Example:
#   ./runners/run-experiment.sh \
#       src/serial/v0-naive/nbody_direct_serial \
#       v0-naive \
#       plummer_50k.bin \
#       --nsteps 50 --dt 1e-4 --eps 0.05 --mass 1.0 --energy-every 10

set -euo pipefail

BINARY="$1"; shift
VARIANT="$1"; shift
IC_FILE="$1"; shift
EXTRA_ARGS=("$@")

REPS="${REPS:-5}"
OUTDIR="data/raw/${VARIANT}"

mkdir -p "$OUTDIR"

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

    # quick sanity check, fail fast if a run didn't reach status=OK
    if ! grep -q "status=OK" "$LOG_PATH"; then
        echo "  WARNING: run ${i} did not report status=OK, check ${LOG_PATH}"
    fi
done

echo
echo "=== Done. Archived in ${OUTDIR}/ ==="

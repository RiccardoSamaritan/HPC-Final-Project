#!/bin/bash
#SBATCH --job-name=baseline
#SBATCH --partition=GENOA
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=8G
#SBATCH --time=02:00:00
#SBATCH --output=log/%x_%j.out
#SBATCH --error=log/%x_%j.err

set -euo pipefail
cd "$SLURM_SUBMIT_DIR"
mkdir -p log

cd src/serial/baseline
make
./generate_ic --model 0 --n 50000 --seed 42 --output "$SLURM_SUBMIT_DIR/plummer_50k.bin"
cd "$SLURM_SUBMIT_DIR"

./runners/run-experiment.sh \
    src/serial/baseline/nbody_direct_serial \
    baseline \
    plummer_50k.bin \
    --nsteps 50 --dt 1e-4 --eps 0.05 --mass 1.0

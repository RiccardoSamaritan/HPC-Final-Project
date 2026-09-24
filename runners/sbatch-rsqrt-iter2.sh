#!/bin/bash
#SBATCH --job-name=rsqrt-i2
#SBATCH --partition=GENOA
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=8G
#SBATCH --time=02:00:00
#SBATCH --output=log/%x_%j.out
#SBATCH --error=log/%x_%j.err

set -euo pipefail
export LD_LIBRARY_PATH=$HOME/local/papi/lib:$LD_LIBRARY_PATH
cd "$SLURM_SUBMIT_DIR"
mkdir -p log

cd src/serial/experiments/rsqrt
make clean
make USE_PAPI=1 N_RSQRT_ITER=2
mv nbody_direct_serial nbody_direct_serial_iter2
mv generate_ic generate_ic_iter2
./generate_ic_iter2 --model 0 --n 50000 --seed 42 --output "$SLURM_SUBMIT_DIR/plummer_50k_iter2.bin"
cd "$SLURM_SUBMIT_DIR"

./runners/run-experiment.sh \
    src/serial/experiments/rsqrt/nbody_direct_serial_iter2 \
    rsqrt-iter2 \
    plummer_50k_iter2.bin \
    --nsteps 50 --dt 1e-4 --eps 0.05 --mass 1.0

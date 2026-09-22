#!/bin/bash
#SBATCH --job-name=aos-soa-soa-ffastmath
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

cd src/serial/experiments/aos-vs-soa
make
cc -std=c11 -I../../../common -DNBODY_USE_DOUBLE -O3 -march=native -flto \
   -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=199309L -ffast-math \
   -o nbody_direct_serial_ffastmath nbody_direct_serial.c \
   ../../../common/profiling.c ../../../common/particles.c ../../../common/utils.c -lm
./generate_ic --model 0 --n 50000 --seed 42 --output "$SLURM_SUBMIT_DIR/plummer_50k.bin"
cd "$SLURM_SUBMIT_DIR"

./runners/run-experiment.sh \
    src/serial/experiments/aos-vs-soa/nbody_direct_serial_ffastmath \
    aos-vs-soa-soa-ffastmath \
    plummer_50k.bin \
    --nsteps 50 --dt 1e-4 --eps 0.05 --mass 1.0 --layout soa

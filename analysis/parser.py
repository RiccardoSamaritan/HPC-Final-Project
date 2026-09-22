# Parses the profile_runN.txt files produced by runners/run-experiment.sh
# and builds a speedup table relative to a baseline experiment.

from __future__ import annotations

import argparse
import csv
import glob
import math
import os
import sys
from dataclasses import dataclass, field

import numpy as np


@dataclass
class RunData:
    run_idx: int
    force_time: np.ndarray
    total_step_time: np.ndarray
    first_drift_time: np.ndarray
    kick_time: np.ndarray
    second_drift_time: np.ndarray
    reading_time: float
    writing_time: float
    initial_energy_time: float
    config: dict
    # PAPI counters are None when the run wasn't built with USE_PAPI=1
    papi_cycles: np.ndarray | None = None
    papi_instructions: np.ndarray | None = None
    papi_l1_dcm: np.ndarray | None = None
    papi_l2_dcm: np.ndarray | None = None


@dataclass
class Experiment:
    name: str
    dir_path: str
    runs: list[RunData]
    n_runs: int
    n_steps: int
    force_time_matrix: np.ndarray
    total_step_time_matrix: np.ndarray
    mean_force_time: float
    std_force_time: float
    median_force_time: float
    trimmed_mean_force_time: float
    trimmed_std_force_time: float
    min_force_time: float
    max_force_time: float
    config: dict
    max_relative_energy_drift: float
    speedup: float = 1.0
    speedup_err: float = 0.0
    # only set if every run in the experiment has PAPI data, otherwise None
    mean_papi_cycles: float | None = None
    mean_papi_instructions: float | None = None
    mean_papi_l1_dcm: float | None = None
    mean_papi_l2_dcm: float | None = None
    ipc: float | None = None


SECTION_NAMES = ("total_step", "first_drift", "force", "kick", "second_drift")


def parse_profile_file(path):
    # turns a profile_runN.txt into {section_name: [raw lines]}
    sections = {}
    current = None

    with open(path) as f:
        for raw_line in f:
            line = raw_line.strip()
            if not line:
                continue
            if line.startswith("[") and line.endswith("]"):
                current = line[1:-1]
                sections[current] = []
                continue
            if current is None:
                raise ValueError(f"{path}: content before any [section] header: {line!r}")
            sections[current].append(line)

    return sections


def parse_config(lines):
    config = {}
    for line in lines:
        key, _, value = line.partition("=")
        key = key.strip()
        value = value.strip()
        try:
            config[key] = float(value)
        except ValueError:
            config[key] = value
    return config


def parse_one_time(lines):
    one_time = {}
    for line in lines:
        key, _, value = line.partition("=")
        one_time[key.strip()] = float(value.strip())
    return one_time


def parse_array_section(lines):
    return np.array([float(v) for v in lines], dtype=np.float64)


def parse_run_file(path, run_idx):
    sections = parse_profile_file(path)

    for required in ("config", "one-time") + SECTION_NAMES:
        if required not in sections:
            raise ValueError(f"{path}: missing required section [{required}]")

    config = parse_config(sections["config"])
    one_time = parse_one_time(sections["one-time"])

    force_time = parse_array_section(sections["force"])
    total_step_time = parse_array_section(sections["total_step"])
    first_drift_time = parse_array_section(sections["first_drift"])
    kick_time = parse_array_section(sections["kick"])
    second_drift_time = parse_array_section(sections["second_drift"])

    # PAPI sections may simply not be there (older runs, non-PAPI builds)
    papi_cycles = parse_array_section(sections["papi_cycles"]) if "papi_cycles" in sections else None
    papi_instructions = parse_array_section(sections["papi_instructions"]) if "papi_instructions" in sections else None
    papi_l1_dcm = parse_array_section(sections["papi_l1_dcm"]) if "papi_l1_dcm" in sections else None
    papi_l2_dcm = parse_array_section(sections["papi_l2_dcm"]) if "papi_l2_dcm" in sections else None

    return RunData(
        run_idx=run_idx,
        force_time=force_time,
        total_step_time=total_step_time,
        first_drift_time=first_drift_time,
        kick_time=kick_time,
        second_drift_time=second_drift_time,
        reading_time=one_time["reading_time"],
        writing_time=one_time["writing_time"],
        initial_energy_time=one_time["initial_energy_time"],
        config=config,
        papi_cycles=papi_cycles,
        papi_instructions=papi_instructions,
        papi_l1_dcm=papi_l1_dcm,
        papi_l2_dcm=papi_l2_dcm,
    )


def trimmed_stats(values, low_pct=2.0, high_pct=98.0):
    # cut the extreme 2% on each side, since the force loop occasionally gets
    # hit by OS scheduling noise and a plain mean/std would be skewed by it
    lo = np.percentile(values, low_pct)
    hi = np.percentile(values, high_pct)
    trimmed = values[(values >= lo) & (values <= hi)]
    if trimmed.size == 0:
        trimmed = values
    return float(np.mean(trimmed)), float(np.std(trimmed))


def papi_mean_or_none(runs, field_name):
    # only average the counter if every single run actually recorded it,
    # otherwise we'd be averaging over fewer samples without saying so
    values = [getattr(r, field_name) for r in runs]
    for v in values:
        if v is None:
            return None
    return float(np.mean(np.concatenate(values)))


def parse_experiment(dir_path, name=None):
    if name is None:
        name = os.path.basename(os.path.normpath(dir_path))

    run_files = sorted(glob.glob(os.path.join(dir_path, "profile_run*.txt")))
    if not run_files:
        raise ValueError(f"no profile_run*.txt files found in {dir_path}")

    runs = []
    for run_idx, path in enumerate(run_files, start=1):
        runs.append(parse_run_file(path, run_idx))

    n_runs = len(runs)
    n_steps_per_run = {len(r.force_time) for r in runs}
    if len(n_steps_per_run) != 1:
        raise ValueError(
            f"{dir_path}: runs have inconsistent step counts: {sorted(n_steps_per_run)}"
        )
    n_steps = n_steps_per_run.pop()

    force_time_matrix = np.stack([r.force_time for r in runs])
    total_step_time_matrix = np.stack([r.total_step_time for r in runs])

    # all repetitions of the same experiment should reach the same energy
    # drift (it's deterministic); if they don't, something's wrong with the
    # run and we'd rather fail loudly than silently average bad data
    drifts = [r.config.get("max_relative_energy_drift") for r in runs]
    if not all(math.isclose(d, drifts[0], rel_tol=1e-6) for d in drifts):
        raise ValueError(
            f"{dir_path}: max_relative_energy_drift differs across runs "
            f"(non-reproducible runs): {sorted(set(drifts))}"
        )
    max_relative_energy_drift = drifts[0]

    pooled_force = force_time_matrix.reshape(-1)
    trimmed_mean_force_time, trimmed_std_force_time = trimmed_stats(pooled_force)

    mean_papi_cycles = papi_mean_or_none(runs, "papi_cycles")
    mean_papi_instructions = papi_mean_or_none(runs, "papi_instructions")
    mean_papi_l1_dcm = papi_mean_or_none(runs, "papi_l1_dcm")
    mean_papi_l2_dcm = papi_mean_or_none(runs, "papi_l2_dcm")

    ipc = None
    if mean_papi_cycles and mean_papi_instructions is not None:
        ipc = mean_papi_instructions / mean_papi_cycles

    return Experiment(
        name=name,
        dir_path=dir_path,
        runs=runs,
        n_runs=n_runs,
        n_steps=n_steps,
        force_time_matrix=force_time_matrix,
        total_step_time_matrix=total_step_time_matrix,
        mean_force_time=float(np.mean(pooled_force)),
        std_force_time=float(np.std(pooled_force)),
        median_force_time=float(np.median(pooled_force)),
        trimmed_mean_force_time=trimmed_mean_force_time,
        trimmed_std_force_time=trimmed_std_force_time,
        min_force_time=float(np.min(pooled_force)),
        max_force_time=float(np.max(pooled_force)),
        config=runs[0].config,
        max_relative_energy_drift=max_relative_energy_drift,
        mean_papi_cycles=mean_papi_cycles,
        mean_papi_instructions=mean_papi_instructions,
        mean_papi_l1_dcm=mean_papi_l1_dcm,
        mean_papi_l2_dcm=mean_papi_l2_dcm,
        ipc=ipc,
    )


def load_all_experiments(dirs, names=None, baseline_idx=0):
    if names is None:
        names = [None] * len(dirs)

    experiments = [parse_experiment(d, n) for d, n in zip(dirs, names)]

    baseline = experiments[baseline_idx]
    t_base = baseline.trimmed_mean_force_time
    rel_err_base = baseline.trimmed_std_force_time / t_base if t_base != 0 else 0.0

    for exp in experiments:
        t_exp = exp.trimmed_mean_force_time
        rel_err_exp = exp.trimmed_std_force_time / t_exp if t_exp != 0 else 0.0

        exp.speedup = t_base / t_exp
        exp.speedup_err = exp.speedup * np.sqrt(rel_err_base ** 2 + rel_err_exp ** 2)

    return experiments


def status(exp):
    tol = exp.config.get("energy_tolerance")
    return "OK" if (tol is not None and exp.max_relative_energy_drift <= tol) else "?"


def fmt_or_na(value, fmt):
    return "N/A" if value is None else format(value, fmt)


def write_csv(experiments, path):
    with open(path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "name",
            "trimmed_mean_force_time",
            "std_force_time",
            "speedup",
            "speedup_err",
            "max_relative_energy_drift",
            "mean_papi_l1_dcm",
            "mean_papi_l2_dcm",
            "ipc",
            "status",
        ])
        for exp in experiments:
            writer.writerow([
                exp.name,
                exp.trimmed_mean_force_time,
                exp.trimmed_std_force_time,
                exp.speedup,
                exp.speedup_err,
                exp.max_relative_energy_drift,
                fmt_or_na(exp.mean_papi_l1_dcm, ".6e"),
                fmt_or_na(exp.mean_papi_l2_dcm, ".6e"),
                fmt_or_na(exp.ipc, ".4f"),
                status(exp),
            ])


def write_markdown(experiments, path):
    lines = [
        "| name | trimmed_mean_force_time (s) | std (s) | speedup | max_relative_energy_drift | "
        "mean_papi_l1_dcm | mean_papi_l2_dcm | ipc | status |",
        "|---|---|---|---|---|---|---|---|---|",
    ]
    for exp in experiments:
        lines.append(
            f"| {exp.name} | {exp.trimmed_mean_force_time:.6e} | "
            f"{exp.trimmed_std_force_time:.3e} | {exp.speedup:.4f} ± {exp.speedup_err:.4f} | "
            f"{exp.max_relative_energy_drift:.4e} | "
            f"{fmt_or_na(exp.mean_papi_l1_dcm, '.6e')} | "
            f"{fmt_or_na(exp.mean_papi_l2_dcm, '.6e')} | "
            f"{fmt_or_na(exp.ipc, '.4f')} | {status(exp)} |"
        )
    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    arg_parser = argparse.ArgumentParser(
        description="Parse aos-vs-soa-style profiling directories and report force-time speedups."
    )
    arg_parser.add_argument("dirs", nargs="+", help="data/raw/<variant> directories; first is the baseline")
    arg_parser.add_argument("--csv", metavar="PATH", help="write the results table as CSV to PATH")
    arg_parser.add_argument("--markdown", metavar="PATH", help="write the results table as Markdown to PATH")
    args = arg_parser.parse_args()

    if len(args.dirs) < 2:
        arg_parser.error("at least two directories are required (baseline_dir + dir ...)")

    experiments = load_all_experiments(args.dirs)

    header = (
        f"{'name':<24} {'trimmed_mean_force(s)':>22} {'std(s)':>12} "
        f"{'speedup':>10} {'speedup_err':>12} {'max_rel_drift':>14} "
        f"{'l1_dcm':>14} {'l2_dcm':>14} {'ipc':>8} {'status':>8}"
    )
    print(header)
    print("-" * len(header))
    for exp in experiments:
        print(
            f"{exp.name:<24} {exp.trimmed_mean_force_time:>22.6e} "
            f"{exp.trimmed_std_force_time:>12.3e} {exp.speedup:>10.4f} "
            f"{exp.speedup_err:>12.4f} {exp.max_relative_energy_drift:>14.4e} "
            f"{fmt_or_na(exp.mean_papi_l1_dcm, '.6e'):>14} "
            f"{fmt_or_na(exp.mean_papi_l2_dcm, '.6e'):>14} "
            f"{fmt_or_na(exp.ipc, '.4f'):>8} {status(exp):>8}"
        )

    if args.csv:
        write_csv(experiments, args.csv)
    if args.markdown:
        write_markdown(experiments, args.markdown)

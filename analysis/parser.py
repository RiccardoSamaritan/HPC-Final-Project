"""
Parser for the profiling files produced by runners/run-experiment.sh.

Each experiment directory (data/raw/<variant>/) contains one profiling
file per repetition (profile_run1.txt ... profile_runN.txt), each with a
[config] section, a [one-time] section, and five per-step timing sections
([total_step], [first_drift], [force], [kick], [second_drift]), one float
per line, one line per simulation step.
"""

from __future__ import annotations

import argparse
import csv
import glob
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


@dataclass
class Experiment:
    name: str
    dir_path: str
    runs: list[RunData]
    n_runs: int
    n_steps: int
    force_time_matrix: np.ndarray      # shape (n_runs, n_steps)
    total_step_time_matrix: np.ndarray
    # Force-time aggregate stats (this is the primary metric for speedup)
    mean_force_time: float
    std_force_time: float
    median_force_time: float
    trimmed_mean_force_time: float     # 2nd-98th percentile trim, pooled across all runs*steps
    trimmed_std_force_time: float
    min_force_time: float
    max_force_time: float
    config: dict
    max_relative_energy_drift: float
    speedup: float = 1.0                # set externally relative to a baseline
    speedup_err: float = 0.0


_SECTION_ARRAYS = ("total_step", "first_drift", "force", "kick", "second_drift")


def _parse_profile_file(path: str) -> dict:
    """Parse one profile_run*.txt file into a dict of raw sections."""
    sections: dict[str, list[str]] = {}
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


def _parse_config(lines: list[str]) -> dict:
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


def _parse_one_time(lines: list[str]) -> dict:
    one_time = {}
    for line in lines:
        key, _, value = line.partition("=")
        one_time[key.strip()] = float(value.strip())
    return one_time


def _parse_run_file(path: str, run_idx: int) -> RunData:
    sections = _parse_profile_file(path)

    for required in ("config", "one-time") + _SECTION_ARRAYS:
        if required not in sections:
            raise ValueError(f"{path}: missing required section [{required}]")

    config = _parse_config(sections["config"])
    one_time = _parse_one_time(sections["one-time"])

    arrays = {
        name: np.array([float(v) for v in sections[name]], dtype=np.float64)
        for name in _SECTION_ARRAYS
    }

    return RunData(
        run_idx=run_idx,
        force_time=arrays["force"],
        total_step_time=arrays["total_step"],
        first_drift_time=arrays["first_drift"],
        kick_time=arrays["kick"],
        second_drift_time=arrays["second_drift"],
        reading_time=one_time["reading_time"],
        writing_time=one_time["writing_time"],
        initial_energy_time=one_time["initial_energy_time"],
        config=config,
    )


def _trimmed_stats(values: np.ndarray, low_pct: float = 2.0, high_pct: float = 98.0) -> tuple[float, float]:
    """Mean and std of values within [low_pct, high_pct] percentiles."""
    lo = np.percentile(values, low_pct)
    hi = np.percentile(values, high_pct)
    trimmed = values[(values >= lo) & (values <= hi)]
    if trimmed.size == 0:
        trimmed = values
    return float(np.mean(trimmed)), float(np.std(trimmed))


def parse_experiment(dir_path: str, name: str | None = None) -> Experiment:
    if name is None:
        name = os.path.basename(os.path.normpath(dir_path))

    run_files = sorted(glob.glob(os.path.join(dir_path, "profile_run*.txt")))
    if not run_files:
        raise ValueError(f"no profile_run*.txt files found in {dir_path}")

    runs: list[RunData] = []
    for run_idx, path in enumerate(run_files, start=1):
        runs.append(_parse_run_file(path, run_idx))

    n_runs = len(runs)
    n_steps_per_run = {len(r.force_time) for r in runs}
    if len(n_steps_per_run) != 1:
        raise ValueError(
            f"{dir_path}: runs have inconsistent step counts: {sorted(n_steps_per_run)}"
        )
    n_steps = n_steps_per_run.pop()

    force_time_matrix = np.stack([r.force_time for r in runs])
    total_step_time_matrix = np.stack([r.total_step_time for r in runs])

    drifts = {r.config.get("max_relative_energy_drift") for r in runs}
    if len(drifts) != 1:
        raise ValueError(
            f"{dir_path}: max_relative_energy_drift differs across runs "
            f"(non-reproducible runs): {sorted(drifts)}"
        )
    max_relative_energy_drift = drifts.pop()

    pooled_force = force_time_matrix.reshape(-1)
    trimmed_mean_force_time, trimmed_std_force_time = _trimmed_stats(pooled_force)

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
    )


def load_all_experiments(
    dirs: list[str],
    names: list[str] | None = None,
    baseline_idx: int = 0,
) -> list[Experiment]:
    if names is None:
        names = [None] * len(dirs)

    experiments = [parse_experiment(d, n) for d, n in zip(dirs, names)]

    baseline = experiments[baseline_idx]
    t_base = baseline.trimmed_mean_force_time
    rel_err_base = (
        baseline.trimmed_std_force_time / t_base if t_base != 0 else 0.0
    )

    for exp in experiments:
        t_exp = exp.trimmed_mean_force_time
        rel_err_exp = exp.trimmed_std_force_time / t_exp if t_exp != 0 else 0.0

        exp.speedup = t_base / t_exp
        exp.speedup_err = exp.speedup * np.sqrt(rel_err_base ** 2 + rel_err_exp ** 2)

    return experiments


def _status(exp: Experiment) -> str:
    tol = exp.config.get("energy_tolerance")
    return "OK" if (tol is not None and exp.max_relative_energy_drift <= tol) else "?"


def _write_csv(experiments: list[Experiment], path: str) -> None:
    with open(path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(
            [
                "name",
                "trimmed_mean_force_time",
                "std_force_time",
                "speedup",
                "speedup_err",
                "max_relative_energy_drift",
                "status",
            ]
        )
        for exp in experiments:
            writer.writerow(
                [
                    exp.name,
                    exp.trimmed_mean_force_time,
                    exp.trimmed_std_force_time,
                    exp.speedup,
                    exp.speedup_err,
                    exp.max_relative_energy_drift,
                    _status(exp),
                ]
            )


def _write_markdown(experiments: list[Experiment], path: str) -> None:
    lines = [
        "| name | trimmed_mean_force_time (s) | std (s) | speedup | max_relative_energy_drift | status |",
        "|---|---|---|---|---|---|",
    ]
    for exp in experiments:
        lines.append(
            f"| {exp.name} | {exp.trimmed_mean_force_time:.6e} | "
            f"{exp.trimmed_std_force_time:.3e} | {exp.speedup:.4f} ± {exp.speedup_err:.4f} | "
            f"{exp.max_relative_energy_drift:.4e} | {_status(exp)} |"
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
        f"{'speedup':>10} {'speedup_err':>12} {'max_rel_drift':>14} {'status':>8}"
    )
    print(header)
    print("-" * len(header))
    for exp in experiments:
        print(
            f"{exp.name:<24} {exp.trimmed_mean_force_time:>22.6e} "
            f"{exp.trimmed_std_force_time:>12.3e} {exp.speedup:>10.4f} "
            f"{exp.speedup_err:>12.4f} {exp.max_relative_energy_drift:>14.4e} {_status(exp):>8}"
        )

    if args.csv:
        _write_csv(experiments, args.csv)
    if args.markdown:
        _write_markdown(experiments, args.markdown)

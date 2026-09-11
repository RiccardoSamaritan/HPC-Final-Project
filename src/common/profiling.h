/*
 * profiling.h
 *
 * Wall-clock instrumentation shared by every solver variant (serial, OpenMP,
 * MPI, hybrid).  The profiler records one-time costs (I/O, initial energy
 * check) and per-step costs broken down by leapfrog phase, so that a speedup
 * measured between two variants can be attributed to a specific phase instead
 * of only the total run time.
 *
 * This header does not depend on nbody_common.h so that it can be reused
 * unchanged across variants that select different dtype builds; all timings
 * are stored as plain double, independently of the simulation arithmetic.
 */

#ifndef PROFILING_H
#define PROFILING_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <stddef.h>
#include <time.h>

#ifdef USE_PAPI
#include <papi.h>
#define PROFILING_PAPI_EVENTS_COUNT 5
#endif


typedef struct profiler_s
{
  // One-time measurements, outside the step loop
  double   reading_time;
  double   writing_time;
  double   initial_energy_time;

  // Per-step measurements, one entry per simulation step
  size_t   n_steps;
  double  *first_drift_time;
  double  *force_time;
  double  *kick_time;
  double  *second_drift_time;
  double  *total_step_time;

#ifdef USE_PAPI
  // Optional hardware counters, only meaningful around the force kernel.
  // Not enabled by default: build with -DUSE_PAPI and link -lpapi.
  int        papi_eventset;
  long long *papi_cycles;
  long long *papi_instructions;
  long long *papi_l1_dcm;
  long long *papi_l2_dcm;
  long long *papi_vec_dp;
#endif
} profiler_t;


/*
 * Configuration echoed alongside the timings, so that a saved profiling file
 * is self-describing and does not depend on an external run log to be
 * interpreted later.
 */
typedef struct profiler_config_s
{
  const char *variant_name;      // e.g. "v0-naive", "v2-rsqrt"
  size_t      n_particles;
  size_t      n_steps;
  double      dt;
  double      eps;
  double      g;
  double      mass;
  double      max_relative_energy_drift;
  double      energy_tolerance;
} profiler_config_t;


/*
 * Monotonic wall-clock reading, in seconds, suitable for measuring intervals.
 * Not tied to wall time of day, so it is safe across leap seconds and clock
 * adjustments.
 */
static inline double get_time (void)
{
  struct timespec  ts;

  clock_gettime (CLOCK_MONOTONIC, &ts);
  return (double) ts.tv_sec + (double) ts.tv_nsec * 1.0e-9;
}


void profiler_allocate (profiler_t *profiler,     // output profiler, previous content ignored
                        size_t      n_steps       // number of steps to be recorded
			);

void profiler_free (profiler_t *profiler          // profiler to release
		    );

/*
 * Print median, trimmed mean (10%), and standard deviation for every
 * recorded phase to stdout.  Meant for a quick look during interactive runs;
 * use save_statistics for a machine-readable record.
 */
void print_statistics (const profiler_t *profiler    // profiler with completed measurements
		       );

/*
 * Write every recorded phase plus the run configuration to a new file at
 * `path`.  The file is created (or truncated) here, not appended to, so that
 * one profiling run always produces exactly one self-contained file; callers
 * that want several repetitions should pass a distinct path per repetition
 * (see runners/ for the convention used across this project).
 */
void save_statistics (const char              *path,        // output file path, created/truncated
                      const profiler_config_t *config,      // run configuration to echo
                      const profiler_t        *profiler     // profiler with completed measurements
		      );

#ifdef USE_PAPI
/*
 * PAPI counters are optional and disabled by default (the exam text marks
 * them as "optional, not covered in the course").  When enabled, they are
 * only read around the force kernel via profiler_papi_start/stop, matching
 * the region that AoS-vs-SoA and vectorisation experiments care about.
 */
void profiler_papi_init (profiler_t *profiler);
void profiler_papi_start (profiler_t *profiler);
void profiler_papi_stop (profiler_t *profiler, size_t step);
void profiler_papi_free (profiler_t *profiler);
#endif

#endif // PROFILING_H
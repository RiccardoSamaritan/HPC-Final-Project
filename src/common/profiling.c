/*
 * profiling.c
 *
 * Implementation of the shared wall-clock profiler declared in profiling.h.
 * Self-contained: no dependency on the solver's internal `die`/allocation
 * helpers, so this file can be compiled and linked into every variant
 * (serial, OpenMP, MPI, hybrid) without pulling in solver-specific code.
 */

#include "profiling.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


static void profiling_die (const char *message,   // fatal error message
                           const char *path       // associated file path, may be NULL
			   )
{
  if (path != NULL)
    fprintf (stderr, "profiling: %s: %s\n", message, path);
  else
    fprintf (stderr, "profiling: %s\n", message);

  exit (EXIT_FAILURE);
}


static double *profiling_alloc_doubles (size_t n    // number of doubles to allocate
					)
{
  double *ptr = malloc (n * sizeof (double));

  if (ptr == NULL)
    profiling_die ("allocation failed", NULL);

  return ptr;
}


void profiler_allocate (profiler_t *profiler,
                        size_t      n_steps)
{
  profiler->reading_time        = 0.0;
  profiler->writing_time        = 0.0;
  profiler->initial_energy_time = 0.0;
  profiler->total_run_time      = 0.0;

  profiler->n_steps           = n_steps;
  profiler->first_drift_time  = profiling_alloc_doubles (n_steps);
  profiler->force_time        = profiling_alloc_doubles (n_steps);
  profiler->kick_time         = profiling_alloc_doubles (n_steps);
  profiler->second_drift_time = profiling_alloc_doubles (n_steps);
  profiler->total_step_time   = profiling_alloc_doubles (n_steps);

#ifdef USE_PAPI
  profiler->papi_eventset    = PAPI_NULL;
  profiler->papi_cycles       = malloc (n_steps * sizeof (long long));
  profiler->papi_instructions = malloc (n_steps * sizeof (long long));
  profiler->papi_l1_dcm       = malloc (n_steps * sizeof (long long));
  profiler->papi_l2_dcm       = malloc (n_steps * sizeof (long long));
  profiler->papi_fma_ins      = malloc (n_steps * sizeof (long long));
#endif
}


void profiler_free (profiler_t *profiler)
{
  free (profiler->first_drift_time);
  free (profiler->force_time);
  free (profiler->kick_time);
  free (profiler->second_drift_time);
  free (profiler->total_step_time);

#ifdef USE_PAPI
  free (profiler->papi_cycles);
  free (profiler->papi_instructions);
  free (profiler->papi_l1_dcm);
  free (profiler->papi_l2_dcm);
  free (profiler->papi_fma_ins);
#endif
}


static int compare_doubles (const void *a,
                            const void *b)
{
  const double  da = *(const double *) a;
  const double  db = *(const double *) b;

  if (da < db) return -1;
  if (da > db) return 1;
  return 0;
}


/*
 * Compute median, 10%-trimmed mean, and standard deviation of `values`.
 * Operates on a scratch copy so the caller's array order is preserved.
 */
static void summarize (const double *values,
                       size_t        n,
                       double       *out_median,
                       double       *out_trimmed_mean,
                       double       *out_std)
{
  double *sorted = profiling_alloc_doubles (n);
  size_t  i;

  for (i = 0u; i < n; ++i) sorted[i] = values[i];
  qsort (sorted, n, sizeof (double), compare_doubles);

  *out_median = (n % 2u != 0u)
    ? sorted[n / 2u]
    : (sorted[(n - 1u) / 2u] + sorted[n / 2u]) / 2.0;

  size_t  trim  = n / 10u;                 // 10% trim, negligible effect below ~10 samples
  size_t  valid = n - 2u * trim;
  double  sum   = 0.0;

  for (i = trim; i < n - trim; ++i) sum += sorted[i];
  *out_trimmed_mean = sum / (double) valid;

  double  variance = 0.0;

  for (i = trim; i < n - trim; ++i)
    {
      double  dev = sorted[i] - *out_trimmed_mean;
      variance += dev * dev;
    }
  *out_std = sqrt (variance / (double) valid);

  free (sorted);
}


static void print_phase (const char   *label,
                         const double *values,
                         size_t        n)
{
  if (n == 0u) return;

  double  median;
  double  trimmed_mean;
  double  std;

  summarize (values, n, &median, &trimmed_mean, &std);
  printf ("%-14s median=%.6e s  trimmed_mean=%.6e s  std=%.6e s\n",
          label, median, trimmed_mean, std);
}


#ifdef USE_PAPI
/*
 * Same median/trimmed-mean/std summary as print_phase, but for a
 * long-long-valued PAPI counter series instead of a double-valued timing
 * series; converts to double once so summarize() can be reused unchanged.
 */
static void print_papi_counter (const char      *label,
                                const long long *values,
                                size_t           n)
{
  if (n == 0u) return;

  double *as_double = profiling_alloc_doubles (n);
  size_t  i;

  for (i = 0u; i < n; ++i) as_double[i] = (double) values[i];

  double  median;
  double  trimmed_mean;
  double  std;

  summarize (as_double, n, &median, &trimmed_mean, &std);
  printf ("%-14s median=%.6e    trimmed_mean=%.6e    std=%.6e\n",
          label, median, trimmed_mean, std);

  free (as_double);
}


static double papi_mean (const long long *values,
                         size_t           n)
{
  double  sum = 0.0;
  size_t  i;

  if (n == 0u) return 0.0;
  for (i = 0u; i < n; ++i) sum += (double) values[i];
  return sum / (double) n;
}
#endif


void print_statistics (const profiler_t *profiler)
{
  printf ("\n--- profiling report ---\n");
  printf ("%-14s : %.6e s\n", "file read",      profiler->reading_time);
  printf ("%-14s : %.6e s\n", "file write",     profiler->writing_time);
  printf ("%-14s : %.6e s\n", "initial energy", profiler->initial_energy_time);
  printf ("%-14s : %.6e s\n", "total run",      profiler->total_run_time);

  printf ("\n--- per-step phases (%zu steps) ---\n", profiler->n_steps);
  print_phase ("total step",   profiler->total_step_time,   profiler->n_steps);
  print_phase ("first drift",  profiler->first_drift_time,  profiler->n_steps);
  print_phase ("force",        profiler->force_time,        profiler->n_steps);
  print_phase ("kick",         profiler->kick_time,         profiler->n_steps);
  print_phase ("second drift", profiler->second_drift_time, profiler->n_steps);

#ifdef USE_PAPI
  printf ("\n--- PAPI hardware counters, force kernel (%zu steps) ---\n", profiler->n_steps);
  print_papi_counter ("cycles",       profiler->papi_cycles,       profiler->n_steps);
  print_papi_counter ("instructions", profiler->papi_instructions, profiler->n_steps);
  print_papi_counter ("L1 dcm",       profiler->papi_l1_dcm,       profiler->n_steps);
  print_papi_counter ("L2 dcm",       profiler->papi_l2_dcm,       profiler->n_steps);
  print_papi_counter ("FMA ins",      profiler->papi_fma_ins,      profiler->n_steps);

  {
    const double  mean_cycles = papi_mean (profiler->papi_cycles,       profiler->n_steps);
    const double  mean_ins    = papi_mean (profiler->papi_instructions, profiler->n_steps);
    const double  ipc         = (mean_cycles > 0.0) ? mean_ins / mean_cycles : 0.0;

    printf ("%-14s : %.6e\n", "IPC", ipc);
  }
#endif
}


static void save_phase (FILE         *fp,
                        const char   *label,
                        const double *values,
                        size_t        n)
{
  size_t  i;

  fprintf (fp, "[%s]\n", label);
  for (i = 0u; i < n; ++i)
    fprintf (fp, "%.9e\n", values[i]);
  fprintf (fp, "\n");
}


#ifdef USE_PAPI
static void save_papi_counter (FILE            *fp,
                               const char      *label,
                               const long long *values,
                               size_t           n)
{
  size_t  i;

  fprintf (fp, "[%s]\n", label);
  for (i = 0u; i < n; ++i)
    fprintf (fp, "%lld\n", values[i]);
  fprintf (fp, "\n");
}
#endif


void save_statistics (const char              *path,
                      const profiler_config_t *config,
                      const profiler_t        *profiler)
{
  FILE *fp = fopen (path, "w");     // create/truncate: one file per run, never append

  if (fp == NULL)
    profiling_die ("cannot open profiling output file for writing", path);

  fprintf (fp, "[config]\n");
  fprintf (fp, "variant=%s\n",                     config->variant_name);
  fprintf (fp, "n_particles=%zu\n",                config->n_particles);
  fprintf (fp, "n_steps=%zu\n",                    config->n_steps);
  fprintf (fp, "dt=%.9e\n",                        config->dt);
  fprintf (fp, "eps=%.9e\n",                       config->eps);
  fprintf (fp, "G=%.9e\n",                         config->g);
  fprintf (fp, "mass=%.9e\n",                      config->mass);
  fprintf (fp, "max_relative_energy_drift=%.9e\n", config->max_relative_energy_drift);
  fprintf (fp, "energy_tolerance=%.9e\n",          config->energy_tolerance);
  fprintf (fp, "\n");

  fprintf (fp, "[one-time]\n");
  fprintf (fp, "reading_time=%.9e\n",        profiler->reading_time);
  fprintf (fp, "writing_time=%.9e\n",        profiler->writing_time);
  fprintf (fp, "initial_energy_time=%.9e\n", profiler->initial_energy_time);
  fprintf (fp, "total_run_time=%.9e\n",      profiler->total_run_time);
  fprintf (fp, "\n");

  save_phase (fp, "total_step",   profiler->total_step_time,   profiler->n_steps);
  save_phase (fp, "first_drift",  profiler->first_drift_time,  profiler->n_steps);
  save_phase (fp, "force",        profiler->force_time,        profiler->n_steps);
  save_phase (fp, "kick",         profiler->kick_time,         profiler->n_steps);
  save_phase (fp, "second_drift", profiler->second_drift_time, profiler->n_steps);

#ifdef USE_PAPI
  save_papi_counter (fp, "papi_cycles",       profiler->papi_cycles,       profiler->n_steps);
  save_papi_counter (fp, "papi_instructions", profiler->papi_instructions, profiler->n_steps);
  save_papi_counter (fp, "papi_l1_dcm",       profiler->papi_l1_dcm,       profiler->n_steps);
  save_papi_counter (fp, "papi_l2_dcm",       profiler->papi_l2_dcm,       profiler->n_steps);
  save_papi_counter (fp, "papi_fma_ins",      profiler->papi_fma_ins,      profiler->n_steps);
#endif

  if (fclose (fp) != 0)
    profiling_die ("error while closing profiling output file", path);
}


#ifdef USE_PAPI

void profiler_papi_init (profiler_t *profiler)
{
  int  retval = PAPI_library_init (PAPI_VER_CURRENT);

  if (retval != PAPI_VER_CURRENT)
    profiling_die ("PAPI_library_init failed", NULL);

  profiler->papi_eventset = PAPI_NULL;
  if (PAPI_create_eventset (&profiler->papi_eventset) != PAPI_OK)
    profiling_die ("PAPI_create_eventset failed", NULL);

  if (PAPI_add_event (profiler->papi_eventset, PAPI_TOT_CYC) != PAPI_OK)
    profiling_die ("PAPI_add_event failed for PAPI_TOT_CYC", NULL);
  if (PAPI_add_event (profiler->papi_eventset, PAPI_TOT_INS) != PAPI_OK)
    profiling_die ("PAPI_add_event failed for PAPI_TOT_INS", NULL);
  if (PAPI_add_event (profiler->papi_eventset, PAPI_L1_DCM) != PAPI_OK)
    profiling_die ("PAPI_add_event failed for PAPI_L1_DCM", NULL);
  if (PAPI_add_event (profiler->papi_eventset, PAPI_L2_DCM) != PAPI_OK)
    profiling_die ("PAPI_add_event failed for PAPI_L2_DCM", NULL);
  if (PAPI_add_event (profiler->papi_eventset, PAPI_FMA_INS) != PAPI_OK)
    profiling_die ("PAPI_add_event failed for PAPI_FMA_INS", NULL);
}


void profiler_papi_start (profiler_t *profiler)
{
  if (PAPI_start (profiler->papi_eventset) != PAPI_OK)
    profiling_die ("PAPI_start failed", NULL);
}


void profiler_papi_stop (profiler_t *profiler,
                         size_t      step)
{
  long long  values[PROFILING_PAPI_EVENTS_COUNT] = {0};

  if (PAPI_stop (profiler->papi_eventset, values) != PAPI_OK)
    profiling_die ("PAPI_stop failed", NULL);

  profiler->papi_cycles[step]       = values[0];
  profiler->papi_instructions[step] = values[1];
  profiler->papi_l1_dcm[step]       = values[2];
  profiler->papi_l2_dcm[step]       = values[3];
  profiler->papi_fma_ins[step]      = values[4];
}


void profiler_papi_free (profiler_t *profiler)
{
  PAPI_cleanup_eventset (profiler->papi_eventset);
  PAPI_destroy_eventset (&profiler->papi_eventset);
  PAPI_shutdown ();
}

#endif
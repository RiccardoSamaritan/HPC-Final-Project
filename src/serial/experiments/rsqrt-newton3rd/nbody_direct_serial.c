/*
 * nbody_direct_serial.c
 *
 * Serial C11 reference implementation for the direct gravitational N-body
 * exercise.  The O(N^2) force kernel is simple;
 * optimising the kernel is part of the assignment. 
 * The kernel is the natural place to discuss SoA data layout, cache locality,
 *  Newton's third law, accumulator dependency chains, rsqrt, OpenMP
 *  reductions, and MPI ring-shift communication.
 *
 * Units are dimensionless.  By default G = 1, particle mass = 1, and the
 * softened potential is
 *
 *   phi_ij = - G m^2 / sqrt(|r_i-r_j|^2 + eps^2).
 *
 * Binary input/output file format, native endian:
 *
 *   8 bytes       magic "NBODYF1\0"
 *   uint64_t      number of particles
 *   N records     x y z vx vy vz as six IEEE single-precision floats
 *
 * The simulation arithmetic uses dtype, selected at compile time:
 *
 *   -DNBODY_USE_DOUBLE    default double-precision arithmetic
 *   -DNBODY_USE_FLOAT     single-precision arithmetic
 *
 * Files are intentionally still stored in single precision, independently of
 * dtype.
 *
 */

#include "../../../common/nbody_common.h"
#include "../../../common/particles.h"
#include "../../../common/profiling.h"
#include "../../../common/utils.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================================

   : ------------------------------------------------------ :
   :  DATA TYPES &                                          :
   :  DATA STRUCTURES                                       :
   : ------------------------------------------------------ :
 */ 


/* ========================================================================================
   
   : ------------------------------------------------------ :
   :  UTILITIES                                             :
   : ------------------------------------------------------ :
 */ 





/* ======================================================================================== 

   : ------------------------------------------------------ :
   :  INTEGRATIION                                          :
   : ------------------------------------------------------ :
 */ 



/*
 * Naive direct O(N^2) softened gravitational acceleration.
 *
 * This is the most interesting kernel.  
 * A very transparent form: one i particle, one j loop, no Newton-third-law
 * reuse, one accumulator per component, and a scalar sqrt from libm.  That is
 * correct, but it leaves the optimisation space visible:
 *
 *   - which data qualifiers must be introduced for the input/output pointers?
 *   - exploit or deliberately avoid Newton's third law;
 *   - split the accumulators to shorten dependency chains;
 *   - use rsqrt plus Newton refinement, then quantify energy error;
 *   - block or transpose data to improve cache/TLB behaviour;
 *   - add OpenMP without atomics in the inner loop;
 *   - later replace the all-pairs loop with an MPI ring shift.
 *
 * ... reason about the needed qualifiers to unleash compiler's optimization
 *
 */
/*
 * Fast approximate reciprocal square root via bit-level manipulation
 * (the classic "Quake" trick), generalised to float/double via dtype.
 * The magic constant halves the exponent bits of the IEEE representation,
 * giving a first-order approximation of 1/sqrt(x) with ~3-4% relative error.
 */
static inline dtype dtype_rsqrt_approx (dtype x)
{
#if defined (NBODY_USE_FLOAT)
  union { float f; uint32_t i; } conv;
  conv.f = x;
  conv.i = 0x5f3759dfu - (conv.i >> 1);
  return conv.f;
#else
  union { double f; uint64_t i; } conv;
  conv.f = x;
  conv.i = 0x5fe6eb50c7b537a9ULL - (conv.i >> 1);
  return conv.f;
#endif
}

/*
 * One Newton-Raphson refinement step applied to the approximation above.
 * y_{n+1} = y_n * (1.5 - 0.5 * x * y_n^2)
 * Reduces the relative error from ~3-4% to ~0.17% (float) after a single
 * iteration; the energy-drift check quantifies whether this is acceptable
 * for the softened potential used here.
 */
static inline dtype dtype_rsqrt (dtype x)
{
  dtype y = dtype_rsqrt_approx (x);
  y = y * ((dtype) 1.5 - (dtype) 0.5 * x * y * y);
  return y;
}

static void compute_accelerations_rsqrt_third_law (size_t              n,
                                                    dtype                g,
                                                    dtype                mass,
                                                    dtype                eps,
                                                    const dtype * restrict x,
                                                    const dtype * restrict y,
                                                    const dtype * restrict z,
                                                    dtype       * restrict ax,
                                                    dtype       * restrict ay,
                                                    dtype       * restrict az
                                                    )
{
  const dtype  eps2 = eps * eps;
  size_t       i;
  size_t       j;

  for (i = 0u; i < n; ++i)
    {
      ax[i] = (dtype) 0.0;
      ay[i] = (dtype) 0.0;
      az[i] = (dtype) 0.0;
    }

  for (i = 0u; i < n - 1u; ++i)
    {
      const dtype  xi  = x[i];
      const dtype  yi  = y[i];
      const dtype  zi  = z[i];
      dtype        axi = ax[i];
      dtype        ayi = ay[i];
      dtype        azi = az[i];

      for (j = i + 1u; j < n; ++j)
        {
          const dtype  dx   = x[j] - xi;
          const dtype  dy   = y[j] - yi;
          const dtype  dz   = z[j] - zi;
          const dtype  r2   = dx * dx + dy * dy + dz * dz + eps2;
          const dtype  invr = dtype_rsqrt (r2);
          const dtype  s    = g * mass * invr * invr * invr;

          axi += dx * s;
          ayi += dy * s;
          azi += dz * s;

          ax[j] -= dx * s;
          ay[j] -= dy * s;
          az[j] -= dz * s;
        }

      ax[i] = axi;
      ay[i] = ayi;
      az[i] = azi;
    }
}

/*
 * Drift all particles by a time interval using the current velocities.  
 * The DKD leapfrog workflow calls it twice per step: a half-drift before the
 * force evaluation and a half-drift after the kick.
 *
 * Again: are data qualifiers missed for optimization?
 */
static void drift (particles_t *p,       // particle positions are modified in place
                   dtype        dt       // drift interval, often 0.5 * full step
		   )
{
  size_t  n  = p->n;
  dtype  *x  = p->x;
  dtype  *y  = p->y;
  dtype  *z  = p->z;
  dtype  *vx = p->vx;
  dtype  *vy = p->vy;
  dtype  *vz = p->vz;
  size_t  i;

  for (i = 0u; i < n; ++i)
    {
      x[i] += dt * vx[i];
      y[i] += dt * vy[i];
      z[i] += dt * vz[i];
    }
}

/*
 * Kick all velocities using the current accelerations.  his is the K in DKD
 */
static void kick (particles_t *p,       // particle velocities are modified in place
                  dtype        dt       // full kick interval
		  )
{
  size_t   n  = p->n;
  dtype  * vx = p->vx;
  dtype  * vy = p->vy;
  dtype  * vz = p->vz;
  dtype  * ax = p->ax;
  dtype  * ay = p->ay;
  dtype  * az = p->az;
  size_t   i;

  for (i = 0u; i < n; ++i)
    {
      vx[i] += dt * ax[i];
      vy[i] += dt * ay[i];
      vz[i] += dt * az[i];
    }
}

/*
 * Compute one DKD leapfrog step:
 *
 *   1. drift positions by dt/2;
 *   2. compute accelerations at the half-step positions;
 *   3. kick velocities by dt;
 *   4. drift positions by dt/2 with the updated velocities.
 *
 * This keeps positions and velocities synchronised at integer time levels 
 */
static void leapfrog_dkd_step (particles_t *p,          // complete particle state, modified in place
                               dtype        g,          // gravitational constant
                               dtype        eps,        // softening length
                               dtype        dt,         // full time step
                               profiler_t  *profiler,   // optional per-phase timings, NULL to disable
                               size_t       step_idx    // index into profiler's per-step arrays
			       )
{
  double  t0;
  double  t1;

  if (profiler != NULL)
    {
      t0 = get_time ();
      drift (p, (dtype) 0.5 * dt);
      t1 = get_time ();
      profiler->first_drift_time[step_idx] = t1 - t0;
      t0 = t1;

      compute_accelerations_rsqrt_third_law (p->n, g, p->mass, eps,
                                   p->x, p->y, p->z,
                                   p->ax, p->ay, p->az);
      t1 = get_time ();
      profiler->force_time[step_idx] = t1 - t0;
      t0 = t1;

      kick (p, dt);
      t1 = get_time ();
      profiler->kick_time[step_idx] = t1 - t0;
      t0 = t1;

      drift (p, (dtype) 0.5 * dt);
      t1 = get_time ();
      profiler->second_drift_time[step_idx] = t1 - t0;
    }
  else
    {
      drift (p, (dtype) 0.5 * dt);
      compute_accelerations_rsqrt_third_law (p->n, g, p->mass, eps,
                                   p->x, p->y, p->z,
                                   p->ax, p->ay, p->az);
      kick (p, dt);
      drift (p, (dtype) 0.5 * dt);
    }
}

/*
 * Kinetic energy of the equal-mass system.
 * A long-double accumulator is used so that summation roundoff in the check is less likely to hide
 * errors caused by the integration or the force kernel.
 */
static dtype kinetic_energy (const particles_t *p    // particle velocities are read-only
			     )
{
  size_t        n    = p->n;
  dtype         mass = p->mass;
  long double   sum  = 0.0L;
  size_t        i;

  for (i = 0u; i < n; ++i)
    {
      const long double  vx = (long double) p->vx[i];
      const long double  vy = (long double) p->vy[i];
      const long double  vz = (long double) p->vz[i];

      sum += vx * vx + vy * vy + vz * vz;
    }

  return (dtype) (0.5L * (long double) mass * sum);
}

/*
 * Simple O(N^2) potential-energy diagnostic for the same softened potential used
 * by the force kernel.  Not performance critical if called only every
 * K steps, and keeping it independent of compute_accelerations_naive makes it a
 * useful correctness check during optimisation.
 */
static dtype potential_energy_naive (particles_t *p,        // particle positions are read-only
                                     dtype        g,        // gravitational constant
                                     dtype        eps       // softening length
				     )
{
  size_t        n    = p->n;
  dtype         eps2 = eps * eps;
  dtype         m2   = p->mass * p->mass;
  long double   sum  = 0.0L;
  size_t        i;
  size_t        j;

  for (i = 0u; i < n; ++i)
    {
      dtype  xi = p->x[i];
      dtype  yi = p->y[i];
      dtype  zi = p->z[i];

      for (j = i + 1u; j < n; ++j)
        {
          dtype  dx   = p->x[j] - xi;
          dtype  dy   = p->y[j] - yi;
          dtype  dz   = p->z[j] - zi;
          dtype  r2   = dx * dx + dy * dy + dz * dz + eps2;
          dtype  invr = (dtype) 1.0 / dtype_sqrt (r2);

          sum -= (long double) g * (long double) m2 * (long double) invr;
        }
    }

  return (dtype) sum;
}

/*
 * Total mechanical energy, returned together with kinetic and potential parts
 * for reporting.
 * The relative drift of this quantity is the main verification
 * metric 
 */
static dtype total_energy (particles_t *p,           // complete particle state, read-only
                           dtype        g,           // gravitational constant
                           dtype        eps,         // softening length
                           dtype       *kinetic,     // output kinetic energy
                           dtype       *potential    // output potential energy
			   )
{
  *kinetic   = kinetic_energy (p);
  *potential = potential_energy_naive (p, g, eps);

  return *kinetic + *potential;
}






/* ======================================================================================== */

/* : ------------------------------------------------------ :
   :  HELP & MAIN                                           :
   : ------------------------------------------------------ :
 */

/*
 * Print a compact command-line reference.
 * Defaults are chosen for > small test < runs
 */
static void print_usage (const char *program    // argv[0]
			 )
{
  fprintf (stderr,
           "usage: %s --input FILE [options]\n"
           "\n"
           "options:\n"
           "  --input FILE              input binary particle file (%s)\n"
           "  --output FILE             optional final-state binary file\n"
           "  --nsteps N                number of DKD steps (default: 10)\n"
           "  --dt X                    time step (default: 0.001)\n"
           "  --eps X                   softening length (default: 0.01)\n"
           "  --G X                     gravitational constant (default: 1)\n"
           "  --mass X                  particle mass (default: 1)\n"
           "  --energy-every N          diagnostic period in steps (default: 1)\n"
           "  --energy-tol X            warning tolerance for max relative drift (default: 1e-3)\n"
           "  --profiler [0|1]          enable per-phase wall-clock profiling (default: 0)\n"
           "  --profiler-path FILE      save profiling statistics to FILE (requires --profiler 1)\n"
           "  --quiet                   only print final summary\n"
           "  --help                    show this help message\n",
           program, NBODY_BINARY_VERSION_TEXT);
}


/* ======================================================================================== */


int main (int argc, char **argv)
{
  const double  t_program_start = get_time ();

  const char  *input_path    = NULL;
  const char  *output_path   = NULL;
  size_t       nsteps        = 10u;
  size_t       energy_every  = 1u;
  dtype        dt            = (dtype) 1.0e-3;
  dtype        eps           = (dtype) 1.0e-2;
  dtype        g             = (dtype) 1.0;
  dtype        mass          = (dtype) 1.0;
  dtype        energy_tol    = (dtype) 1.0e-3;
  bool         quiet         = false;
  bool         profiler_on   = false;
  const char  *profiler_path = NULL;
  particles_t  particles;
  profiler_t   profiler;
  dtype        kinetic0;
  dtype        potential0;
  dtype        energy0;


  // ·························································
  // allocate particles container to an empty state
  particles_init_empty (&particles);

  
  // ························································
  // parse CLI
  for (int argi = 1; argi < argc; ++argi)
    {
      const char *value;

      if ((value = option_value (&argi, argc, argv, "--input")) != NULL)
        input_path = value;
      else if ((value = option_value (&argi, argc, argv, "--output")) != NULL)
        output_path = value;
      else if ((value = option_value (&argi, argc, argv, "--nsteps")) != NULL)
        nsteps = parse_size (value, "--nsteps");
      else if ((value = option_value (&argi, argc, argv, "--energy-every")) != NULL)
        energy_every = parse_size (value, "--energy-every");
      else if ((value = option_value (&argi, argc, argv, "--dt")) != NULL)
        dt = parse_dtype (value, "--dt");
      else if ((value = option_value (&argi, argc, argv, "--eps")) != NULL)
        eps = parse_dtype (value, "--eps");
      else if ((value = option_value (&argi, argc, argv, "--G")) != NULL)
        g = parse_dtype (value, "--G");
      else if ((value = option_value (&argi, argc, argv, "--mass")) != NULL)
        mass = parse_dtype (value, "--mass");
      else if ((value = option_value (&argi, argc, argv, "--energy-tol")) != NULL)
        energy_tol = parse_dtype (value, "--energy-tol");
      else if ((value = option_value (&argi, argc, argv, "--profiler")) != NULL)
        profiler_on = (parse_size (value, "--profiler") != 0u);
      else if ((value = option_value (&argi, argc, argv, "--profiler-path")) != NULL)
        profiler_path = value;
      else if (strcmp (argv[argi], "--quiet") == 0)
        quiet = true;
      else if (strcmp (argv[argi], "--help") == 0)
        {
          print_usage (argv[0]);
          return EXIT_SUCCESS;
        }
      else
        {
          print_usage (argv[0]);
          die ("unknown option: %s", argv[argi]);
        }
    }

  if (input_path == NULL)
    {
      print_usage (argv[0]);
      die ("missing required --input FILE");
    }
  if (!(dt > (dtype) 0.0))
    die ("--dt must be positive");
  if (!(eps >= (dtype) 0.0))
    die ("--eps must be non-negative");
  if (!(g > (dtype) 0.0))
    die ("--G must be positive");
  if (!(mass > (dtype) 0.0))
    die ("--mass must be positive");
  if (energy_every == 0u)
    die ("--energy-every must be positive");
  if (!(energy_tol > (dtype) 0.0))
    die ("--energy-tol must be positive");


  // ························································
  // set up profiling, if requested
  if (profiler_on)
    profiler_allocate (&profiler, nsteps);

  // ························································
  // read particles from input file
  {
    double  t0 = profiler_on ? get_time () : 0.0;

    particles_read_binary (input_path, mass, &particles);
    if (profiler_on)
      profiler.reading_time = get_time () - t0;
  }

  // ························································
  // get energy baseline
  {
    double  t0 = profiler_on ? get_time () : 0.0;

    energy0 = total_energy (&particles, g, eps, &kinetic0, &potential0);
    if (profiler_on)
      profiler.initial_energy_time = get_time () - t0;
  }

  if (!quiet)
    {
      printf ("# serial direct N-body DKD rsqrt-newton3rd\n");
      printf ("# arithmetic_dtype=%s binary_storage=float32 format=%s\n",
              DTYPE_NAME, NBODY_BINARY_VERSION_TEXT);
      printf ("# N=%zu nsteps=%zu dt=%.17g eps=%.17g G=%.17g mass=%.17g\n",
              particles.n, nsteps, (double) dt, (double) eps,
              (double) g, (double) mass);
      printf ("# step time kinetic potential total rel_energy_drift\n");
      printf ("%zu %.17g %.17g %.17g %.17g %.17g\n",
              (size_t) 0u, 0.0, (double) kinetic0, (double) potential0,
              (double) energy0, 0.0);
    }


  // ························································
  // integration

  double max_rel_drift = 0.0;
  
  for (size_t step = 1u; step <= nsteps; ++step)
    {
      double  step_t0 = profiler_on ? get_time () : 0.0;

      leapfrog_dkd_step (&particles, g, eps, dt,
                         profiler_on ? &profiler : NULL, step - 1u);

      if (profiler_on)
        profiler.total_step_time[step - 1u] = get_time () - step_t0;

      // once in a while, get diagnostics
      //
      if (((step % energy_every) == 0u) || (step == nsteps))
        {
          dtype         kinetic;
          dtype         potential;
          const dtype   energy = total_energy (&particles, g, eps, &kinetic, &potential);
          const double  denom  = fmax (fabs ((double) energy0), (double) DTYPE_MIN_NORMAL);
          const double  rel    = fabs ((double) (energy - energy0)) / denom;

          if (rel > max_rel_drift)
            max_rel_drift = rel;
          if (!quiet)
            printf ("%zu %.17g %.17g %.17g %.17g %.17g\n",
                    step, (double) step * (double) dt, (double) kinetic,
                    (double) potential, (double) energy, rel);
        }
    }

  // ························································
  // write final file

  if (output_path != NULL)
    {
      double  t0 = profiler_on ? get_time () : 0.0;

      particles_write_binary (output_path, &particles);
      if (profiler_on)
        profiler.writing_time = get_time () - t0;
    }

  // ························································
  // say good-bye

  printf ("# final: N=%zu steps=%zu arithmetic_dtype=%s max_relative_energy_drift=%.17g tolerance=%.17g status=%s\n",
          particles.n, nsteps, DTYPE_NAME, max_rel_drift, (double) energy_tol,
          (max_rel_drift <= (double) energy_tol) ? "OK" : "WARNING");

  if (max_rel_drift > (double) energy_tol)
    fprintf (stderr,
             "warning: relative energy drift %.6e exceeds tolerance %.6e; "
             "try smaller --dt, larger --eps, or better initial conditions\n",
             max_rel_drift, (double) energy_tol);


  // ························································
  // profiling report

  if (profiler_on)
    {
      profiler.total_run_time = get_time () - t_program_start;

      print_statistics (&profiler);

      if (profiler_path != NULL)
        {
          profiler_config_t  config;

          config.variant_name              = "rsqrt-newton3rd";
          config.n_particles                = particles.n;
          config.n_steps                    = nsteps;
          config.dt                         = (double) dt;
          config.eps                        = (double) eps;
          config.g                          = (double) g;
          config.mass                       = (double) mass;
          config.max_relative_energy_drift = max_rel_drift;
          config.energy_tolerance           = (double) energy_tol;

          save_statistics (profiler_path, &config, &profiler);
        }

      profiler_free (&profiler);
    }


  // ························································
  // don't leave garbage behind you

  particles_free (&particles);


  return EXIT_SUCCESS;
}

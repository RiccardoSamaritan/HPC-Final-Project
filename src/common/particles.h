#ifndef NBODY_PARTICLES_H
#define NBODY_PARTICLES_H

#include "nbody_common.h"

typedef struct particles_s
{
  size_t  n;
  dtype   mass;
  dtype  *x;
  dtype  *y;
  dtype  *z;
  dtype  *vx;
  dtype  *vy;
  dtype  *vz;
  dtype  *ax;
  dtype  *ay;
  dtype  *az;
} particles_t;

/*
 * Initialise an empty particle container.  This function does not allocate; it
 * simply gives every pointer a known value so that particles_free can safely be
 * called after a partial failure path.
 */
void particles_init_empty (particles_t *p    // particle container to initialise
			    );

/*
 * Allocate the SoA storage used by the solver.  Positions, velocities, and
 * accelerations are separate arrays, not an array of structs, because the
 * direct kernel only needs streams of x/y/z coordinates and accumulators.  This
 * layout is also the natural one for later SIMD and MPI ring-buffer work.
 */
void particles_allocate (particles_t  *p,       // output container
                         size_t        n,       // number of particles
                         dtype         mass     // mass of each particle
			 );

/*
 * Release all particle arrays and return the container to the empty state.  No
 * simulation data survive this call.
 */
void particles_free (particles_t *p    // container to release
		     );

/*
 * Load particle coordinates and velocities from the binary file.
 * The on-disk values are single precision, then converted to dtype so the same
 * initial-condition file can be used for both float and double solver builds.
 * Acceleration arrays are left uninitialised because every force evaluation
 * overwrites them.
 */
void particles_read_binary (const char  *path,       // input file path
                            dtype        mass,       // mass assigned to each particle
                            particles_t *p           // output particle container
			    );

/*
 * Write the current particle state in the same binary format accepted by the
 * reader.  Conversion to single precision is done explicitly record by record;
 * this is simple rather than maximally fast;
 * check in the code for initial condition generator
 */
void particles_write_binary (const char        *path,       // output file path
                             const particles_t *p           // particle state to write
			     );

#endif

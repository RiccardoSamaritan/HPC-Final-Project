#ifndef NBODY_UTILS_H
#define NBODY_UTILS_H

#include <stddef.h>
#include <stdio.h>

void die (const char *format, ...);

/*
 * Allocate a cache-line aligned block.  Alignment is not required for scalar
 * correctness, but it makes the serial skeleton a better starting point for
 * vectorisation and OpenMP first-touch experiments.
 */
void *checked_aligned_alloc (size_t  nbytes,      // requested useful bytes
                             size_t  alignment    // power-of-two alignment
			     );

/*
 * Read exactly nmemb items from a binary stream.  Centralising the check avoids
 * partial binary records being mistaken for valid particles, which is otherwise
 * easy to do when replacing a line-oriented ASCII reader with fread.
 */
void checked_fread (void       *ptr,       // destination buffer
                    size_t      size,      // item size in bytes
                    size_t      nmemb,     // number of items expected
                    FILE       *fp,        // open input stream
                    const char *path,      // file name for diagnostics
                    const char *what       // logical record name
		    );

/*
 * Write exactly nmemb items to a binary stream.  All output paths go through
 * this helper so that disk-full and permission errors are reported at the point
 * where the data loss happens, not later in a benchmark script.
 */
void checked_fwrite (const void *ptr,       // source buffer
                     size_t      size,      // item size in bytes
                     size_t      nmemb,     // number of items to write
                     FILE       *fp,        // open output stream
                     const char *path,      // file name for diagnostics
                     const char *what       // logical record name
		     );

#endif

#ifndef NBODY_UTILS_H
#define NBODY_UTILS_H

#include "nbody_common.h"

#include <stddef.h>
#include <stdio.h>

void die (const char *format, ...);

/*
 * Parse a size_t command-line value.  All user-facing quantities that count
 * particles or steps pass through this function so that overflow and malformed
 * input fail early, before any allocation or simulation state is modified.
 */
size_t parse_size (const char *text,     // decimal text to parse
                   const char *name      // option name used in errors
		   );

/*
 * Parse a finite floating-point command-line value and cast it to dtype.  The
 * parser reads through double because strtof and strtod differ only in final
 * rounding for the ranges used here; the explicit range check keeps float-mode
 * builds from silently accepting values that cannot be represented by dtype.
 */
dtype parse_dtype (const char *text,     // decimal text to parse
                   const char *name      // option name used in errors
		   );

/*
 * Return the value associated with either "--key value" or "--key=value".
 * The caller passes the loop index by address so that the separated-value
 * form consumes the following argv entry exactly once.
 */
const char *option_value (int        *i,       // current argv index, updated on success
                          int         argc,    // argc from main
                          char      **argv,    // argv from main
                          const char *key      // long option name, including "--"
			  );

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

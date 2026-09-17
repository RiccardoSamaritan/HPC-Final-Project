#include "utils.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

void die (const char *format, ...)
{
  va_list  args;

  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);
  fputc ('\n', stderr);
  exit (EXIT_FAILURE);
}

void *checked_aligned_alloc (size_t  nbytes,      // requested useful bytes
                             size_t  alignment    // power-of-two alignment
			     )
{
  void   *ptr;
  size_t  padded;

  if (nbytes == 0u)
    die ("attempted zero-byte allocation");
  if (alignment == 0u)
    die ("invalid zero alignment");
  if (nbytes > SIZE_MAX - alignment)
    die ("allocation size overflow");

  padded = ((nbytes + alignment - 1u) / alignment) * alignment;
  ptr = aligned_alloc (alignment, padded);
  if (ptr == NULL)
    die ("aligned_alloc failed for %zu bytes", padded);

  return ptr;
}

void checked_fread (void       *ptr,       // destination buffer
                    size_t      size,      // item size in bytes
                    size_t      nmemb,     // number of items expected
                    FILE       *fp,        // open input stream
                    const char *path,      // file name for diagnostics
                    const char *what       // logical record name
		    )
{
  const size_t  got = fread (ptr, size, nmemb, fp);

  if (got != nmemb)
    {
      if (ferror (fp))
        die ("read error while reading %s from '%s'", what, path);
      die ("short file while reading %s from '%s'", what, path);
    }
}

void checked_fwrite (const void *ptr,       // source buffer
                     size_t      size,      // item size in bytes
                     size_t      nmemb,     // number of items to write
                     FILE       *fp,        // open output stream
                     const char *path,      // file name for diagnostics
                     const char *what       // logical record name
		     )
{
  const size_t  written = fwrite (ptr, size, nmemb, fp);

  if (written != nmemb)
    die ("write error while writing %s to '%s'", what, path);
}

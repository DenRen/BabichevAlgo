#include <stdio.h>
#include <string.h>

#include "debug_func.h"

// ==============\\
// Print function -------------------------------------------------------------
// ==============//

void
print_error_line (const char strerr[],
                  const char name_file[],
                  unsigned line) {
    fprintf (stderr, "Failed in %s, error: %s\nLINE: %s: %d\n",
                     strerr, strerror (errno), name_file, line);
}
// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

void *reallocarray(void *ptr, size_t nelem, size_t elsize) {
  // Check for overflow of nelem * elsize.
  if (nelem > 0 && elsize > SIZE_MAX / nelem) {
    errno = ENOMEM;
    return NULL;
  }
  return realloc(ptr, nelem * elsize);
}
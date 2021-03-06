// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <stdlib.h>

int rand(void) {
  return arc4random_uniform((uint32_t)RAND_MAX + 1);
}

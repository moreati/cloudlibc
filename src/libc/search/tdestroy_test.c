// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stddef.h>
#include <testing.h>

static int compar(const void *a, const void *b) {
  return (char *)a - (char *)b;
}

static void destructor(void *keyp) {
  bool *key = keyp;
  ASSERT_FALSE(*key);
  *key = true;
}

TEST(tdestroy, example) {
#define NKEYS 1000
  // Insert a thousand keys.
  bool keys[1000] = {};
  void *root = NULL;
  for (size_t i = 0; i < NKEYS; ++i)
    ASSERT_EQ(&keys[i], *(bool **)tsearch(&keys[i], &root, compar));

  // All of them should be destroyed if we call tdestroy().
  tdestroy(root, destructor);
  for (size_t i = 0; i < NKEYS; ++i)
    ASSERT_TRUE(keys[i]);
}

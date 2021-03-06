// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <pthread.h>

int pthread_key_delete(pthread_key_t key) {
  // Increase generation count to prevent destructors from being called.
  ++key.__key->generation;

  // Place the key object back in the freelist.
  pthread_mutex_lock(&__pthread_key_freelist_lock);
  SLIST_INSERT_HEAD(&__pthread_key_freelist, key.__key, freelist);
  pthread_mutex_unlock(&__pthread_key_freelist_lock);
  return 0;
}

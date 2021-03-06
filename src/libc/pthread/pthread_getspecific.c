// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <pthread.h>
#include <stdlib.h>
#include <threads.h>

void *pthread_getspecific(pthread_key_t key) {
  struct pthread_specific *specific, **specific_prev;
  SLIST_FOREACH_PREVPTR(specific, specific_prev, &__pthread_specifics, next) {
    if (specific->generation != specific->key->generation) {
      // Garbage collect entries whose key has been destroyed in the meantime.
      SLIST_REMOVE_PREVPTR(specific_prev, next);
      free(specific);
    } else if (specific->key == key.__key) {
      // Found a matching value. Move the entry to the head of the list
      // to speed up future requests.
      SLIST_REMOVE_PREVPTR(specific_prev, next);
      SLIST_INSERT_HEAD(&__pthread_specifics, specific, next);
      return specific->value;
    }
  }
  return NULL;
}

__strong_reference(pthread_getspecific, tss_get);

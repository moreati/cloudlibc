// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <cloudabi_syscalls.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_cond_signal(pthread_cond_t *cond) {
  if (atomic_load_explicit(&cond->__waiters, memory_order_relaxed) !=
      CLOUDABI_CONDVAR_HAS_NO_WAITERS) {
    cloudabi_errno_t error =
        cloudabi_sys_condvar_signal(&cond->__waiters, cond->__pshared, 1);
    if (error != 0)
      __pthread_terminate(error, "Failed to broadcast on condition variable");
  }
  return 0;
}

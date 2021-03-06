// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/select.h>

#include <errno.h>
#include <stddef.h>

int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds,
           fd_set *restrict errorfds, struct timeval *restrict timeout) {
  if (timeout != NULL) {
    // Timeout specified.
    if (timeout->tv_usec < 0 || timeout->tv_usec >= 1000000) {
      errno = EINVAL;
      return -1;
    }
    struct timespec ts = {.tv_sec = timeout->tv_sec,
                          .tv_nsec = (long)timeout->tv_usec * 1000};
    return pselect(nfds, readfds, writefds, errorfds, &ts);
  } else {
    // No timeout specified.
    return pselect(nfds, readfds, writefds, errorfds, NULL);
  }
}

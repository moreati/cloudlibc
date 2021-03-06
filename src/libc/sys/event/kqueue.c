// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/event.h>

#include <cloudabi_syscalls.h>
#include <errno.h>

int kqueue(void) {
  cloudabi_fd_t fd;
  cloudabi_errno_t error = cloudabi_sys_fd_create1(CLOUDABI_FILETYPE_POLL, &fd);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return fd;
}

// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>

int fcntl(int fildes, int cmd, ...) {
  switch (cmd) {
    case F_GETFD:
      // Act as if the close-on-exec flag is always set.
      return FD_CLOEXEC;
    case F_SETFD:
      // The close-on-exec flag is ignored.
      return 0;
    case F_GETFL: {
      // Obtain the flags and the rights of the descriptor.
      cloudabi_fdstat_t fds;
      cloudabi_errno_t error = cloudabi_sys_fd_stat_get(fildes, &fds);
      if (error != 0) {
        errno = error;
        return -1;
      }

      // Roughly approximate the access mode by converting the rights.
      int oflags = fds.fs_flags;
      if (fds.fs_rights_base &
          (CLOUDABI_RIGHT_FD_READ | CLOUDABI_RIGHT_FD_WRITE)) {
        if ((fds.fs_rights_base & CLOUDABI_RIGHT_FD_READ) != 0)
          oflags |= O_RDONLY;
        if ((fds.fs_rights_base & CLOUDABI_RIGHT_FD_WRITE) != 0)
          oflags |= O_WRONLY;
      } else if ((fds.fs_rights_base & CLOUDABI_RIGHT_PROC_EXEC) != 0) {
        oflags |= O_EXEC;
      } else {
        oflags |= O_SEARCH;
      }
      return oflags;
    }
    case F_SETFL: {
      // Set new file descriptor flags.
      va_list ap;
      va_start(ap, cmd);
      int flags = va_arg(ap, int);
      va_end(ap);

      cloudabi_fdstat_t fds = {.fs_flags = flags & 0xfff};
      cloudabi_errno_t error =
          cloudabi_sys_fd_stat_put(fildes, &fds, CLOUDABI_FDSTAT_FLAGS);
      if (error != 0) {
        errno = error;
        return -1;
      }
      return 0;
    }
    default:
      errno = EINVAL;
      return -1;
  }
}

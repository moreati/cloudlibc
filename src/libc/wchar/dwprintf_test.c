// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <testing.h>
#include <unistd.h>
#include <wchar.h>

TEST_SEPARATE_PROCESS(dwprintf, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, dwprintf(-1, L"Hello, world\n"));
  ASSERT_EQ(EBADF, errno);

  // Failure to convert to ASCII.
  ASSERT_EQ(-1, dwprintf(-1, L"%ls\n", L"ï"));
  ASSERT_EQ(EILSEQ, errno);

  // Write to descriptor until it would block.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[1], F_SETFL, O_NONBLOCK));
  ASSERT_EQ(-1, dwprintf(fds[1], L"%1000000000s", ""));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Other side of the pipe has been closed.
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(-1, dwprintf(fds[1], L"Hello"));
  ASSERT_EQ(EPIPE, errno);
  ASSERT_EQ(0, close(fds[1]));
}

TEST(dwprintf, example) {
  // Write a string into a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(12, dwprintf_l(fds[1], LC_C_UNICODE_LOCALE, L"%ls, %s", L"H€llø",
                           "world"));
  ASSERT_EQ(0, close(fds[1]));

  // Read it back again.
  char buf[16];
  ASSERT_EQ(15, read(fds[0], buf, sizeof(buf)));
  ASSERT_ARREQ("H€llø, world", buf, 15);
  ASSERT_EQ(0, close(fds[0]));
}

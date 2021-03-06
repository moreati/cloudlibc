// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdio.h>
#include <testing.h>
#include <unistd.h>

TEST(ftello, espipe) {
  // Create a stream to read side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[0], "r");
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(0, close(fds[1]));

  // This stream has no offset.
  ASSERT_EQ(-1, ftello(fp));
  ASSERT_FALSE(ferror(fp));
  ASSERT_EQ(ESPIPE, errno);

  ASSERT_EQ(0, fclose(fp));
}

TEST(ftello, success) {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);
  ASSERT_EQ(0, ftello(fp));
  ASSERT_EQ(0, fclose(fp));
}

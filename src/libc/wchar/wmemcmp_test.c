// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wmemcmp, null) {
  ASSERT_EQ(0, wmemcmp(NULL, NULL, 0));
}

TEST(wmemcmp, example) {
  const wchar_t buf1[] = L"Hello";
  const wchar_t buf2[] = L"Helxo";
  ASSERT_EQ(0, wmemcmp(buf1, buf1, __arraycount(buf1)));
  ASSERT_GT(0, wmemcmp(buf1, buf2, __arraycount(buf1)));
  ASSERT_LT(0, wmemcmp(buf2, buf1, __arraycount(buf1)));
}

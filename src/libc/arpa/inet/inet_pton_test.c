// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <arpa/inet.h>

#include <stddef.h>
#include <testing.h>

TEST(inet_pton, inet_valid) {
  char addr[4];
#define TEST_INET(string, binary)                  \
  ASSERT_EQ(1, inet_pton(AF_INET, string, &addr)); \
  ASSERT_ARREQ(binary, addr, __arraycount(addr));
  TEST_INET("0.0.0.0", "\x00\x00\x00\x00");
  TEST_INET("192.168.003.004", "\xc0\xa8\x03\x04");
  TEST_INET("255.255.255.255", "\xff\xff\xff\xff");
#undef TEST_INET
}

TEST(inet_pton, inet_invalid) {
  ASSERT_EQ(0, inet_pton(AF_INET, "256.256.256.256", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "10.0.0.1hello", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "10.0.0.", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "hi10.0.0.5", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "0000.0.0.0", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "0.0.0.0.0", NULL));
}

TEST(inet_pton, inet6_valid) {
  char addr[16];
#define TEST_INET6(string, binary)                  \
  ASSERT_EQ(1, inet_pton(AF_INET6, string, &addr)); \
  ASSERT_ARREQ(binary, addr, __arraycount(addr));
  TEST_INET6(
      "1080::8:800:200c:417a",
      "\x10\x80\x00\x00\x00\x00\x00\x00\x00\x08\x08\x00\x20\x0c\x41\x7a");
  TEST_INET6(
      "ff01::101",
      "\xff\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x01");
  TEST_INET6(
      "::1",
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01");
  TEST_INET6(
      "::", "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  TEST_INET6(
      "2001:DB8:0:1:1:1:1:1",
      "\x20\x01\x0d\xb8\x00\x00\x00\x01\x00\x01\x00\x01\x00\x01\x00\x01");
  TEST_INET6(
      "::13.1.68.3",
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0d\x01\x44\x03");
  TEST_INET6(
      "0:0:0:0::13.1.68.3",
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0d\x01\x44\x03");
  TEST_INET6(
      "0:0:0:0:0:0:13.1.68.3",
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0d\x01\x44\x03");
  TEST_INET6(
      "::ffff:129.144.52.38",
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x81\x90\x34\x26");
  TEST_INET6(
      "2001:db8:1:1:1:1::",
      "\x20\x01\x0d\xb8\x00\x01\x00\x01\x00\x01\x00\x01\x00\x00\x00\x00");
#undef TEST_INET6
}

TEST(inet_pton, inet6_invalid) {
  ASSERT_EQ(0, inet_pton(AF_INET6, "", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, ":::", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, ":1:2:3:4:5:6:7", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "1:2:3:4:5:6:7:", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "1::2::3", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "0:0:0:0:0::13.1.68.3", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "0:0:0:0:0:0::13.1.68.3", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "2001:DB8::1:1:1:1:1", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "2001:db8:1:1:1:1:1:", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "2001:db8:1:1:1:1:1::", NULL));
}

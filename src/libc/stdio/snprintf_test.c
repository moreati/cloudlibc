// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <fenv.h>
#include <float.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <testing.h>

TEST(snprintf, null) {
  ASSERT_EQ(12, snprintf(NULL, 0, "Hello, world"));
}

TEST(snprintf, null_string) {
  char buf[14];
  char *ptr = NULL;
  ASSERT_EQ(13, snprintf(buf, sizeof(buf), "Hello%.3sworld", ptr));
  ASSERT_STREQ("Hello(nuworld", buf);
}

TEST(snprintf, truncation) {
  char buf[10];
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "Hello, world"));
  ASSERT_STREQ("Hello, wo", buf);
}

TEST(snprintf, number_width_precision_left) {
  char buf[30];
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%20.10d", 5));
  ASSERT_STREQ("          0000000005", buf);
}

TEST(snprintf, number_width_precision_right) {
  char buf[30];
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%-20.10d", 5));
  ASSERT_STREQ("0000000005          ", buf);
}

TEST(snprintf, grouping_en_us) {
  locale_t locale = newlocale(LC_NUMERIC_MASK, "en_US", 0);
  ASSERT_NE(0, locale);
  char buf[21];
  ASSERT_EQ(20, snprintf_l(buf, sizeof(buf), locale, "%'020d", 12345678));
  ASSERT_STREQ("000000000012,345,678", buf);
  freelocale(locale);
}

#if 0
TEST(snprintf, grouping_ru_ru) {
  locale_t locale = newlocale(LC_NUMERIC_MASK, "ru_RU", 0);
  ASSERT_NE(0, locale);
  char buf[21];
  ASSERT_EQ(20, snprintf_l(buf, sizeof(buf), locale, "%'020d", -123456789));
  ASSERT_STREQ("-00000000123 456 789", buf);
  freelocale(locale);
}
#endif

TEST(snprintf, numbered_arguments) {
  char buf[4];
  ASSERT_EQ(3, snprintf(buf, sizeof(buf), "%2$d %1$d", 5, 7));
  ASSERT_STREQ("7 5", buf);
}

TEST(snprintf, hex_left) {
  char buf[21];
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%-#20.10x", 0x1337));
  ASSERT_STREQ("0x0000001337        ", buf);
}

TEST(snprintf, hex_right) {
  char buf[21];
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%#20.10x", 0x1337));
  ASSERT_STREQ("        0x0000001337", buf);
}

TEST(snprintf, octal1) {
  char buf[2];
  ASSERT_EQ(1, snprintf(buf, sizeof(buf), "%o", 1));
  ASSERT_STREQ("1", buf);
}

TEST(snprintf, octal2) {
  char buf[3];
  ASSERT_EQ(2, snprintf(buf, sizeof(buf), "%#o", 1));
  ASSERT_STREQ("01", buf);
}

TEST(snprintf, float10_e_zero) {
  char buf[13];
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%e", 0.0));
  ASSERT_STREQ("0.000000e+00", buf);
  ASSERT_EQ(5, snprintf(buf, sizeof(buf), "%.0e", 0.0));
  ASSERT_STREQ("0e+00", buf);
  ASSERT_EQ(6, snprintf(buf, sizeof(buf), "%#.0e", 0.0));
  ASSERT_STREQ("0.e+00", buf);
  ASSERT_EQ(6, snprintf(buf, sizeof(buf), "% .0e", 0.0));
  ASSERT_STREQ(" 0e+00", buf);
  ASSERT_EQ(6, snprintf(buf, sizeof(buf), "%+.0e", 0.0));
  ASSERT_STREQ("+0e+00", buf);
}

TEST(snprintf, float10_e_pi) {
  char buf[107];
  ASSERT_EQ(5, snprintf(buf, sizeof(buf), "%.0e", M_PI));
  ASSERT_STREQ("3e+00", buf);
  ASSERT_EQ(7, snprintf(buf, sizeof(buf), "%.1e", M_PI));
  ASSERT_STREQ("3.1e+00", buf);
  ASSERT_EQ(8, snprintf(buf, sizeof(buf), "%.2e", M_PI));
  ASSERT_STREQ("3.14e+00", buf);
  ASSERT_EQ(9, snprintf(buf, sizeof(buf), "%.3e", M_PI));
  ASSERT_STREQ("3.142e+00", buf);
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%.4e", M_PI));
  ASSERT_STREQ("3.1416e+00", buf);
  ASSERT_EQ(11, snprintf(buf, sizeof(buf), "%.5e", M_PI));
  ASSERT_STREQ("3.14159e+00", buf);
  ASSERT_EQ(12, snprintf(buf, sizeof(buf), "%.6e", M_PI));
  ASSERT_STREQ("3.141593e+00", buf);
  ASSERT_EQ(13, snprintf(buf, sizeof(buf), "%.7e", M_PI));
  ASSERT_STREQ("3.1415927e+00", buf);
  ASSERT_EQ(14, snprintf(buf, sizeof(buf), "%.8e", M_PI));
  ASSERT_STREQ("3.14159265e+00", buf);

#if DBL_MANT_DIG == 53
  // This implementation already starts to return zeroes as soon as the
  // digits uniquely correspond with the floating point value.
  ASSERT_EQ(106, snprintf(buf, sizeof(buf), "%.100e", M_PI));
  ASSERT_STREQ(
      "3."
      "141592653589793100000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000e+00",
      buf);
#endif
}

TEST(snprintf, float16_nan) {
  char buf[11];
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%-10a", -NAN));
  ASSERT_STREQ("-nan      ", buf);
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%10A", NAN));
  ASSERT_STREQ("       NAN", buf);
}

TEST(snprintf, float16_inf) {
  char buf[11];
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%10a", INFINITY));
  ASSERT_STREQ("       inf", buf);
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%-10A", -INFINITY));
  ASSERT_STREQ("-INF      ", buf);
}

TEST(snprintf, float16_simple_zero) {
  char buf[8];
  ASSERT_EQ(6, snprintf(buf, sizeof(buf), "%a", 0.0));
  ASSERT_STREQ("0x0p+0", buf);
  ASSERT_EQ(7, snprintf(buf, sizeof(buf), "%#a", 0.0));
  ASSERT_STREQ("0x0.p+0", buf);
}

TEST(snprintf, float16_simple_one) {
  char buf[8];
  ASSERT_EQ(6, snprintf(buf, sizeof(buf), "%a", 1.0));
  ASSERT_STREQ("0x1p+0", buf);
  ASSERT_EQ(7, snprintf(buf, sizeof(buf), "%#a", 1.0));
  ASSERT_STREQ("0x1.p+0", buf);
}

TEST(snprintf, float16_simple_number) {
  char buf[16];
  ASSERT_EQ(15, snprintf(buf, sizeof(buf), "%a", 0x1234.5678p12));
  ASSERT_STREQ("0x1.2345678p+24", buf);
}

TEST(snprintf, float16_negative) {
  char buf[17];
  ASSERT_EQ(16, snprintf(buf, sizeof(buf), "%a", -0x1234.5678p12));
  ASSERT_STREQ("-0x1.2345678p+24", buf);
}

TEST(snprintf, float16_round) {
  char buf[21];
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%.2a %.2a", 1.51, -1.51));
  ASSERT_STREQ("0x1.82p+0 -0x1.83p+0", buf);
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%.2a %.2a", 1.51, -1.51));
  ASSERT_STREQ("0x1.83p+0 -0x1.83p+0", buf);
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%.2a %.2a", 1.51, -1.51));
  ASSERT_STREQ("0x1.82p+0 -0x1.82p+0", buf);
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(20, snprintf(buf, sizeof(buf), "%.2a %.2a", 1.51, -1.51));
  ASSERT_STREQ("0x1.83p+0 -0x1.82p+0", buf);
}

TEST(snprintf, float16_align) {
  char buf[31];
  ASSERT_EQ(30, snprintf(buf, sizeof(buf), "%#30.15a", 0x12.abcp12));
  ASSERT_STREQ("       0x1.2abc00000000000p+16", buf);
  ASSERT_EQ(30, snprintf(buf, sizeof(buf), "%#-30.15A", 0x12.abcp12));
  ASSERT_STREQ("0X1.2ABC00000000000P+16       ", buf);
  ASSERT_EQ(30, snprintf(buf, sizeof(buf), "%#030.15a", 0x12.abcp12));
  ASSERT_STREQ("0x00000001.2abc00000000000p+16", buf);
  ASSERT_EQ(30, snprintf(buf, sizeof(buf), "%+#030.15a", 0x12.abcp12));
  ASSERT_STREQ("+0x0000001.2abc00000000000p+16", buf);
  ASSERT_EQ(30, snprintf(buf, sizeof(buf), "% #030.15a", 0x12.abcp12));
  ASSERT_STREQ(" 0x0000001.2abc00000000000p+16", buf);
}

#if LDBL_HAS_SUBNORM == 1
TEST(snprintf, float16_subnormal) {
#if LDBL_MANT_DIG == 64
  // Print subnormal values.
  char buf[29];
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%La", LDBL_TRUE_MIN));
  ASSERT_STREQ("0x1p-16445", buf);
  long double high = nexttowardl(LDBL_MIN, 0.0L);
  ASSERT_EQ(27, snprintf(buf, sizeof(buf), "%La", high));
  ASSERT_STREQ("0x1.fffffffffffffffcp-16383", buf);
#elif LDBL_MANT_DIG == 113
  char buf[40];
  ASSERT_EQ(10, snprintf(buf, sizeof(buf), "%La", LDBL_TRUE_MIN));
  ASSERT_STREQ("0x1p-16494", buf);
  long double high = nexttowardl(LDBL_MIN, 0.0L);
  ASSERT_EQ(39, snprintf(buf, sizeof(buf), "%La", high));
  ASSERT_STREQ("0x1.fffffffffffffffffffffffffffep-16383", buf);
#else
#error "Unknown floating point type"
#endif

  // Test rounding behaviour of subnormal values.
  ASSERT_EQ(0, fesetround(FE_DOWNWARD));
  ASSERT_EQ(28, snprintf(buf, sizeof(buf), "%.2La %.2La", high, -high));
  ASSERT_STREQ("0x1.ffp-16383 -0x1.00p-16382", buf);
  ASSERT_EQ(0, fesetround(FE_TONEAREST));
  ASSERT_EQ(28, snprintf(buf, sizeof(buf), "%.2La %.2La", high, -high));
  ASSERT_STREQ("0x1.00p-16382 -0x1.00p-16382", buf);
  ASSERT_EQ(0, fesetround(FE_TOWARDZERO));
  ASSERT_EQ(28, snprintf(buf, sizeof(buf), "%.2La %.2La", high, -high));
  ASSERT_STREQ("0x1.ffp-16383 -0x1.ffp-16383", buf);
  ASSERT_EQ(0, fesetround(FE_UPWARD));
  ASSERT_EQ(28, snprintf(buf, sizeof(buf), "%.2La %.2La", high, -high));
  ASSERT_STREQ("0x1.00p-16382 -0x1.ffp-16383", buf);
}
#endif

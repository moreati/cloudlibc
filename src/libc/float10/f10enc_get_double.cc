// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float10.h>

#include <double-conversion/strtod.h>

double __f10enc_get_double(const char *str, size_t len, int exponent) {
  return double_conversion::Strtod(
      double_conversion::Vector<const char>(str, len), exponent);
}

// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wchar_t *(wcsrchr)(const wchar_t *ws, wchar_t wc) {
  wchar_t *last = NULL;
  for (;;) {
    if (*ws == wc)
      last = (wchar_t *)ws;
    if (*ws++ == L'\0')
      return last;
  }
}

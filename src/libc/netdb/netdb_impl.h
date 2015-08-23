// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef NETDB_NETDB_IMPL_H
#define NETDB_NETDB_IMPL_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Port number entries.
extern const char __iana_port_numbers[];

// Routines for parsing port number entries.

// Returns the port number in network byte order.
static inline int portstr_get_port(const char *entry) {
  union {
    uint8_t bytes[2];
    uint16_t number;
  } v = {.bytes = {entry[0], entry[1]}};
  return v.number;
}

// Tests whether the entry applies to the protocol used.
static inline bool portstr_match_protoid(const char *entry, uint8_t protoid) {
  return (entry[2] & protoid) != 0;
}

// Returns the name stored in the entry.
static inline const char *portstr_get_name(const char *entry) {
  return entry + 3;
}

// Returns a reference to the next entry.
static inline const char *portstr_get_next(const char *entry) {
  const char *name = portstr_get_name(entry);
  return name + strlen(name) + 1;
}

#endif
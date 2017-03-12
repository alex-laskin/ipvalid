
/*
 * Copyright (c) 2017 Alex Laskin <alex@lask.in>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "ipvalid.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern const char *__progname;

static void usage(void) {
  fprintf(stderr, "Usage: %s [OPTIONS]\n", __progname);
  fprintf(stderr, "Version: %s\n", PACKAGE_STRING);
  fprintf(stderr, "\n");
  fprintf(stderr, " -4 ADDR, --ipv4=ADDR         treat ADDR as IPv4\n");
  fprintf(stderr, " -6 ADDR, --ipv6=ADDR         treat ADDR as IPv6\n");
  fprintf(stderr, " -h, --help         display help and exit\n");
  fprintf(stderr, " -v, --version      print version and exit\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "see manual page " PACKAGE "(1) for more information\n");
}

const char *validate(int af, const char *src, char *dst, socklen_t size) {
  char buf[16];

  switch (inet_pton(af, src, buf)) {
  case 1:
    return inet_ntop(af, buf, dst, size);
  case 0:
  case -1:
    return NULL;
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  int ch;
  int af = 0;
  char src[INET6_ADDRSTRLEN];
  char dst[INET6_ADDRSTRLEN];

  static struct option long_options[] = {
    {"ipv4", required_argument, 0, '4'},
    {"ipv6", required_argument, 0, '6'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {0}
  };
  while (1) {
    int option_index = 0;

    ch = getopt_long(argc, argv, "4:6:hv", long_options, &option_index);
    if (ch == -1)
      break;
    switch (ch) {
    case '4':
      af = AF_INET;
      strncpy(src, optarg, INET_ADDRSTRLEN);
      break;
    case '6':
      af = AF_INET6;
      strncpy(src, optarg, INET6_ADDRSTRLEN);
      break;
    case 'h':
      usage();
      exit(0);
    case 'v':
      fprintf(stdout, "%s\n", PACKAGE_VERSION);
      exit(0);
    default:
      fprintf(stderr, "unknown option `%c'\n", ch);
      usage();
      exit(1);
    }
  }

  if (af && validate(af, src, dst, INET6_ADDRSTRLEN)) {
    fprintf(stdout, "%s\n", dst);
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

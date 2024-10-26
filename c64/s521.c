
// ECDSA Implementation for curve P-521
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
// python curve.py 64 NIST521
// This completes weierstrass.c for this curve. 

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "nist521curve.h"   // elliptic curve API

/*** Insert automatically generated code for P-521 prime group order group.c here ***/
/* Note that much of this code is not needed and can be deleted */


// Command line : python monty.py 64 nist521

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 9
#define Radix 59
#define Nbits 521
#define Nbytes 66

#define MONTGOMERY
#define NIST521

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 59u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 59u;
  n[0] &= mask;
  for (i = 1; i < 8; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 59u;
  }
  n[8] += (spint)carry;
  return -((n[8] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x36fb71e91386409u) & carry;
  n[1] += ((spint)0x6b937113388f5d7u) & carry;
  n[2] += ((spint)0x523dc269740eeu) & carry;
  n[3] += ((spint)0x3c1df97cb35bfe6u) & carry;
  n[4] += ((spint)0x7ffffffffa51868u) & carry;
  n[5] -= (spint)1u & carry;
  n[8] += ((spint)0x2000000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] -= (spint)0x36fb71e91386409u;
  n[1] -= (spint)0x6b937113388f5d7u;
  n[2] -= (spint)0x523dc269740eeu;
  n[3] -= (spint)0x3c1df97cb35bfe6u;
  n[4] -= (spint)0x7ffffffffa51868u;
  n[5] += (spint)1u;
  n[8] -= (spint)0x2000000000000u;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static void inline modadd(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[5] = a[5] + b[5];
  n[6] = a[6] + b[6];
  n[7] = a[7] + b[7];
  n[8] = a[8] + b[8];
  n[0] -= (spint)0x6df6e3d2270c812u;
  n[1] -= (spint)0xd726e226711ebaeu;
  n[2] -= (spint)0xa47b84d2e81dcu;
  n[3] -= (spint)0x783bf2f966b7fccu;
  n[4] -= (spint)0xfffffffff4a30d0u;
  n[5] += (spint)2u;
  n[8] -= (spint)0x4000000000000u;
  carry = prop(n);
  n[0] += ((spint)0x6df6e3d2270c812u) & carry;
  n[1] += ((spint)0xd726e226711ebaeu) & carry;
  n[2] += ((spint)0xa47b84d2e81dcu) & carry;
  n[3] += ((spint)0x783bf2f966b7fccu) & carry;
  n[4] += ((spint)0xfffffffff4a30d0u) & carry;
  n[5] -= (spint)2u & carry;
  n[8] += ((spint)0x4000000000000u) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void inline modsub(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  n[5] = a[5] - b[5];
  n[6] = a[6] - b[6];
  n[7] = a[7] - b[7];
  n[8] = a[8] - b[8];
  carry = prop(n);
  n[0] += ((spint)0x6df6e3d2270c812u) & carry;
  n[1] += ((spint)0xd726e226711ebaeu) & carry;
  n[2] += ((spint)0xa47b84d2e81dcu) & carry;
  n[3] += ((spint)0x783bf2f966b7fccu) & carry;
  n[4] += ((spint)0xfffffffff4a30d0u) & carry;
  n[5] -= (spint)2u & carry;
  n[8] += ((spint)0x4000000000000u) & carry;
  (void)prop(n);
}

// Modular negation
static void inline modneg(const spint *b, spint *n) {
  spint carry;
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  n[5] = (spint)0 - b[5];
  n[6] = (spint)0 - b[6];
  n[7] = (spint)0 - b[7];
  n[8] = (spint)0 - b[8];
  carry = prop(n);
  n[0] += ((spint)0x6df6e3d2270c812u) & carry;
  n[1] += ((spint)0xd726e226711ebaeu) & carry;
  n[2] += ((spint)0xa47b84d2e81dcu) & carry;
  n[3] += ((spint)0x783bf2f966b7fccu) & carry;
  n[4] += ((spint)0xfffffffff4a30d0u) & carry;
  n[5] -= (spint)2u & carry;
  n[8] += ((spint)0x4000000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 3902326505693672769824936250097569004
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0x36fb71e91386409u;
  spint p1 = 0x6b937113388f5d7u;
  spint p2 = 0x523dc269740eeu;
  spint p3 = 0x3c1df97cb35bfe6u;
  spint p4 = 0x7ffffffffa51868u;
  spint q = ((spint)1 << 59u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x52f5ccd79a995c7u;
  t += (dpint)a[0] * b[0];
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)v0 * (dpint)p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  t += (dpint)v1 * (dpint)p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v0 * (dpint)p3;
  t += (dpint)v1 * (dpint)p2;
  t += (dpint)v2 * (dpint)p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)v0 * (dpint)p4;
  t += (dpint)v1 * (dpint)p3;
  t += (dpint)v2 * (dpint)p2;
  t += (dpint)v3 * (dpint)p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (dpint)v4 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  t += (dpint)(spint)(q - v0);
  t += (dpint)v1 * (dpint)p4;
  t += (dpint)v2 * (dpint)p3;
  t += (dpint)v3 * (dpint)p2;
  t += (dpint)v4 * (dpint)p1;
  spint v5 = (((spint)t * ndash) & mask);
  t += (dpint)v5 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  spint s = (spint)mask;
  s -= v1;
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  t += (dpint)v5 * (dpint)p1;
  t += (dpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (dpint)v6 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[7];
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)a[7] * b[0];
  s = (spint)mask;
  s -= v2;
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  t += (dpint)v6 * (dpint)p1;
  t += (dpint)s;
  spint v7 = (((spint)t * ndash) & mask);
  t += (dpint)v7 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[0] * b[8];
  t += (dpint)a[1] * b[7];
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  t += (dpint)a[7] * b[1];
  t += (dpint)a[8] * b[0];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v0 << 49u);
  s -= v3;
  t += (dpint)v4 * (dpint)p4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  t += (dpint)v7 * (dpint)p1;
  t += (dpint)s;
  spint v8 = (((spint)t * ndash) & mask);
  t += (dpint)v8 * (dpint)p0;
  t >>= 59;
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v1 << 49u);
  s -= v4;
  t += (dpint)v5 * (dpint)p4;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)v7 * (dpint)p2;
  t += (dpint)v8 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v2 << 49u);
  s -= v5;
  t += (dpint)v6 * (dpint)p4;
  t += (dpint)v7 * (dpint)p3;
  t += (dpint)v8 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v3 << 49u);
  s -= v6;
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 49u);
  s -= v7;
  t += (dpint)v8 * (dpint)p4;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v5 << 49u);
  s -= v8;
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v6 << 49u);
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v7 << 49u);
  t += (dpint)s;
  c[6] = ((spint)t & mask);
  t >>= 59;
  t += (dpint)a[8] * b[8];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v8 << 49u);
  t += (dpint)s;
  c[7] = ((spint)t & mask);
  t >>= 59;
  t -= (dpint)1u;
  c[8] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0x36fb71e91386409u;
  spint p1 = 0x6b937113388f5d7u;
  spint p2 = 0x523dc269740eeu;
  spint p3 = 0x3c1df97cb35bfe6u;
  spint p4 = 0x7ffffffffa51868u;
  spint q = ((spint)1 << 59u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x52f5ccd79a995c7u;
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (udpint)v0 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (udpint)v1 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  t += (udpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (udpint)v2 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p3;
  t += (udpint)v1 * p2;
  t += (udpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (udpint)v3 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v0 * p4;
  t += (udpint)v1 * p3;
  t += (udpint)v2 * p2;
  t += (udpint)v3 * p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (udpint)v4 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)(spint)(q - v0);
  t += (udpint)v1 * p4;
  t += (udpint)v2 * p3;
  t += (udpint)v3 * p2;
  t += (udpint)v4 * p1;
  spint v5 = (((spint)t * ndash) & mask);
  t += (udpint)v5 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  spint s = (spint)mask;
  s -= v1;
  t += (udpint)v2 * p4;
  t += (udpint)v3 * p3;
  t += (udpint)v4 * p2;
  t += (udpint)v5 * p1;
  t += (udpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (udpint)v6 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  s -= v2;
  t += (udpint)v3 * p4;
  t += (udpint)v4 * p3;
  t += (udpint)v5 * p2;
  t += (udpint)v6 * p1;
  t += (udpint)s;
  spint v7 = (((spint)t * ndash) & mask);
  t += (udpint)v7 * p0;
  t >>= 59;
  tot = (udpint)a[0] * a[8];
  tot += (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  s = (spint)mask;
  t += (udpint)v0 << 49u;
  s -= v3;
  t += (udpint)v4 * p4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  t += (udpint)v7 * p1;
  t += (udpint)s;
  spint v8 = (((spint)t * ndash) & mask);
  t += (udpint)v8 * p0;
  t >>= 59;
  tot = (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v1 << 49u;
  s -= v4;
  t += (udpint)v5 * p4;
  t += (udpint)v6 * p3;
  t += (udpint)v7 * p2;
  t += (udpint)v8 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 << 49u;
  s -= v5;
  t += (udpint)v6 * p4;
  t += (udpint)v7 * p3;
  t += (udpint)v8 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 << 49u;
  s -= v6;
  t += (udpint)v7 * p4;
  t += (udpint)v8 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 49u;
  s -= v7;
  t += (udpint)v8 * p4;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 << 49u;
  s -= v8;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 << 49u;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v7 << 49u;
  t += (udpint)s;
  c[6] = ((spint)t & mask);
  t >>= 59;
  tot = (udpint)a[8] * a[8];
  t += tot;
  s = (spint)mask;
  t += (udpint)v8 << 49u;
  t += (udpint)s;
  c[7] = ((spint)t & mask);
  t >>= 59;
  t -= 1u;
  c[8] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 9; i++) {
    c[i] = a[i];
  }
}

// square n times
static void modnsqr(spint *a, int n) {
  int i;
  for (i = 0; i < n; i++) {
    modsqr(a, a);
  }
}

// Calculate progenitor
static void modpro(const spint *w, spint *z) {
  spint x[9];
  spint t0[9];
  spint t1[9];
  spint t2[9];
  spint t3[9];
  spint t4[9];
  spint t5[9];
  spint t6[9];
  spint t7[9];
  spint t8[9];
  spint t9[9];
  spint t10[9];
  spint t11[9];
  spint t12[9];
  spint t13[9];
  spint t14[9];
  spint t15[9];
  spint t16[9];
  spint t17[9];
  spint t18[9];
  spint t19[9];
  spint t20[9];
  modcpy(w, x);
  modsqr(x, t2);
  modmul(x, t2, t13);
  modmul(x, t13, t4);
  modmul(t13, t4, t0);
  modmul(x, t0, t5);
  modmul(t13, t5, t16);
  modmul(t2, t16, t10);
  modmul(t2, t10, t18);
  modmul(t2, t18, t1);
  modmul(t2, t1, t12);
  modmul(t4, t12, t15);
  modmul(t2, t15, z);
  modmul(t2, z, t3);
  modmul(t2, t3, t6);
  modmul(t5, t6, t11);
  modmul(t4, t11, t14);
  modmul(t4, t14, t9);
  modmul(t2, t9, t17);
  modmul(t4, t17, t7);
  modmul(t4, t7, t4);
  modmul(t2, t4, t8);
  modmul(t2, t8, t5);
  modmul(t2, t5, t2);
  modcpy(t2, t19);
  modnsqr(t19, 3);
  modmul(t15, t19, t15);
  modsqr(t15, t19);
  modcpy(t19, t20);
  modnsqr(t20, 8);
  modmul(t15, t20, t20);
  modnsqr(t20, 10);
  modmul(t19, t20, t19);
  modcpy(t19, t20);
  modnsqr(t20, 8);
  modmul(t15, t20, t20);
  modnsqr(t20, 28);
  modmul(t19, t20, t19);
  modcpy(t19, t20);
  modnsqr(t20, 63);
  modmul(t19, t20, t19);
  modcpy(t19, t20);
  modnsqr(t20, 8);
  modmul(t15, t20, t20);
  modnsqr(t20, 127);
  modmul(t19, t20, t19);
  modmul(x, t19, t19);
  modnsqr(t19, 7);
  modmul(t11, t19, t19);
  modnsqr(t19, 5);
  modmul(t13, t19, t19);
  modnsqr(t19, 8);
  modmul(t10, t19, t19);
  modnsqr(t19, 8);
  modmul(t18, t19, t19);
  modnsqr(t19, 11);
  modmul(t5, t19, t19);
  modnsqr(t19, 4);
  modmul(t18, t19, t18);
  modnsqr(t18, 8);
  modmul(t17, t18, t17);
  modnsqr(t17, 6);
  modmul(t11, t17, t17);
  modnsqr(t17, 5);
  modmul(t12, t17, t17);
  modnsqr(t17, 5);
  modmul(t16, t17, t16);
  modnsqr(t16, 10);
  modmul(t15, t16, t15);
  modnsqr(t15, 4);
  modmul(t13, t15, t15);
  modnsqr(t15, 15);
  modmul(t14, t15, t14);
  modnsqr(t14, 9);
  modmul(t2, t14, t14);
  modnsqr(t14, 2);
  modmul(t13, t14, t13);
  modnsqr(t13, 9);
  modmul(t12, t13, t12);
  modnsqr(t12, 7);
  modmul(t11, t12, t11);
  modnsqr(t11, 4);
  modmul(t10, t11, t10);
  modnsqr(t10, 12);
  modmul(t5, t10, t10);
  modnsqr(t10, 6);
  modmul(t9, t10, t9);
  modnsqr(t9, 7);
  modmul(t8, t9, t8);
  modnsqr(t8, 8);
  modmul(t4, t8, t8);
  modnsqr(t8, 8);
  modmul(t1, t8, t8);
  modnsqr(t8, 8);
  modmul(t7, t8, t7);
  modnsqr(t7, 5);
  modmul(t1, t7, t7);
  modnsqr(t7, 9);
  modmul(t2, t7, t7);
  modnsqr(t7, 6);
  modmul(t6, t7, t6);
  modnsqr(t6, 7);
  modmul(t5, t6, t5);
  modnsqr(t5, 7);
  modmul(t4, t5, t4);
  modnsqr(t4, 5);
  modmul(t3, t4, t3);
  modnsqr(t3, 4);
  modmul(t0, t3, t3);
  modnsqr(t3, 9);
  modmul(t2, t3, t2);
  modnsqr(t2, 7);
  modmul(t1, t2, t1);
  modnsqr(t1, 5);
  modmul(t0, t1, t0);
  modnsqr(t0, 9);
  modmul(z, t0, z);
  modnsqr(z, 6);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  int i;
  spint s[9];
  spint t[9];
  if (h == NULL) {
    modpro(x, t);
  } else {
    modcpy(h, t);
  }
  modcpy(x, s);
  for (i = 0; i < (3 - 1); i++) {
    modsqr(s, s);
    modmul(s, x, s);
  }
  modnsqr(t, 4);
  modmul(s, t, z);
}

// Convert m to n-residue form, n=nres(m)
static void nres(const spint *m, spint *n) {
  const spint c[9] = {
      0x5765fcd15cf17dbu, 0x6c812fbca5b2553u, 0x46d5e1be513b608u,
      0x2c0753b44a31127u, 0x33fcf450ac6f345u, 0x5e636b0d472b3deu,
      0xe03d1492d0d455u,  0x475fd1888707a5bu, 0x80e468e696d6u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[9];
  c[0] = 1;
  for (i = 1; i < 9; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[9];
  for (i = 0; i < 9; i++) {
    b[i] = n[i];
    t[i] = n[i + 9];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[9];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 9; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 59u) &
          (((c0 ^ (spint)1) - (spint)1) >> 59u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[9];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 9; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 59u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 9; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 9; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 9; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[9];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
  modnsqr(r, 2);
  return modis1(r);
}

// conditional move g to f if d=1
static int modcmv(int d, const spint *g, spint *f) {
  int i;
  spint c = (-d);
  spint w = 0;
  spint r = f[0] ^ g[1];
  spint ra = r + r;
  ra >>= 1;
  for (i = 0; i < 9; i++) {
    spint t = (f[i] ^ g[i]) & c;
    t ^= r;
    spint e = f[i] ^ t;
    w ^= e;
    f[i] = e ^ ra;
  }
  return w;
}

// conditional swap g and f if d=1
static int modcsw(int d, spint *g, spint *f) {
  int i;
  spint c = (-d);
  spint w = 0;
  spint r = f[0] ^ g[1];
  spint ra = r + r;
  ra >>= 1;
  for (i = 0; i < 9; i++) {
    spint t = (f[i] ^ g[i]) & c;
    t ^= r;
    spint e = f[i] ^ t;
    w ^= e;
    f[i] = e ^ ra;
    e = g[i] ^ t;
    w ^= e;
    g[i] = e ^ ra;
  }
  return w;
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  int k;
  spint t[9];
  spint b[9];
  spint v[9];
  spint z[9] = {0x7f9c7b7b63e2205u, 0x6567b50c3590214u, 0x2de0d991f4e9c91u,
                0x4ff9c8ee6de6cccu, 0x3a3055276d535f7u, 0x25b5f525f98aceu,
                0x2313ef1f980035fu, 0x460bf9eb845697du, 0x9a0a650d44b2u};
  spint s[9];
  spint y[9];
  if (h == NULL) {
    modpro(x, y);
  } else {
    modcpy(h, y);
  }
  modmul(y, x, s);
  modmul(s, y, t);
  nres(z, z);
  for (k = 3; k > 1; k--) {
    modcpy(t, b);
    modnsqr(b, k - 2);
    int d = 1 - modis1(b);
    modmul(s, z, v);
    (void)modcmv(d, v, s);
    modsqr(z, z);
    modmul(t, z, v);
    (void)modcmv(d, v, t);
  }
  modcpy(s, r);
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
  int i;
  a[8] = ((a[8] << n)) | (a[7] >> (59u - n));
  for (i = 7; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7ffffffffffffff) | (a[i - 1] >> (59u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7ffffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 8; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (59u - n)) & (spint)0x7ffffffffffffff);
  }
  a[8] = a[8] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 59u;
  unsigned int m = r % 59u;
  modzer(a);
  if (r >= 66 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
  nres(a, a);
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[9];
  redc(a, c);
  for (i = 65; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 9; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 66; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[9];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[9], d[9];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 9; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 59) & 1;
  }
  return eq;
}

/*** End of automatically generated code ***/

// number of bytes in representation
#define BYTES Nbytes
typedef spint gel[Nlimbs];  // group element definition

// Some utility functions for I/O and debugging

// reverse bytes of buff - for little endian
static void reverse(char *buff) {
    int n=BYTES;
    for (int i = 0; i < n/2; i++) { 
        char ch = buff[i]; 
        buff[i] = buff[n - i - 1]; 
        buff[n - i - 1] = ch; 
    } 
}

static int char2int(char input)
{
    if ((input >= '0') && (input <= '9'))
        return input - '0';
    if ((input >= 'A') && (input <= 'F'))
        return input - 'A' + 10;
    if ((input >= 'a') && (input <= 'f'))
        return input - 'a' + 10;
    return 0;
}

// Convert from a hex string to byte array 
static void fromHex(int ilen, const char *src, char *dst)
{
    int i,lz,len=0;
    char pad[300];
    while (src[len]!=0) len++;
    lz=2*ilen-len;
    if (lz<0) lz=0;
    for (i=0;i<lz;i++) pad[i]='0';  // pad with leading zeros
    for (i=lz;i<2*ilen;i++) pad[i]=src[i-lz];

    for (i=0;i<ilen;i++)
    {
        dst[i] = (char2int(pad[2*i]) * 16) + char2int(pad[2*i + 1]);
    }
}

static void byte2hex(char *ptr,unsigned char ch)
{
    int t=ch/16;
    int b=ch%16;
    if (t<10)
    	ptr[0]='0'+t;
    else
    	ptr[0]='a'+(t-10);
    if (b<10)
    	ptr[1]='0'+b;
    else
    	ptr[1]='a'+(b-10);    	
}

// Convert a byte array to a hex string 
static void toHex(int len, const char *src, char *dst)
{
    int i;
    for (i = 0; i < len; i++)
    {
        unsigned char ch = src[i];
        byte2hex(&dst[i * 2],ch);
    }
    dst[2*len]='\0';
}

// I/O debug code
// output a modulo number in hex
/*
static void output(spint *x) {
    char b[Nbytes+1];
    char buff[(2*Nbytes)+1];
    modexp(x,b);
    toHex(Nbytes,b,buff);
    puts(buff);
}

// output a point (x,y)
void outputxy(point *P)
{
    if (ecn521isinf(P)) {
        printf("P= O\n");
    } else {
        char x[BYTES],y[BYTES];
        char buff[(2*BYTES)+1];
        ecn521get(P,x,y);
        toHex(BYTES,x,buff);
        printf("Px= "); puts(buff);
        toHex(BYTES,y,buff);
        printf("Py= "); puts(buff);
    }
}
*/

// reduce 74 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^520.x + y, where x and y < q (x is top 9 bytes, y is bottom 65 bytes)
// Important that x and y < q
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,c;

    mod2r(8*(BYTES-1),c); // 2^520

    for (i=0;i<BYTES-1;i++)
        buff[i]=h[i];  // little endian
    buff[BYTES-1]=0;
    reverse(buff);
    modimp(buff,y);

    for (i=0;i<9;i++)
        buff[i]=h[BYTES-1+i];
    for (i=9;i<BYTES;i++)
        buff[i]=0;
    reverse(buff);
    modimp(buff,x);

    modmul(x,c,x);
    modadd(x,y,r);   // 2^520.x + y
}

//#define PREHASHED   // define for test vectors

// API

#include "tlsecc.h"

// Input private key - 66 random bytes
// Output public key - 133 bytes (0x04<x>|<y>), or 67 if compressed (0x02<x>.. or 0x03<x>)
void NIST521_KEY_PAIR(int compress,char *SK,char *PK)
{
    point P;
    ecn521gen(&P);
    ecn521mul(SK,&P);

    if (compress) {
        PK[0]=0x02+ecn521get(&P,&PK[1],NULL); // 0x02 or 0x03
    } else {
        PK[0]=0x04; // no compression
        ecn521get(&P,&PK[1],&PK[BYTES+1]);  // get x and y
    }
}

// input private key, per-message random number, message to be signed. Output signature.
// ran must be Nbytes+8 in length, in this case 74 bytes
void NIST521_SIGN(char *prv,char *ran,int mlen,char *m,char *sig)
{
    char h[BYTES];
    point R;
    gel e,r,s,k;

#ifdef PREHASHED
    modimp(m,e);
#else
    int i;
    hash512 sh512;
    HASH512_init(&sh512);
    for (i=0;i<mlen;i++)
        HASH512_process(&sh512,m[i]);
    HASH512_hash(&sh512,h); 

    modimp(h,e);
#endif

    ecn521gen(&R);
    modimp(prv,s);

    reduce(ran,k);
    modexp(k,h);
    ecn521mul(h,&R);
    modinv(k,NULL,k);

    ecn521get(&R,h,NULL);
    modimp(h,r);

    modmul(s,r,s);
    modadd(s,e,s);
    modmul(s,k,s);
    modzer(k);

    modexp(r,sig);
    modexp(s,&sig[BYTES]);
}

// input public key, message and signature
int NIST521_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    point G,Q;
    int i,res;
    char rb[BYTES],u[BYTES],v[BYTES];
    gel e,r,s;
#ifdef PREHASHED
    modimp(m,e);
#else
    char h[BYTES];
    hash512 sh512;
    HASH512_init(&sh512);
    for (i=0;i<mlen;i++)
        HASH512_process(&sh512,m[i]);
    HASH512_hash(&sh512,h); 

    modimp(h,e);
#endif

    ecn521gen(&G);

// import from signature
    if (!modimp(sig,r)) return 0; // if not in range
    if (!modimp(&sig[BYTES],s)) return 0;

    if (modis0(r) || modis0(s)) return 0;
    modinv(s,NULL,s);
    modmul(r,s,r); modexp(r,v);  // export to byte array
    modmul(s,e,s); modexp(s,u); 

    if (pub[0]==0x04) {
        ecn521set(0,&pub[1],&pub[BYTES+1],&Q);
    } else {
        ecn521set((int)pub[0]&1,&pub[1],NULL,&Q);
    }

    ecn521mul2(u,&G,v,&Q,&Q);
    if (ecn521isinf(&Q)) return 0;

    ecn521get(&Q,rb,NULL);

    res=1;
    for (i=0;i<BYTES;i++) {
        if (sig[i]!=rb[i]) res=0;
    }
    
    return res;
}

int NIST521_SHARED_SECRET(char *SK,char *PK,char *SS)
{
    point P;

    if (PK[0]==0x04) {
        ecn521set(0,&PK[1],&PK[BYTES+1],&P);
    } else {
        ecn521set((int)PK[0]&1,&PK[1],NULL,&P);
    }

    ecn521mul(SK,&P);

    ecn521get(&P,SS,NULL);  // x coordinate
    if (ecn521isinf(&P)) return 0;
    return 1;
}

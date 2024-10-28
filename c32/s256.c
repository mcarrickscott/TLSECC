
// ECDSA Implementation for curve P-256
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
// python curve.py 64 NIST256
// This completes weierstrass.c for this curve. Then
// gcc -O2 EC256.c weierstrass.c hash.c -o EC256

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "nist256curve.h"   // elliptic curve API

/*** Insert automatically generated code for P-256 prime group order group.c here ***/
/* Note that much of this code is not needed and can be deleted */

// Command line : python monty.py 32 nist256

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 9
#define Radix 29
#define Nbits 256
#define Nbytes 32

#define MONTGOMERY
#define NIST256

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 29u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 29u;
  n[0] &= mask;
  for (i = 1; i < 8; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 29u;
  }
  n[8] += (spint)carry;
  return -((n[8] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x1c632551u) & carry;
  n[1] += ((spint)0x1dce5617u) & carry;
  n[2] += ((spint)0x5e7a13cu) & carry;
  n[3] += ((spint)0xdf55b4eu) & carry;
  n[4] += ((spint)0x1ffffbceu) & carry;
  n[5] -= (spint)1u & carry;
  n[6] += ((spint)0x40000u) & carry;
  n[7] += ((spint)0x1fe00000u) & carry;
  n[8] += ((spint)0xffffffu) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] -= (spint)0x1c632551u;
  n[1] -= (spint)0x1dce5617u;
  n[2] -= (spint)0x5e7a13cu;
  n[3] -= (spint)0xdf55b4eu;
  n[4] -= (spint)0x1ffffbceu;
  n[5] += (spint)1u;
  n[6] -= (spint)0x40000u;
  n[7] -= (spint)0x1fe00000u;
  n[8] -= (spint)0xffffffu;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static void modadd(const spint *a, const spint *b, spint *n) {
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
  n[0] -= (spint)0x38c64aa2u;
  n[1] -= (spint)0x3b9cac2eu;
  n[2] -= (spint)0xbcf4278u;
  n[3] -= (spint)0x1beab69cu;
  n[4] -= (spint)0x3ffff79cu;
  n[5] += (spint)2u;
  n[6] -= (spint)0x80000u;
  n[7] -= (spint)0x3fc00000u;
  n[8] -= (spint)0x1fffffeu;
  carry = prop(n);
  n[0] += ((spint)0x38c64aa2u) & carry;
  n[1] += ((spint)0x3b9cac2eu) & carry;
  n[2] += ((spint)0xbcf4278u) & carry;
  n[3] += ((spint)0x1beab69cu) & carry;
  n[4] += ((spint)0x3ffff79cu) & carry;
  n[5] -= (spint)2u & carry;
  n[6] += ((spint)0x80000u) & carry;
  n[7] += ((spint)0x3fc00000u) & carry;
  n[8] += ((spint)0x1fffffeu) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void modsub(const spint *a, const spint *b, spint *n) {
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
  n[0] += ((spint)0x38c64aa2u) & carry;
  n[1] += ((spint)0x3b9cac2eu) & carry;
  n[2] += ((spint)0xbcf4278u) & carry;
  n[3] += ((spint)0x1beab69cu) & carry;
  n[4] += ((spint)0x3ffff79cu) & carry;
  n[5] -= (spint)2u & carry;
  n[6] += ((spint)0x80000u) & carry;
  n[7] += ((spint)0x3fc00000u) & carry;
  n[8] += ((spint)0x1fffffeu) & carry;
  (void)prop(n);
}

// Modular negation
static void modneg(const spint *b, spint *n) {
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
  n[0] += ((spint)0x38c64aa2u) & carry;
  n[1] += ((spint)0x3b9cac2eu) & carry;
  n[2] += ((spint)0xbcf4278u) & carry;
  n[3] += ((spint)0x1beab69cu) & carry;
  n[4] += ((spint)0x3ffff79cu) & carry;
  n[5] -= (spint)2u & carry;
  n[6] += ((spint)0x80000u) & carry;
  n[7] += ((spint)0x3fc00000u) & carry;
  n[8] += ((spint)0x1fffffeu) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 3881626228440796233
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0x1c632551u;
  spint p1 = 0x1dce5617u;
  spint p2 = 0x5e7a13cu;
  spint p3 = 0xdf55b4eu;
  spint p4 = 0x1ffffbceu;
  spint p7 = 0x1fe00000u;
  spint p8 = 0xffffffu;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xe00bc4fu;
  t += (dpint)a[0] * b[0];
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * (dpint)p0;
  t >>= 29;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)v0 * (dpint)p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * (dpint)p0;
  t >>= 29;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  t += (dpint)v1 * (dpint)p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * (dpint)p0;
  t >>= 29;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v0 * (dpint)p3;
  t += (dpint)v1 * (dpint)p2;
  t += (dpint)v2 * (dpint)p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * (dpint)p0;
  t >>= 29;
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
  t >>= 29;
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
  t >>= 29;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  spint s = (spint)mask;
  t += (dpint)(udpint)((udpint)v0 << 18u);
  s -= v1;
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  t += (dpint)v5 * (dpint)p1;
  t += (dpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (dpint)v6 * (dpint)p0;
  t >>= 29;
  t += (dpint)a[0] * b[7];
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)a[7] * b[0];
  s = (spint)mask;
  t += (dpint)v0 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v1 << 18u);
  s -= v2;
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  t += (dpint)v6 * (dpint)p1;
  t += (dpint)s;
  spint v7 = (((spint)t * ndash) & mask);
  t += (dpint)v7 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v0 * (dpint)p8;
  t += (dpint)v1 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v2 << 18u);
  s -= v3;
  t += (dpint)v4 * (dpint)p4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  t += (dpint)v7 * (dpint)p1;
  t += (dpint)s;
  spint v8 = (((spint)t * ndash) & mask);
  t += (dpint)v8 * (dpint)p0;
  t >>= 29;
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  s = (spint)mask;
  t += (dpint)v1 * (dpint)p8;
  t += (dpint)v2 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v3 << 18u);
  s -= v4;
  t += (dpint)v5 * (dpint)p4;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)v7 * (dpint)p2;
  t += (dpint)v8 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  s = (spint)mask;
  t += (dpint)v2 * (dpint)p8;
  t += (dpint)v3 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v4 << 18u);
  s -= v5;
  t += (dpint)v6 * (dpint)p4;
  t += (dpint)v7 * (dpint)p3;
  t += (dpint)v8 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  s = (spint)mask;
  t += (dpint)v3 * (dpint)p8;
  t += (dpint)v4 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v5 << 18u);
  s -= v6;
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  s = (spint)mask;
  t += (dpint)v4 * (dpint)p8;
  t += (dpint)v5 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v6 << 18u);
  s -= v7;
  t += (dpint)v8 * (dpint)p4;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  s = (spint)mask;
  t += (dpint)v5 * (dpint)p8;
  t += (dpint)v6 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v7 << 18u);
  s -= v8;
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  s = (spint)mask;
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v8 << 18u);
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  s = (spint)mask;
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  t += (dpint)s;
  c[6] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[8] * b[8];
  s = (spint)mask;
  t += (dpint)v8 * (dpint)p8;
  t += (dpint)s;
  c[7] = ((spint)t & mask);
  t >>= 29;
  t -= (dpint)1u;
  c[8] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0x1c632551u;
  spint p1 = 0x1dce5617u;
  spint p2 = 0x5e7a13cu;
  spint p3 = 0xdf55b4eu;
  spint p4 = 0x1ffffbceu;
  spint p7 = 0x1fe00000u;
  spint p8 = 0xffffffu;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xe00bc4fu;
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (udpint)v0 * p0;
  t >>= 29;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (udpint)v1 * p0;
  t >>= 29;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  t += (udpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (udpint)v2 * p0;
  t >>= 29;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p3;
  t += (udpint)v1 * p2;
  t += (udpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (udpint)v3 * p0;
  t >>= 29;
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
  t >>= 29;
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
  t >>= 29;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  spint s = (spint)mask;
  t += (udpint)v0 << 18u;
  s -= v1;
  t += (udpint)v2 * p4;
  t += (udpint)v3 * p3;
  t += (udpint)v4 * p2;
  t += (udpint)v5 * p1;
  t += (udpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (udpint)v6 * p0;
  t >>= 29;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v0 * p7;
  t += (udpint)v1 << 18u;
  s -= v2;
  t += (udpint)v3 * p4;
  t += (udpint)v4 * p3;
  t += (udpint)v5 * p2;
  t += (udpint)v6 * p1;
  t += (udpint)s;
  spint v7 = (((spint)t * ndash) & mask);
  t += (udpint)v7 * p0;
  t >>= 29;
  tot = (udpint)a[0] * a[8];
  tot += (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  s = (spint)mask;
  t += (udpint)v0 * p8;
  t += (udpint)v1 * p7;
  t += (udpint)v2 << 18u;
  s -= v3;
  t += (udpint)v4 * p4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  t += (udpint)v7 * p1;
  t += (udpint)s;
  spint v8 = (((spint)t * ndash) & mask);
  t += (udpint)v8 * p0;
  t >>= 29;
  tot = (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v1 * p8;
  t += (udpint)v2 * p7;
  t += (udpint)v3 << 18u;
  s -= v4;
  t += (udpint)v5 * p4;
  t += (udpint)v6 * p3;
  t += (udpint)v7 * p2;
  t += (udpint)v8 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 * p8;
  t += (udpint)v3 * p7;
  t += (udpint)v4 << 18u;
  s -= v5;
  t += (udpint)v6 * p4;
  t += (udpint)v7 * p3;
  t += (udpint)v8 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 * p8;
  t += (udpint)v4 * p7;
  t += (udpint)v5 << 18u;
  s -= v6;
  t += (udpint)v7 * p4;
  t += (udpint)v8 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 * p8;
  t += (udpint)v5 * p7;
  t += (udpint)v6 << 18u;
  s -= v7;
  t += (udpint)v8 * p4;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 * p8;
  t += (udpint)v6 * p7;
  t += (udpint)v7 << 18u;
  s -= v8;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 * p8;
  t += (udpint)v7 * p7;
  t += (udpint)v8 << 18u;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v7 * p8;
  t += (udpint)v8 * p7;
  t += (udpint)s;
  c[6] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[8] * a[8];
  t += tot;
  s = (spint)mask;
  t += (udpint)v8 * p8;
  t += (udpint)s;
  c[7] = ((spint)t & mask);
  t >>= 29;
  t -= 1u;
  c[8] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
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
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, t0);
  modsqr(t0, t5);
  modmul(z, t5, t1);
  modmul(x, t1, t4);
  modsqr(t4, t2);
  modmul(x, t2, z);
  modmul(x, z, t6);
  modmul(t1, t6, t0);
  modmul(z, t0, z);
  modmul(t2, z, t2);
  modmul(t0, t2, t3);
  modmul(t4, t3, t0);
  modmul(z, t0, z);
  modmul(t6, z, t6);
  modmul(t2, t6, t2);
  modmul(t4, t2, t7);
  modmul(t3, t7, t4);
  modmul(t0, t4, t0);
  modmul(t2, t0, t3);
  modmul(t1, t3, t2);
  modmul(x, t2, t1);
  modmul(t7, t1, t7);
  modmul(t6, t7, t6);
  modmul(t1, t6, t1);
  modmul(t5, t1, t5);
  modmul(t4, t5, t4);
  modmul(t2, t4, t2);
  modmul(t3, t2, t3);
  modmul(t6, t3, t6);
  modmul(t4, t6, t4);
  modmul(t3, t4, t3);
  modmul(t4, t3, t4);
  modmul(t2, t4, t2);
  modmul(t6, t2, t6);
  modmul(t1, t6, t1);
  modmul(t4, t1, t4);
  modmul(t2, t4, t2);
  modmul(t5, t2, t5);
  modmul(t0, t5, t0);
  modmul(t7, t0, t7);
  modcpy(t7, t8);
  modnsqr(t8, 16);
  modmul(t7, t8, t8);
  modnsqr(t8, 48);
  modmul(t7, t8, t8);
  modnsqr(t8, 16);
  modmul(t7, t8, t8);
  modnsqr(t8, 16);
  modmul(t7, t8, t8);
  modnsqr(t8, 16);
  modmul(t7, t8, t7);
  modnsqr(t7, 15);
  modmul(t6, t7, t6);
  modnsqr(t6, 17);
  modmul(t5, t6, t5);
  modnsqr(t5, 16);
  modmul(t4, t5, t4);
  modnsqr(t4, 14);
  modmul(t3, t4, t3);
  modnsqr(t3, 18);
  modmul(t2, t3, t2);
  modnsqr(t2, 15);
  modmul(t1, t2, t1);
  modnsqr(t1, 17);
  modmul(t0, t1, t0);
  modnsqr(t0, 10);
  modmul(z, t0, z);
  modsqr(z, z);
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
  for (i = 0; i < (4 - 1); i++) {
    modsqr(s, s);
    modmul(s, x, s);
  }
  modnsqr(t, 5);
  modmul(s, t, z);
}

// Convert m to n-residue form, n=nres(m)
static void nres(const spint *m, spint *n) {
  const spint c[9] = {0x148d9ef5u, 0xf4e7f75u,  0x14c6a651u,
                      0x3f8b765u,  0x165861f1u, 0x1256d7d8u,
                      0x1c185b23u, 0xd4cab0fu,  0x84b655u};
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
  return ((spint)1 & ((d - (spint)1) >> 29u) &
          (((c0 ^ (spint)1) - (spint)1) >> 29u));
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
  return ((spint)1 & ((d - (spint)1) >> 29u));
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
  modnsqr(r, 3);
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
  spint z[9] = {0x141e6602u, 0xc96bfddu, 0xde36bc1u, 0xd95a008u, 0x3dfc154u,
                0x1d674215u, 0x64aea01u, 0xc54ee1u,  0xffc97fu};
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
  for (k = 4; k > 1; k--) {
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
  a[8] = ((a[8] << n)) | (a[7] >> (29u - n));
  for (i = 7; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x1fffffff) | (a[i - 1] >> (29u - n));
  }
  a[0] = (a[0] << n) & (spint)0x1fffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 8; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (29u - n)) & (spint)0x1fffffff);
  }
  a[8] = a[8] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 29u;
  unsigned int m = r % 29u;
  modzer(a);
  if (r >= 32 * 8)
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
  for (i = 31; i >= 0; i--) {
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
  for (i = 0; i < 32; i++) {
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
    eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
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
    if (ecn256isinf(P)) {
        printf("P= O\n");
    } else {
        char x[BYTES],y[BYTES];
        char buff[(2*BYTES)+1];
        ecn256get(P,x,y);
        toHex(BYTES,x,buff);
        printf("Px= "); puts(buff);
        toHex(BYTES,y,buff);
        printf("Py= "); puts(buff);
    }
}
*/

// reduce 40 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^248.x + y, where x and y < q (x is top 9 bytes, y is bottom 31 bytes)
// Important that x and y < q
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,c;
    
    mod2r(8*(BYTES-1),c); // 2^248

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
    modadd(x,y,r); // 2^248.x + y
}

//#define PREHASHED   // define for test vectors

// API

#include "tlsecc.h"

// Input private key - 32 random bytes
// Output public key - 65 bytes (0x04<x>|<y>), or 33 if compressed (0x02<x>.. or 0x03<x>)

void NIST256_KEY_PAIR(int compress,char *SK,char *PK)
{
    point P;
    ecn256gen(&P);
    ecn256mul(SK,&P);
    if (compress) {
        PK[0]=0x02+ecn256get(&P,&PK[1],NULL); // 0x02 or 0x03
    } else {
        PK[0]=0x04; // no compression
        ecn256get(&P,&PK[1],&PK[BYTES+1]);  // get x and y
    }
}

// input private key, per-message random number, message to be signed. Output signature.
// ran must be Nbytes+8 in length, in this case 40 bytes
void NIST256_SIGN(char *prv,char *ran,int mlen,char *m,char *sig)
{
    char h[BYTES];
    point R;
    gel e,r,s,k;

#ifdef PREHASHED
    modimp(m,e);
#else
    int i;
    hash256 sh256;
    HASH256_init(&sh256);
    for (i=0;i<mlen;i++)
        HASH256_process(&sh256,m[i]);
    HASH256_hash(&sh256,h); 

    modimp(h,e);
#endif

    ecn256gen(&R);
    modimp(prv,s);

    reduce(ran,k);
    modexp(k,h);
    ecn256mul(h,&R);
    modinv(k,NULL,k);

    ecn256get(&R,h,NULL);
    modimp(h,r);

    modmul(s,r,s);
    modadd(s,e,s);
    modmul(s,k,s);
    modzer(k);

    modexp(r,sig);
    modexp(s,&sig[BYTES]);
}

// input public key, message and signature
int NIST256_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    point G,Q;
    int i,res;
    char rb[BYTES],u[BYTES],v[BYTES];
    gel e,r,s;
#ifdef PREHASHED
    modimp(m,e);
#else
    char h[BYTES];
    hash256 sh256;
    HASH256_init(&sh256);
    for (i=0;i<mlen;i++)
        HASH256_process(&sh256,m[i]);
    HASH256_hash(&sh256,h); 

    modimp(h,e);
#endif

    ecn256gen(&G);

// import from signature
    if (!modimp(sig,r)) return 0; // if not in range
    if (!modimp(&sig[BYTES],s)) return 0;

    if (modis0(r) || modis0(s)) return 0;
    modinv(s,NULL,s);
    modmul(r,s,r); modexp(r,v);  // export to byte array
    modmul(s,e,s); modexp(s,u); 

    if (pub[0]==0x04) {
        ecn256set(0,&pub[1],&pub[BYTES+1],&Q);
    } else {
        ecn256set((int)pub[0]&1,&pub[1],NULL,&Q);
    }

    ecn256mul2(u,&G,v,&Q,&Q);
    if (ecn256isinf(&Q)) return 0;

    ecn256get(&Q,rb,NULL);

    res=1;
    for (i=0;i<BYTES;i++) {
        if (sig[i]!=rb[i]) res=0;
    }
    
    return res;
}

int NIST256_SHARED_SECRET(char *SK,char *PK,char *SS)
{
    point P;

    if (PK[0]==0x04) {
        ecn256set(0,&PK[1],&PK[BYTES+1],&P);
    } else {
        ecn256set((int)PK[0]&1,&PK[1],NULL,&P);
    }

    ecn256mul(SK,&P);

    ecn256get(&P,SS,NULL);  // x coordinate
    if (ecn256isinf(&P)) return 0;
    return 1;
}
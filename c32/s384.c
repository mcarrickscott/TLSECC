
// ECDSA Implementation for curve P-384
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
// python curve.py 64 NIST384
// This completes weierstrass.c for this curve. 

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "nist384curve.h"   // elliptic curve API

/*** Insert automatically generated code for P-384 prime group order group.c here ***/
/* Note that much of this code is not needed and can be deleted */


// Command line : python monty.py 32 nist384

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 14
#define Radix 28
#define Nbits 384
#define Nbytes 48

#define MONTGOMERY
#define NIST384

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 28u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 28u;
  n[0] &= mask;
  for (i = 1; i < 13; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[13] += (spint)carry;
  return -((n[13] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0xcc52973u) & carry;
  n[1] += ((spint)0xec196acu) & carry;
  n[2] += ((spint)0xa77aecu) & carry;
  n[3] += ((spint)0xdb248bu) & carry;
  n[4] += ((spint)0xddf581au) & carry;
  n[5] += ((spint)0x81f4372u) & carry;
  n[6] += ((spint)0xfc7634du) & carry;
  n[7] -= (spint)1u & carry;
  n[13] += ((spint)0x100000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] -= (spint)0xcc52973u;
  n[1] -= (spint)0xec196acu;
  n[2] -= (spint)0xa77aecu;
  n[3] -= (spint)0xdb248bu;
  n[4] -= (spint)0xddf581au;
  n[5] -= (spint)0x81f4372u;
  n[6] -= (spint)0xfc7634du;
  n[7] += (spint)1u;
  n[13] -= (spint)0x100000u;
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
  n[9] = a[9] + b[9];
  n[10] = a[10] + b[10];
  n[11] = a[11] + b[11];
  n[12] = a[12] + b[12];
  n[13] = a[13] + b[13];
  n[0] -= (spint)0x198a52e6u;
  n[1] -= (spint)0x1d832d58u;
  n[2] -= (spint)0x14ef5d8u;
  n[3] -= (spint)0x1b64916u;
  n[4] -= (spint)0x1bbeb034u;
  n[5] -= (spint)0x103e86e4u;
  n[6] -= (spint)0x1f8ec69au;
  n[7] += (spint)2u;
  n[13] -= (spint)0x200000u;
  carry = prop(n);
  n[0] += ((spint)0x198a52e6u) & carry;
  n[1] += ((spint)0x1d832d58u) & carry;
  n[2] += ((spint)0x14ef5d8u) & carry;
  n[3] += ((spint)0x1b64916u) & carry;
  n[4] += ((spint)0x1bbeb034u) & carry;
  n[5] += ((spint)0x103e86e4u) & carry;
  n[6] += ((spint)0x1f8ec69au) & carry;
  n[7] -= (spint)2u & carry;
  n[13] += ((spint)0x200000u) & carry;
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
  n[9] = a[9] - b[9];
  n[10] = a[10] - b[10];
  n[11] = a[11] - b[11];
  n[12] = a[12] - b[12];
  n[13] = a[13] - b[13];
  carry = prop(n);
  n[0] += ((spint)0x198a52e6u) & carry;
  n[1] += ((spint)0x1d832d58u) & carry;
  n[2] += ((spint)0x14ef5d8u) & carry;
  n[3] += ((spint)0x1b64916u) & carry;
  n[4] += ((spint)0x1bbeb034u) & carry;
  n[5] += ((spint)0x103e86e4u) & carry;
  n[6] += ((spint)0x1f8ec69au) & carry;
  n[7] -= (spint)2u & carry;
  n[13] += ((spint)0x200000u) & carry;
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
  n[9] = (spint)0 - b[9];
  n[10] = (spint)0 - b[10];
  n[11] = (spint)0 - b[11];
  n[12] = (spint)0 - b[12];
  n[13] = (spint)0 - b[13];
  carry = prop(n);
  n[0] += ((spint)0x198a52e6u) & carry;
  n[1] += ((spint)0x1d832d58u) & carry;
  n[2] += ((spint)0x14ef5d8u) & carry;
  n[3] += ((spint)0x1b64916u) & carry;
  n[4] += ((spint)0x1bbeb034u) & carry;
  n[5] += ((spint)0x103e86e4u) & carry;
  n[6] += ((spint)0x1f8ec69au) & carry;
  n[7] -= (spint)2u & carry;
  n[13] += ((spint)0x200000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1309973499202281886
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0xcc52973u;
  spint p1 = 0xec196acu;
  spint p2 = 0xa77aecu;
  spint p3 = 0xdb248bu;
  spint p4 = 0xddf581au;
  spint p5 = 0x81f4372u;
  spint p6 = 0xfc7634du;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x88fdc45u;
  t += (dpint)a[0] * b[0];
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)v0 * (dpint)p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  t += (dpint)v1 * (dpint)p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v0 * (dpint)p3;
  t += (dpint)v1 * (dpint)p2;
  t += (dpint)v2 * (dpint)p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * (dpint)p0;
  t >>= 28;
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
  t >>= 28;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  t += (dpint)v0 * (dpint)p5;
  t += (dpint)v1 * (dpint)p4;
  t += (dpint)v2 * (dpint)p3;
  t += (dpint)v3 * (dpint)p2;
  t += (dpint)v4 * (dpint)p1;
  spint v5 = (((spint)t * ndash) & mask);
  t += (dpint)v5 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  t += (dpint)v0 * (dpint)p6;
  t += (dpint)v1 * (dpint)p5;
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  t += (dpint)v5 * (dpint)p1;
  spint v6 = (((spint)t * ndash) & mask);
  t += (dpint)v6 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[7];
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)a[7] * b[0];
  t += (dpint)(spint)(q - v0);
  t += (dpint)v1 * (dpint)p6;
  t += (dpint)v2 * (dpint)p5;
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  t += (dpint)v6 * (dpint)p1;
  spint v7 = (((spint)t * ndash) & mask);
  t += (dpint)v7 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[8];
  t += (dpint)a[1] * b[7];
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  t += (dpint)a[7] * b[1];
  t += (dpint)a[8] * b[0];
  spint s = (spint)mask;
  s -= v1;
  t += (dpint)v2 * (dpint)p6;
  t += (dpint)v3 * (dpint)p5;
  t += (dpint)v4 * (dpint)p4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  t += (dpint)v7 * (dpint)p1;
  t += (dpint)s;
  spint v8 = (((spint)t * ndash) & mask);
  t += (dpint)v8 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[9];
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  t += (dpint)a[9] * b[0];
  s = (spint)mask;
  s -= v2;
  t += (dpint)v3 * (dpint)p6;
  t += (dpint)v4 * (dpint)p5;
  t += (dpint)v5 * (dpint)p4;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)v7 * (dpint)p2;
  t += (dpint)v8 * (dpint)p1;
  t += (dpint)s;
  spint v9 = (((spint)t * ndash) & mask);
  t += (dpint)v9 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[10];
  t += (dpint)a[1] * b[9];
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  t += (dpint)a[9] * b[1];
  t += (dpint)a[10] * b[0];
  s = (spint)mask;
  s -= v3;
  t += (dpint)v4 * (dpint)p6;
  t += (dpint)v5 * (dpint)p5;
  t += (dpint)v6 * (dpint)p4;
  t += (dpint)v7 * (dpint)p3;
  t += (dpint)v8 * (dpint)p2;
  t += (dpint)v9 * (dpint)p1;
  t += (dpint)s;
  spint v10 = (((spint)t * ndash) & mask);
  t += (dpint)v10 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[11];
  t += (dpint)a[1] * b[10];
  t += (dpint)a[2] * b[9];
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  t += (dpint)a[9] * b[2];
  t += (dpint)a[10] * b[1];
  t += (dpint)a[11] * b[0];
  s = (spint)mask;
  s -= v4;
  t += (dpint)v5 * (dpint)p6;
  t += (dpint)v6 * (dpint)p5;
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
  t += (dpint)v9 * (dpint)p2;
  t += (dpint)v10 * (dpint)p1;
  t += (dpint)s;
  spint v11 = (((spint)t * ndash) & mask);
  t += (dpint)v11 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[12];
  t += (dpint)a[1] * b[11];
  t += (dpint)a[2] * b[10];
  t += (dpint)a[3] * b[9];
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  t += (dpint)a[9] * b[3];
  t += (dpint)a[10] * b[2];
  t += (dpint)a[11] * b[1];
  t += (dpint)a[12] * b[0];
  s = (spint)mask;
  s -= v5;
  t += (dpint)v6 * (dpint)p6;
  t += (dpint)v7 * (dpint)p5;
  t += (dpint)v8 * (dpint)p4;
  t += (dpint)v9 * (dpint)p3;
  t += (dpint)v10 * (dpint)p2;
  t += (dpint)v11 * (dpint)p1;
  t += (dpint)s;
  spint v12 = (((spint)t * ndash) & mask);
  t += (dpint)v12 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[13];
  t += (dpint)a[1] * b[12];
  t += (dpint)a[2] * b[11];
  t += (dpint)a[3] * b[10];
  t += (dpint)a[4] * b[9];
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  t += (dpint)a[9] * b[4];
  t += (dpint)a[10] * b[3];
  t += (dpint)a[11] * b[2];
  t += (dpint)a[12] * b[1];
  t += (dpint)a[13] * b[0];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v0 << 20u);
  s -= v6;
  t += (dpint)v7 * (dpint)p6;
  t += (dpint)v8 * (dpint)p5;
  t += (dpint)v9 * (dpint)p4;
  t += (dpint)v10 * (dpint)p3;
  t += (dpint)v11 * (dpint)p2;
  t += (dpint)v12 * (dpint)p1;
  t += (dpint)s;
  spint v13 = (((spint)t * ndash) & mask);
  t += (dpint)v13 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[1] * b[13];
  t += (dpint)a[2] * b[12];
  t += (dpint)a[3] * b[11];
  t += (dpint)a[4] * b[10];
  t += (dpint)a[5] * b[9];
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  t += (dpint)a[9] * b[5];
  t += (dpint)a[10] * b[4];
  t += (dpint)a[11] * b[3];
  t += (dpint)a[12] * b[2];
  t += (dpint)a[13] * b[1];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v1 << 20u);
  s -= v7;
  t += (dpint)v8 * (dpint)p6;
  t += (dpint)v9 * (dpint)p5;
  t += (dpint)v10 * (dpint)p4;
  t += (dpint)v11 * (dpint)p3;
  t += (dpint)v12 * (dpint)p2;
  t += (dpint)v13 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[2] * b[13];
  t += (dpint)a[3] * b[12];
  t += (dpint)a[4] * b[11];
  t += (dpint)a[5] * b[10];
  t += (dpint)a[6] * b[9];
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  t += (dpint)a[9] * b[6];
  t += (dpint)a[10] * b[5];
  t += (dpint)a[11] * b[4];
  t += (dpint)a[12] * b[3];
  t += (dpint)a[13] * b[2];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v2 << 20u);
  s -= v8;
  t += (dpint)v9 * (dpint)p6;
  t += (dpint)v10 * (dpint)p5;
  t += (dpint)v11 * (dpint)p4;
  t += (dpint)v12 * (dpint)p3;
  t += (dpint)v13 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[3] * b[13];
  t += (dpint)a[4] * b[12];
  t += (dpint)a[5] * b[11];
  t += (dpint)a[6] * b[10];
  t += (dpint)a[7] * b[9];
  t += (dpint)a[8] * b[8];
  t += (dpint)a[9] * b[7];
  t += (dpint)a[10] * b[6];
  t += (dpint)a[11] * b[5];
  t += (dpint)a[12] * b[4];
  t += (dpint)a[13] * b[3];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v3 << 20u);
  s -= v9;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  t += (dpint)v12 * (dpint)p4;
  t += (dpint)v13 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[4] * b[13];
  t += (dpint)a[5] * b[12];
  t += (dpint)a[6] * b[11];
  t += (dpint)a[7] * b[10];
  t += (dpint)a[8] * b[9];
  t += (dpint)a[9] * b[8];
  t += (dpint)a[10] * b[7];
  t += (dpint)a[11] * b[6];
  t += (dpint)a[12] * b[5];
  t += (dpint)a[13] * b[4];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 20u);
  s -= v10;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  t += (dpint)v13 * (dpint)p4;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[5] * b[13];
  t += (dpint)a[6] * b[12];
  t += (dpint)a[7] * b[11];
  t += (dpint)a[8] * b[10];
  t += (dpint)a[9] * b[9];
  t += (dpint)a[10] * b[8];
  t += (dpint)a[11] * b[7];
  t += (dpint)a[12] * b[6];
  t += (dpint)a[13] * b[5];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v5 << 20u);
  s -= v11;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[6] * b[13];
  t += (dpint)a[7] * b[12];
  t += (dpint)a[8] * b[11];
  t += (dpint)a[9] * b[10];
  t += (dpint)a[10] * b[9];
  t += (dpint)a[11] * b[8];
  t += (dpint)a[12] * b[7];
  t += (dpint)a[13] * b[6];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v6 << 20u);
  s -= v12;
  t += (dpint)v13 * (dpint)p6;
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v7 << 20u);
  s -= v13;
  t += (dpint)s;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v8 << 20u);
  t += (dpint)s;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v9 << 20u);
  t += (dpint)s;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v10 << 20u);
  t += (dpint)s;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v11 << 20u);
  t += (dpint)s;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v12 << 20u);
  t += (dpint)s;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[13];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v13 << 20u);
  t += (dpint)s;
  c[12] = ((spint)t & mask);
  t >>= 28;
  t -= (dpint)1u;
  c[13] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0xcc52973u;
  spint p1 = 0xec196acu;
  spint p2 = 0xa77aecu;
  spint p3 = 0xdb248bu;
  spint p4 = 0xddf581au;
  spint p5 = 0x81f4372u;
  spint p6 = 0xfc7634du;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x88fdc45u;
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (udpint)v0 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (udpint)v1 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  t += (udpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (udpint)v2 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p3;
  t += (udpint)v1 * p2;
  t += (udpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (udpint)v3 * p0;
  t >>= 28;
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
  t >>= 28;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p5;
  t += (udpint)v1 * p4;
  t += (udpint)v2 * p3;
  t += (udpint)v3 * p2;
  t += (udpint)v4 * p1;
  spint v5 = (((spint)t * ndash) & mask);
  t += (udpint)v5 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v0 * p6;
  t += (udpint)v1 * p5;
  t += (udpint)v2 * p4;
  t += (udpint)v3 * p3;
  t += (udpint)v4 * p2;
  t += (udpint)v5 * p1;
  spint v6 = (((spint)t * ndash) & mask);
  t += (udpint)v6 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)(spint)(q - v0);
  t += (udpint)v1 * p6;
  t += (udpint)v2 * p5;
  t += (udpint)v3 * p4;
  t += (udpint)v4 * p3;
  t += (udpint)v5 * p2;
  t += (udpint)v6 * p1;
  spint v7 = (((spint)t * ndash) & mask);
  t += (udpint)v7 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[8];
  tot += (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  spint s = (spint)mask;
  s -= v1;
  t += (udpint)v2 * p6;
  t += (udpint)v3 * p5;
  t += (udpint)v4 * p4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  t += (udpint)v7 * p1;
  t += (udpint)s;
  spint v8 = (((spint)t * ndash) & mask);
  t += (udpint)v8 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[9];
  tot += (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  s -= v2;
  t += (udpint)v3 * p6;
  t += (udpint)v4 * p5;
  t += (udpint)v5 * p4;
  t += (udpint)v6 * p3;
  t += (udpint)v7 * p2;
  t += (udpint)v8 * p1;
  t += (udpint)s;
  spint v9 = (((spint)t * ndash) & mask);
  t += (udpint)v9 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[10];
  tot += (udpint)a[1] * a[9];
  tot += (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  s = (spint)mask;
  s -= v3;
  t += (udpint)v4 * p6;
  t += (udpint)v5 * p5;
  t += (udpint)v6 * p4;
  t += (udpint)v7 * p3;
  t += (udpint)v8 * p2;
  t += (udpint)v9 * p1;
  t += (udpint)s;
  spint v10 = (((spint)t * ndash) & mask);
  t += (udpint)v10 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[11];
  tot += (udpint)a[1] * a[10];
  tot += (udpint)a[2] * a[9];
  tot += (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  s -= v4;
  t += (udpint)v5 * p6;
  t += (udpint)v6 * p5;
  t += (udpint)v7 * p4;
  t += (udpint)v8 * p3;
  t += (udpint)v9 * p2;
  t += (udpint)v10 * p1;
  t += (udpint)s;
  spint v11 = (((spint)t * ndash) & mask);
  t += (udpint)v11 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[12];
  tot += (udpint)a[1] * a[11];
  tot += (udpint)a[2] * a[10];
  tot += (udpint)a[3] * a[9];
  tot += (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  s = (spint)mask;
  s -= v5;
  t += (udpint)v6 * p6;
  t += (udpint)v7 * p5;
  t += (udpint)v8 * p4;
  t += (udpint)v9 * p3;
  t += (udpint)v10 * p2;
  t += (udpint)v11 * p1;
  t += (udpint)s;
  spint v12 = (((spint)t * ndash) & mask);
  t += (udpint)v12 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[13];
  tot += (udpint)a[1] * a[12];
  tot += (udpint)a[2] * a[11];
  tot += (udpint)a[3] * a[10];
  tot += (udpint)a[4] * a[9];
  tot += (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v0 << 20u;
  s -= v6;
  t += (udpint)v7 * p6;
  t += (udpint)v8 * p5;
  t += (udpint)v9 * p4;
  t += (udpint)v10 * p3;
  t += (udpint)v11 * p2;
  t += (udpint)v12 * p1;
  t += (udpint)s;
  spint v13 = (((spint)t * ndash) & mask);
  t += (udpint)v13 * p0;
  t >>= 28;
  tot = (udpint)a[1] * a[13];
  tot += (udpint)a[2] * a[12];
  tot += (udpint)a[3] * a[11];
  tot += (udpint)a[4] * a[10];
  tot += (udpint)a[5] * a[9];
  tot += (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  s = (spint)mask;
  t += (udpint)v1 << 20u;
  s -= v7;
  t += (udpint)v8 * p6;
  t += (udpint)v9 * p5;
  t += (udpint)v10 * p4;
  t += (udpint)v11 * p3;
  t += (udpint)v12 * p2;
  t += (udpint)v13 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[13];
  tot += (udpint)a[3] * a[12];
  tot += (udpint)a[4] * a[11];
  tot += (udpint)a[5] * a[10];
  tot += (udpint)a[6] * a[9];
  tot += (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 << 20u;
  s -= v8;
  t += (udpint)v9 * p6;
  t += (udpint)v10 * p5;
  t += (udpint)v11 * p4;
  t += (udpint)v12 * p3;
  t += (udpint)v13 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[13];
  tot += (udpint)a[4] * a[12];
  tot += (udpint)a[5] * a[11];
  tot += (udpint)a[6] * a[10];
  tot += (udpint)a[7] * a[9];
  tot *= 2;
  tot += (udpint)a[8] * a[8];
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 << 20u;
  s -= v9;
  t += (udpint)v10 * p6;
  t += (udpint)v11 * p5;
  t += (udpint)v12 * p4;
  t += (udpint)v13 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 20u;
  s -= v10;
  t += (udpint)v11 * p6;
  t += (udpint)v12 * p5;
  t += (udpint)v13 * p4;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 << 20u;
  s -= v11;
  t += (udpint)v12 * p6;
  t += (udpint)v13 * p5;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 << 20u;
  s -= v12;
  t += (udpint)v13 * p6;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  s = (spint)mask;
  t += (udpint)v7 << 20u;
  s -= v13;
  t += (udpint)s;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v8 << 20u;
  t += (udpint)s;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  s = (spint)mask;
  t += (udpint)v9 << 20u;
  t += (udpint)s;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v10 << 20u;
  t += (udpint)s;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  s = (spint)mask;
  t += (udpint)v11 << 20u;
  t += (udpint)s;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v12 << 20u;
  t += (udpint)s;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[13];
  t += tot;
  s = (spint)mask;
  t += (udpint)v13 << 20u;
  t += (udpint)s;
  c[12] = ((spint)t & mask);
  t >>= 28;
  t -= 1u;
  c[13] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 14; i++) {
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
  spint x[14];
  spint t0[14];
  spint t1[14];
  spint t2[14];
  spint t3[14];
  spint t4[14];
  spint t5[14];
  spint t6[14];
  spint t7[14];
  spint t8[14];
  spint t9[14];
  spint t10[14];
  modcpy(w, x);
  modsqr(x, t3);
  modmul(x, t3, t1);
  modmul(t3, t1, t0);
  modmul(t3, t0, t2);
  modmul(t3, t2, t4);
  modmul(t3, t4, z);
  modmul(t3, z, t5);
  modmul(t3, t5, t3);
  modsqr(t3, t6);
  modmul(x, t6, t6);
  modcpy(t6, t8);
  modnsqr(t8, 2);
  modsqr(t8, t9);
  modsqr(t9, t7);
  modcpy(t7, t10);
  modnsqr(t10, 5);
  modmul(t7, t10, t7);
  modcpy(t7, t10);
  modnsqr(t10, 10);
  modmul(t7, t10, t7);
  modcpy(t7, t10);
  modnsqr(t10, 4);
  modmul(t9, t10, t9);
  modnsqr(t9, 21);
  modmul(t7, t9, t7);
  modcpy(t7, t9);
  modnsqr(t9, 3);
  modmul(t8, t9, t8);
  modnsqr(t8, 47);
  modmul(t7, t8, t7);
  modcpy(t7, t8);
  modnsqr(t8, 95);
  modmul(t7, t8, t7);
  modmul(t3, t7, t7);
  modnsqr(t7, 6);
  modmul(t2, t7, t7);
  modnsqr(t7, 3);
  modmul(t1, t7, t7);
  modnsqr(t7, 7);
  modmul(t5, t7, t7);
  modnsqr(t7, 6);
  modmul(t5, t7, t7);
  modsqr(t7, t7);
  modmul(x, t7, t7);
  modnsqr(t7, 11);
  modmul(t6, t7, t7);
  modnsqr(t7, 2);
  modmul(x, t7, t7);
  modnsqr(t7, 8);
  modmul(t5, t7, t7);
  modnsqr(t7, 2);
  modmul(t1, t7, t7);
  modnsqr(t7, 6);
  modmul(z, t7, t7);
  modnsqr(t7, 4);
  modmul(t2, t7, t7);
  modnsqr(t7, 6);
  modmul(t6, t7, t6);
  modnsqr(t6, 5);
  modmul(z, t6, t6);
  modnsqr(t6, 10);
  modmul(t5, t6, t6);
  modnsqr(t6, 9);
  modmul(t5, t6, t5);
  modnsqr(t5, 4);
  modmul(z, t5, t5);
  modnsqr(t5, 6);
  modmul(t4, t5, t4);
  modnsqr(t4, 3);
  modmul(x, t4, t4);
  modnsqr(t4, 7);
  modmul(z, t4, t4);
  modnsqr(t4, 7);
  modmul(t0, t4, t4);
  modnsqr(t4, 5);
  modmul(t2, t4, t4);
  modnsqr(t4, 5);
  modmul(t3, t4, t3);
  modnsqr(t3, 5);
  modmul(z, t3, t3);
  modnsqr(t3, 4);
  modmul(z, t3, t3);
  modnsqr(t3, 5);
  modmul(t2, t3, t2);
  modnsqr(t2, 3);
  modmul(t1, t2, t2);
  modnsqr(t2, 7);
  modmul(t1, t2, t2);
  modnsqr(t2, 6);
  modmul(z, t2, t2);
  modnsqr(t2, 4);
  modmul(t0, t2, t2);
  modnsqr(t2, 3);
  modmul(t1, t2, t2);
  modnsqr(t2, 4);
  modmul(t1, t2, t2);
  modnsqr(t2, 4);
  modmul(t1, t2, t1);
  modnsqr(t1, 6);
  modmul(t0, t1, t1);
  modnsqr(t1, 5);
  modmul(t0, t1, t0);
  modnsqr(t0, 6);
  modmul(z, t0, z);
  modsqr(z, z);
  modmul(x, z, z);
  modnsqr(z, 2);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[14];
  spint t[14];
  if (h == NULL) {
    modpro(x, t);
  } else {
    modcpy(h, t);
  }
  modcpy(x, s);
  modnsqr(t, 2);
  modmul(s, t, z);
}

// Convert m to n-residue form, n=nres(m)
static void nres(const spint *m, spint *n) {
  const spint c[14] = {0x60e3cb4u, 0x35fd093u, 0x90eb460u, 0x73c4e12u,
                       0xff2dbd2u, 0x43a6addu, 0x2d164eeu, 0x1cc5bf0u,
                       0x9174aabu, 0x95d40d4u, 0xa282668u, 0x3fb05b7u,
                       0xb39bf21u, 0xee012u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[14];
  c[0] = 1;
  for (i = 1; i < 14; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[14];
  for (i = 0; i < 14; i++) {
    b[i] = n[i];
    t[i] = n[i + 14];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[14];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 14; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[14];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 14; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 14; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 14; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 14; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[14];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
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
  for (i = 0; i < 14; i++) {
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
  for (i = 0; i < 14; i++) {
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
  spint s[14];
  spint y[14];
  if (h == NULL) {
    modpro(x, y);
  } else {
    modcpy(h, y);
  }
  modmul(y, x, s);
  modcpy(s, r);
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
  int i;
  a[13] = ((a[13] << n)) | (a[12] >> (28u - n));
  for (i = 12; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 13; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[13] = a[13] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 28u;
  unsigned int m = r % 28u;
  modzer(a);
  if (r >= 48 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
  nres(a, a);
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[14];
  redc(a, c);
  for (i = 47; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 14; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 48; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[14];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[14], d[14];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 14; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
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
    if (ecn384isinf(P)) {
        printf("P= O\n");
    } else {
        char x[BYTES],y[BYTES];
        char buff[(2*BYTES)+1];
        ecn384get(P,x,y);
        toHex(BYTES,x,buff);
        printf("Px= "); puts(buff);
        toHex(BYTES,y,buff);
        printf("Py= "); puts(buff);
    }
}
*/

// reduce 56 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^376.x + y, where x and y < q (x is top 9 bytes, y is bottom 47 bytes)
// Important that x and y < q
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,c;

    mod2r(8*(BYTES-1),c); // 2^376

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
    modadd(x,y,r);    // 2^376.x + y
}

//#define PREHASHED   // define for test vectors

// API

#include "tlsecc.h"

// Input private key - 48 random bytes
// Output public key - 97 bytes (0x04<x>|<y>), or 49 if compressed (0x02<x>.. or 0x03<x>)

void NIST384_KEY_PAIR(int compress,char *SK,char *PK)
{
    point P;
    ecn384gen(&P);
    ecn384mul(SK,&P);

    if (compress) {
        PK[0]=0x02+ecn384get(&P,&PK[1],NULL); // 0x02 or 0x03
    } else {
        PK[0]=0x04; // no compression
        ecn384get(&P,&PK[1],&PK[BYTES+1]);  // get x and y
    }
}

// input private key, per-message random number, message to be signed. Output signature.
// ran must be Nbytes+8 in length, in this case 56 bytes
void NIST384_SIGN(char *prv,char *ran,int mlen,char *m,char *sig)
{
    char h[BYTES];
    point R;
    gel e,r,s,k;

#ifdef PREHASHED
    modimp(m,e);
#else
    int i;
    hash384 sh384;
    HASH384_init(&sh384);
    for (i=0;i<mlen;i++)
        HASH384_process(&sh384,m[i]);
    HASH384_hash(&sh384,h); 

    modimp(h,e);
#endif

    ecn384gen(&R);
    modimp(prv,s);

    reduce(ran,k);
    modexp(k,h);
    ecn384mul(h,&R);
    modinv(k,NULL,k);

    ecn384get(&R,h,NULL);
    modimp(h,r);

    modmul(s,r,s);
    modadd(s,e,s);
    modmul(s,k,s);
    modzer(k);

    modexp(r,sig);
    modexp(s,&sig[BYTES]);
}

// input public key, message and signature
int NIST384_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    point G,Q;
    int i;
    char rb[BYTES],u[BYTES],v[BYTES];
    gel e,r,s,rds;
#ifdef PREHASHED
    modimp(m,e);
#else
    char h[BYTES];
    hash384 sh384;
    HASH384_init(&sh384);
    for (i=0;i<mlen;i++)
        HASH384_process(&sh384,m[i]);
    HASH384_hash(&sh384,h); 

    modimp(h,e);
#endif

    ecn384gen(&G);

// import from signature
    if (!modimp(sig,r)) return 0; // if not in range
    if (!modimp(&sig[BYTES],s)) return 0;

    if (modis0(r) || modis0(s)) return 0;
    modinv(s,NULL,s);
    modmul(r,s,rds); modexp(rds,v);  // export to byte array
    modmul(s,e,s); modexp(s,u); 

    if (pub[0]==0x04) {
        ecn384set(0,&pub[1],&pub[BYTES+1],&Q);
    } else {
        ecn384set((int)pub[0]&1,&pub[1],NULL,&Q);
    }

    ecn384mul2(u,&G,v,&Q,&Q);
    if (ecn384isinf(&Q)) return 0;

    ecn384get(&Q,rb,NULL);

    modimp(rb,e);
    if (modcmp(r,e)) return 1;
    return 0;
}

int NIST384_SHARED_SECRET(char *SK,char *PK,char *SS)
{
    point P;

    if (PK[0]==0x04) {
        ecn384set(0,&PK[1],&PK[BYTES+1],&P);
    } else {
        ecn384set((int)PK[0]&1,&PK[1],NULL,&P);
    }

    ecn384mul(SK,&P);

    ecn384get(&P,SS,NULL);  // x coordinate
    if (ecn384isinf(&P)) return 0;
    return 1;
}
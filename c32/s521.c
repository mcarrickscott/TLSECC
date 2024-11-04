
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

// Command line : python monty.py 32 nist521

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 19
#define Radix 28
#define Nbits 521
#define Nbytes 66

#define MONTGOMERY
#define NIST521

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 28u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 28u;
  n[0] &= mask;
  for (i = 1; i < 18; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[18] += (spint)carry;
  return -((n[18] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x1386409u) & carry;
  n[1] += ((spint)0x6fb71e9u) & carry;
  n[2] += ((spint)0xc47aebbu) & carry;
  n[3] += ((spint)0xc9b8899u) & carry;
  n[4] += ((spint)0x5d03bb5u) & carry;
  n[5] += ((spint)0x48f709au) & carry;
  n[6] += ((spint)0xb7fcc01u) & carry;
  n[7] += ((spint)0xbf2f966u) & carry;
  n[8] += ((spint)0x1868783u) & carry;
  n[9] += ((spint)0xfffffa5u) & carry;
  n[10] -= (spint)1u & carry;
  n[18] += ((spint)0x20000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] -= (spint)0x1386409u;
  n[1] -= (spint)0x6fb71e9u;
  n[2] -= (spint)0xc47aebbu;
  n[3] -= (spint)0xc9b8899u;
  n[4] -= (spint)0x5d03bb5u;
  n[5] -= (spint)0x48f709au;
  n[6] -= (spint)0xb7fcc01u;
  n[7] -= (spint)0xbf2f966u;
  n[8] -= (spint)0x1868783u;
  n[9] -= (spint)0xfffffa5u;
  n[10] += (spint)1u;
  n[18] -= (spint)0x20000u;
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
  n[14] = a[14] + b[14];
  n[15] = a[15] + b[15];
  n[16] = a[16] + b[16];
  n[17] = a[17] + b[17];
  n[18] = a[18] + b[18];
  n[0] -= (spint)0x270c812u;
  n[1] -= (spint)0xdf6e3d2u;
  n[2] -= (spint)0x188f5d76u;
  n[3] -= (spint)0x19371132u;
  n[4] -= (spint)0xba0776au;
  n[5] -= (spint)0x91ee134u;
  n[6] -= (spint)0x16ff9802u;
  n[7] -= (spint)0x17e5f2ccu;
  n[8] -= (spint)0x30d0f06u;
  n[9] -= (spint)0x1fffff4au;
  n[10] += (spint)2u;
  n[18] -= (spint)0x40000u;
  carry = prop(n);
  n[0] += ((spint)0x270c812u) & carry;
  n[1] += ((spint)0xdf6e3d2u) & carry;
  n[2] += ((spint)0x188f5d76u) & carry;
  n[3] += ((spint)0x19371132u) & carry;
  n[4] += ((spint)0xba0776au) & carry;
  n[5] += ((spint)0x91ee134u) & carry;
  n[6] += ((spint)0x16ff9802u) & carry;
  n[7] += ((spint)0x17e5f2ccu) & carry;
  n[8] += ((spint)0x30d0f06u) & carry;
  n[9] += ((spint)0x1fffff4au) & carry;
  n[10] -= (spint)2u & carry;
  n[18] += ((spint)0x40000u) & carry;
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
  n[14] = a[14] - b[14];
  n[15] = a[15] - b[15];
  n[16] = a[16] - b[16];
  n[17] = a[17] - b[17];
  n[18] = a[18] - b[18];
  carry = prop(n);
  n[0] += ((spint)0x270c812u) & carry;
  n[1] += ((spint)0xdf6e3d2u) & carry;
  n[2] += ((spint)0x188f5d76u) & carry;
  n[3] += ((spint)0x19371132u) & carry;
  n[4] += ((spint)0xba0776au) & carry;
  n[5] += ((spint)0x91ee134u) & carry;
  n[6] += ((spint)0x16ff9802u) & carry;
  n[7] += ((spint)0x17e5f2ccu) & carry;
  n[8] += ((spint)0x30d0f06u) & carry;
  n[9] += ((spint)0x1fffff4au) & carry;
  n[10] -= (spint)2u & carry;
  n[18] += ((spint)0x40000u) & carry;
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
  n[14] = (spint)0 - b[14];
  n[15] = (spint)0 - b[15];
  n[16] = (spint)0 - b[16];
  n[17] = (spint)0 - b[17];
  n[18] = (spint)0 - b[18];
  carry = prop(n);
  n[0] += ((spint)0x270c812u) & carry;
  n[1] += ((spint)0xdf6e3d2u) & carry;
  n[2] += ((spint)0x188f5d76u) & carry;
  n[3] += ((spint)0x19371132u) & carry;
  n[4] += ((spint)0xba0776au) & carry;
  n[5] += ((spint)0x91ee134u) & carry;
  n[6] += ((spint)0x16ff9802u) & carry;
  n[7] += ((spint)0x17e5f2ccu) & carry;
  n[8] += ((spint)0x30d0f06u) & carry;
  n[9] += ((spint)0x1fffff4au) & carry;
  n[10] -= (spint)2u & carry;
  n[18] += ((spint)0x40000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1749402643979958766
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0x1386409u;
  spint p1 = 0x6fb71e9u;
  spint p2 = 0xc47aebbu;
  spint p3 = 0xc9b8899u;
  spint p4 = 0x5d03bb5u;
  spint p5 = 0x48f709au;
  spint p6 = 0xb7fcc01u;
  spint p7 = 0xbf2f966u;
  spint p8 = 0x1868783u;
  spint p9 = 0xfffffa5u;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x9a995c7u;
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
  t += (dpint)v0 * (dpint)p7;
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
  t += (dpint)v0 * (dpint)p8;
  t += (dpint)v1 * (dpint)p7;
  t += (dpint)v2 * (dpint)p6;
  t += (dpint)v3 * (dpint)p5;
  t += (dpint)v4 * (dpint)p4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  t += (dpint)v7 * (dpint)p1;
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
  t += (dpint)v0 * (dpint)p9;
  t += (dpint)v1 * (dpint)p8;
  t += (dpint)v2 * (dpint)p7;
  t += (dpint)v3 * (dpint)p6;
  t += (dpint)v4 * (dpint)p5;
  t += (dpint)v5 * (dpint)p4;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)v7 * (dpint)p2;
  t += (dpint)v8 * (dpint)p1;
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
  t += (dpint)(spint)(q - v0);
  t += (dpint)v1 * (dpint)p9;
  t += (dpint)v2 * (dpint)p8;
  t += (dpint)v3 * (dpint)p7;
  t += (dpint)v4 * (dpint)p6;
  t += (dpint)v5 * (dpint)p5;
  t += (dpint)v6 * (dpint)p4;
  t += (dpint)v7 * (dpint)p3;
  t += (dpint)v8 * (dpint)p2;
  t += (dpint)v9 * (dpint)p1;
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
  spint s = (spint)mask;
  s -= v1;
  t += (dpint)v2 * (dpint)p9;
  t += (dpint)v3 * (dpint)p8;
  t += (dpint)v4 * (dpint)p7;
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
  s -= v2;
  t += (dpint)v3 * (dpint)p9;
  t += (dpint)v4 * (dpint)p8;
  t += (dpint)v5 * (dpint)p7;
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
  s -= v3;
  t += (dpint)v4 * (dpint)p9;
  t += (dpint)v5 * (dpint)p8;
  t += (dpint)v6 * (dpint)p7;
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
  t += (dpint)a[0] * b[14];
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
  t += (dpint)a[14] * b[0];
  s = (spint)mask;
  s -= v4;
  t += (dpint)v5 * (dpint)p9;
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)v8 * (dpint)p6;
  t += (dpint)v9 * (dpint)p5;
  t += (dpint)v10 * (dpint)p4;
  t += (dpint)v11 * (dpint)p3;
  t += (dpint)v12 * (dpint)p2;
  t += (dpint)v13 * (dpint)p1;
  t += (dpint)s;
  spint v14 = (((spint)t * ndash) & mask);
  t += (dpint)v14 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[15];
  t += (dpint)a[1] * b[14];
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
  t += (dpint)a[14] * b[1];
  t += (dpint)a[15] * b[0];
  s = (spint)mask;
  s -= v5;
  t += (dpint)v6 * (dpint)p9;
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  t += (dpint)v9 * (dpint)p6;
  t += (dpint)v10 * (dpint)p5;
  t += (dpint)v11 * (dpint)p4;
  t += (dpint)v12 * (dpint)p3;
  t += (dpint)v13 * (dpint)p2;
  t += (dpint)v14 * (dpint)p1;
  t += (dpint)s;
  spint v15 = (((spint)t * ndash) & mask);
  t += (dpint)v15 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[16];
  t += (dpint)a[1] * b[15];
  t += (dpint)a[2] * b[14];
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
  t += (dpint)a[14] * b[2];
  t += (dpint)a[15] * b[1];
  t += (dpint)a[16] * b[0];
  s = (spint)mask;
  s -= v6;
  t += (dpint)v7 * (dpint)p9;
  t += (dpint)v8 * (dpint)p8;
  t += (dpint)v9 * (dpint)p7;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  t += (dpint)v12 * (dpint)p4;
  t += (dpint)v13 * (dpint)p3;
  t += (dpint)v14 * (dpint)p2;
  t += (dpint)v15 * (dpint)p1;
  t += (dpint)s;
  spint v16 = (((spint)t * ndash) & mask);
  t += (dpint)v16 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[17];
  t += (dpint)a[1] * b[16];
  t += (dpint)a[2] * b[15];
  t += (dpint)a[3] * b[14];
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
  t += (dpint)a[14] * b[3];
  t += (dpint)a[15] * b[2];
  t += (dpint)a[16] * b[1];
  t += (dpint)a[17] * b[0];
  s = (spint)mask;
  s -= v7;
  t += (dpint)v8 * (dpint)p9;
  t += (dpint)v9 * (dpint)p8;
  t += (dpint)v10 * (dpint)p7;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  t += (dpint)v13 * (dpint)p4;
  t += (dpint)v14 * (dpint)p3;
  t += (dpint)v15 * (dpint)p2;
  t += (dpint)v16 * (dpint)p1;
  t += (dpint)s;
  spint v17 = (((spint)t * ndash) & mask);
  t += (dpint)v17 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[0] * b[18];
  t += (dpint)a[1] * b[17];
  t += (dpint)a[2] * b[16];
  t += (dpint)a[3] * b[15];
  t += (dpint)a[4] * b[14];
  t += (dpint)a[5] * b[13];
  t += (dpint)a[6] * b[12];
  t += (dpint)a[7] * b[11];
  t += (dpint)a[8] * b[10];
  t += (dpint)a[9] * b[9];
  t += (dpint)a[10] * b[8];
  t += (dpint)a[11] * b[7];
  t += (dpint)a[12] * b[6];
  t += (dpint)a[13] * b[5];
  t += (dpint)a[14] * b[4];
  t += (dpint)a[15] * b[3];
  t += (dpint)a[16] * b[2];
  t += (dpint)a[17] * b[1];
  t += (dpint)a[18] * b[0];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v0 << 17u);
  s -= v8;
  t += (dpint)v9 * (dpint)p9;
  t += (dpint)v10 * (dpint)p8;
  t += (dpint)v11 * (dpint)p7;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
  t += (dpint)v14 * (dpint)p4;
  t += (dpint)v15 * (dpint)p3;
  t += (dpint)v16 * (dpint)p2;
  t += (dpint)v17 * (dpint)p1;
  t += (dpint)s;
  spint v18 = (((spint)t * ndash) & mask);
  t += (dpint)v18 * (dpint)p0;
  t >>= 28;
  t += (dpint)a[1] * b[18];
  t += (dpint)a[2] * b[17];
  t += (dpint)a[3] * b[16];
  t += (dpint)a[4] * b[15];
  t += (dpint)a[5] * b[14];
  t += (dpint)a[6] * b[13];
  t += (dpint)a[7] * b[12];
  t += (dpint)a[8] * b[11];
  t += (dpint)a[9] * b[10];
  t += (dpint)a[10] * b[9];
  t += (dpint)a[11] * b[8];
  t += (dpint)a[12] * b[7];
  t += (dpint)a[13] * b[6];
  t += (dpint)a[14] * b[5];
  t += (dpint)a[15] * b[4];
  t += (dpint)a[16] * b[3];
  t += (dpint)a[17] * b[2];
  t += (dpint)a[18] * b[1];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v1 << 17u);
  s -= v9;
  t += (dpint)v10 * (dpint)p9;
  t += (dpint)v11 * (dpint)p8;
  t += (dpint)v12 * (dpint)p7;
  t += (dpint)v13 * (dpint)p6;
  t += (dpint)v14 * (dpint)p5;
  t += (dpint)v15 * (dpint)p4;
  t += (dpint)v16 * (dpint)p3;
  t += (dpint)v17 * (dpint)p2;
  t += (dpint)v18 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[2] * b[18];
  t += (dpint)a[3] * b[17];
  t += (dpint)a[4] * b[16];
  t += (dpint)a[5] * b[15];
  t += (dpint)a[6] * b[14];
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  t += (dpint)a[14] * b[6];
  t += (dpint)a[15] * b[5];
  t += (dpint)a[16] * b[4];
  t += (dpint)a[17] * b[3];
  t += (dpint)a[18] * b[2];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v2 << 17u);
  s -= v10;
  t += (dpint)v11 * (dpint)p9;
  t += (dpint)v12 * (dpint)p8;
  t += (dpint)v13 * (dpint)p7;
  t += (dpint)v14 * (dpint)p6;
  t += (dpint)v15 * (dpint)p5;
  t += (dpint)v16 * (dpint)p4;
  t += (dpint)v17 * (dpint)p3;
  t += (dpint)v18 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[3] * b[18];
  t += (dpint)a[4] * b[17];
  t += (dpint)a[5] * b[16];
  t += (dpint)a[6] * b[15];
  t += (dpint)a[7] * b[14];
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  t += (dpint)a[14] * b[7];
  t += (dpint)a[15] * b[6];
  t += (dpint)a[16] * b[5];
  t += (dpint)a[17] * b[4];
  t += (dpint)a[18] * b[3];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v3 << 17u);
  s -= v11;
  t += (dpint)v12 * (dpint)p9;
  t += (dpint)v13 * (dpint)p8;
  t += (dpint)v14 * (dpint)p7;
  t += (dpint)v15 * (dpint)p6;
  t += (dpint)v16 * (dpint)p5;
  t += (dpint)v17 * (dpint)p4;
  t += (dpint)v18 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[4] * b[18];
  t += (dpint)a[5] * b[17];
  t += (dpint)a[6] * b[16];
  t += (dpint)a[7] * b[15];
  t += (dpint)a[8] * b[14];
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)a[14] * b[8];
  t += (dpint)a[15] * b[7];
  t += (dpint)a[16] * b[6];
  t += (dpint)a[17] * b[5];
  t += (dpint)a[18] * b[4];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 17u);
  s -= v12;
  t += (dpint)v13 * (dpint)p9;
  t += (dpint)v14 * (dpint)p8;
  t += (dpint)v15 * (dpint)p7;
  t += (dpint)v16 * (dpint)p6;
  t += (dpint)v17 * (dpint)p5;
  t += (dpint)v18 * (dpint)p4;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[5] * b[18];
  t += (dpint)a[6] * b[17];
  t += (dpint)a[7] * b[16];
  t += (dpint)a[8] * b[15];
  t += (dpint)a[9] * b[14];
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)a[14] * b[9];
  t += (dpint)a[15] * b[8];
  t += (dpint)a[16] * b[7];
  t += (dpint)a[17] * b[6];
  t += (dpint)a[18] * b[5];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v5 << 17u);
  s -= v13;
  t += (dpint)v14 * (dpint)p9;
  t += (dpint)v15 * (dpint)p8;
  t += (dpint)v16 * (dpint)p7;
  t += (dpint)v17 * (dpint)p6;
  t += (dpint)v18 * (dpint)p5;
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[6] * b[18];
  t += (dpint)a[7] * b[17];
  t += (dpint)a[8] * b[16];
  t += (dpint)a[9] * b[15];
  t += (dpint)a[10] * b[14];
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)a[14] * b[10];
  t += (dpint)a[15] * b[9];
  t += (dpint)a[16] * b[8];
  t += (dpint)a[17] * b[7];
  t += (dpint)a[18] * b[6];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v6 << 17u);
  s -= v14;
  t += (dpint)v15 * (dpint)p9;
  t += (dpint)v16 * (dpint)p8;
  t += (dpint)v17 * (dpint)p7;
  t += (dpint)v18 * (dpint)p6;
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[18];
  t += (dpint)a[8] * b[17];
  t += (dpint)a[9] * b[16];
  t += (dpint)a[10] * b[15];
  t += (dpint)a[11] * b[14];
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)a[14] * b[11];
  t += (dpint)a[15] * b[10];
  t += (dpint)a[16] * b[9];
  t += (dpint)a[17] * b[8];
  t += (dpint)a[18] * b[7];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v7 << 17u);
  s -= v15;
  t += (dpint)v16 * (dpint)p9;
  t += (dpint)v17 * (dpint)p8;
  t += (dpint)v18 * (dpint)p7;
  t += (dpint)s;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[18];
  t += (dpint)a[9] * b[17];
  t += (dpint)a[10] * b[16];
  t += (dpint)a[11] * b[15];
  t += (dpint)a[12] * b[14];
  t += (dpint)a[13] * b[13];
  t += (dpint)a[14] * b[12];
  t += (dpint)a[15] * b[11];
  t += (dpint)a[16] * b[10];
  t += (dpint)a[17] * b[9];
  t += (dpint)a[18] * b[8];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v8 << 17u);
  s -= v16;
  t += (dpint)v17 * (dpint)p9;
  t += (dpint)v18 * (dpint)p8;
  t += (dpint)s;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[18];
  t += (dpint)a[10] * b[17];
  t += (dpint)a[11] * b[16];
  t += (dpint)a[12] * b[15];
  t += (dpint)a[13] * b[14];
  t += (dpint)a[14] * b[13];
  t += (dpint)a[15] * b[12];
  t += (dpint)a[16] * b[11];
  t += (dpint)a[17] * b[10];
  t += (dpint)a[18] * b[9];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v9 << 17u);
  s -= v17;
  t += (dpint)v18 * (dpint)p9;
  t += (dpint)s;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[18];
  t += (dpint)a[11] * b[17];
  t += (dpint)a[12] * b[16];
  t += (dpint)a[13] * b[15];
  t += (dpint)a[14] * b[14];
  t += (dpint)a[15] * b[13];
  t += (dpint)a[16] * b[12];
  t += (dpint)a[17] * b[11];
  t += (dpint)a[18] * b[10];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v10 << 17u);
  s -= v18;
  t += (dpint)s;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[18];
  t += (dpint)a[12] * b[17];
  t += (dpint)a[13] * b[16];
  t += (dpint)a[14] * b[15];
  t += (dpint)a[15] * b[14];
  t += (dpint)a[16] * b[13];
  t += (dpint)a[17] * b[12];
  t += (dpint)a[18] * b[11];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v11 << 17u);
  t += (dpint)s;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[18];
  t += (dpint)a[13] * b[17];
  t += (dpint)a[14] * b[16];
  t += (dpint)a[15] * b[15];
  t += (dpint)a[16] * b[14];
  t += (dpint)a[17] * b[13];
  t += (dpint)a[18] * b[12];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v12 << 17u);
  t += (dpint)s;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[18];
  t += (dpint)a[14] * b[17];
  t += (dpint)a[15] * b[16];
  t += (dpint)a[16] * b[15];
  t += (dpint)a[17] * b[14];
  t += (dpint)a[18] * b[13];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v13 << 17u);
  t += (dpint)s;
  c[12] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[14] * b[18];
  t += (dpint)a[15] * b[17];
  t += (dpint)a[16] * b[16];
  t += (dpint)a[17] * b[15];
  t += (dpint)a[18] * b[14];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v14 << 17u);
  t += (dpint)s;
  c[13] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[15] * b[18];
  t += (dpint)a[16] * b[17];
  t += (dpint)a[17] * b[16];
  t += (dpint)a[18] * b[15];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v15 << 17u);
  t += (dpint)s;
  c[14] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[16] * b[18];
  t += (dpint)a[17] * b[17];
  t += (dpint)a[18] * b[16];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v16 << 17u);
  t += (dpint)s;
  c[15] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[17] * b[18];
  t += (dpint)a[18] * b[17];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v17 << 17u);
  t += (dpint)s;
  c[16] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[18] * b[18];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v18 << 17u);
  t += (dpint)s;
  c[17] = ((spint)t & mask);
  t >>= 28;
  t -= (dpint)1u;
  c[18] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0x1386409u;
  spint p1 = 0x6fb71e9u;
  spint p2 = 0xc47aebbu;
  spint p3 = 0xc9b8899u;
  spint p4 = 0x5d03bb5u;
  spint p5 = 0x48f709au;
  spint p6 = 0xb7fcc01u;
  spint p7 = 0xbf2f966u;
  spint p8 = 0x1868783u;
  spint p9 = 0xfffffa5u;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x9a995c7u;
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
  t += (udpint)v0 * p7;
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
  t += (udpint)v0 * p8;
  t += (udpint)v1 * p7;
  t += (udpint)v2 * p6;
  t += (udpint)v3 * p5;
  t += (udpint)v4 * p4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  t += (udpint)v7 * p1;
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
  t += (udpint)v0 * p9;
  t += (udpint)v1 * p8;
  t += (udpint)v2 * p7;
  t += (udpint)v3 * p6;
  t += (udpint)v4 * p5;
  t += (udpint)v5 * p4;
  t += (udpint)v6 * p3;
  t += (udpint)v7 * p2;
  t += (udpint)v8 * p1;
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
  t += (udpint)(spint)(q - v0);
  t += (udpint)v1 * p9;
  t += (udpint)v2 * p8;
  t += (udpint)v3 * p7;
  t += (udpint)v4 * p6;
  t += (udpint)v5 * p5;
  t += (udpint)v6 * p4;
  t += (udpint)v7 * p3;
  t += (udpint)v8 * p2;
  t += (udpint)v9 * p1;
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
  spint s = (spint)mask;
  s -= v1;
  t += (udpint)v2 * p9;
  t += (udpint)v3 * p8;
  t += (udpint)v4 * p7;
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
  s -= v2;
  t += (udpint)v3 * p9;
  t += (udpint)v4 * p8;
  t += (udpint)v5 * p7;
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
  s -= v3;
  t += (udpint)v4 * p9;
  t += (udpint)v5 * p8;
  t += (udpint)v6 * p7;
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
  tot = (udpint)a[0] * a[14];
  tot += (udpint)a[1] * a[13];
  tot += (udpint)a[2] * a[12];
  tot += (udpint)a[3] * a[11];
  tot += (udpint)a[4] * a[10];
  tot += (udpint)a[5] * a[9];
  tot += (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  s = (spint)mask;
  s -= v4;
  t += (udpint)v5 * p9;
  t += (udpint)v6 * p8;
  t += (udpint)v7 * p7;
  t += (udpint)v8 * p6;
  t += (udpint)v9 * p5;
  t += (udpint)v10 * p4;
  t += (udpint)v11 * p3;
  t += (udpint)v12 * p2;
  t += (udpint)v13 * p1;
  t += (udpint)s;
  spint v14 = (((spint)t * ndash) & mask);
  t += (udpint)v14 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[15];
  tot += (udpint)a[1] * a[14];
  tot += (udpint)a[2] * a[13];
  tot += (udpint)a[3] * a[12];
  tot += (udpint)a[4] * a[11];
  tot += (udpint)a[5] * a[10];
  tot += (udpint)a[6] * a[9];
  tot += (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  s -= v5;
  t += (udpint)v6 * p9;
  t += (udpint)v7 * p8;
  t += (udpint)v8 * p7;
  t += (udpint)v9 * p6;
  t += (udpint)v10 * p5;
  t += (udpint)v11 * p4;
  t += (udpint)v12 * p3;
  t += (udpint)v13 * p2;
  t += (udpint)v14 * p1;
  t += (udpint)s;
  spint v15 = (((spint)t * ndash) & mask);
  t += (udpint)v15 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[16];
  tot += (udpint)a[1] * a[15];
  tot += (udpint)a[2] * a[14];
  tot += (udpint)a[3] * a[13];
  tot += (udpint)a[4] * a[12];
  tot += (udpint)a[5] * a[11];
  tot += (udpint)a[6] * a[10];
  tot += (udpint)a[7] * a[9];
  tot *= 2;
  tot += (udpint)a[8] * a[8];
  t += tot;
  s = (spint)mask;
  s -= v6;
  t += (udpint)v7 * p9;
  t += (udpint)v8 * p8;
  t += (udpint)v9 * p7;
  t += (udpint)v10 * p6;
  t += (udpint)v11 * p5;
  t += (udpint)v12 * p4;
  t += (udpint)v13 * p3;
  t += (udpint)v14 * p2;
  t += (udpint)v15 * p1;
  t += (udpint)s;
  spint v16 = (((spint)t * ndash) & mask);
  t += (udpint)v16 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[17];
  tot += (udpint)a[1] * a[16];
  tot += (udpint)a[2] * a[15];
  tot += (udpint)a[3] * a[14];
  tot += (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  s -= v7;
  t += (udpint)v8 * p9;
  t += (udpint)v9 * p8;
  t += (udpint)v10 * p7;
  t += (udpint)v11 * p6;
  t += (udpint)v12 * p5;
  t += (udpint)v13 * p4;
  t += (udpint)v14 * p3;
  t += (udpint)v15 * p2;
  t += (udpint)v16 * p1;
  t += (udpint)s;
  spint v17 = (((spint)t * ndash) & mask);
  t += (udpint)v17 * p0;
  t >>= 28;
  tot = (udpint)a[0] * a[18];
  tot += (udpint)a[1] * a[17];
  tot += (udpint)a[2] * a[16];
  tot += (udpint)a[3] * a[15];
  tot += (udpint)a[4] * a[14];
  tot += (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  s = (spint)mask;
  t += (udpint)v0 << 17u;
  s -= v8;
  t += (udpint)v9 * p9;
  t += (udpint)v10 * p8;
  t += (udpint)v11 * p7;
  t += (udpint)v12 * p6;
  t += (udpint)v13 * p5;
  t += (udpint)v14 * p4;
  t += (udpint)v15 * p3;
  t += (udpint)v16 * p2;
  t += (udpint)v17 * p1;
  t += (udpint)s;
  spint v18 = (((spint)t * ndash) & mask);
  t += (udpint)v18 * p0;
  t >>= 28;
  tot = (udpint)a[1] * a[18];
  tot += (udpint)a[2] * a[17];
  tot += (udpint)a[3] * a[16];
  tot += (udpint)a[4] * a[15];
  tot += (udpint)a[5] * a[14];
  tot += (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v1 << 17u;
  s -= v9;
  t += (udpint)v10 * p9;
  t += (udpint)v11 * p8;
  t += (udpint)v12 * p7;
  t += (udpint)v13 * p6;
  t += (udpint)v14 * p5;
  t += (udpint)v15 * p4;
  t += (udpint)v16 * p3;
  t += (udpint)v17 * p2;
  t += (udpint)v18 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[18];
  tot += (udpint)a[3] * a[17];
  tot += (udpint)a[4] * a[16];
  tot += (udpint)a[5] * a[15];
  tot += (udpint)a[6] * a[14];
  tot += (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 << 17u;
  s -= v10;
  t += (udpint)v11 * p9;
  t += (udpint)v12 * p8;
  t += (udpint)v13 * p7;
  t += (udpint)v14 * p6;
  t += (udpint)v15 * p5;
  t += (udpint)v16 * p4;
  t += (udpint)v17 * p3;
  t += (udpint)v18 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[18];
  tot += (udpint)a[4] * a[17];
  tot += (udpint)a[5] * a[16];
  tot += (udpint)a[6] * a[15];
  tot += (udpint)a[7] * a[14];
  tot += (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 << 17u;
  s -= v11;
  t += (udpint)v12 * p9;
  t += (udpint)v13 * p8;
  t += (udpint)v14 * p7;
  t += (udpint)v15 * p6;
  t += (udpint)v16 * p5;
  t += (udpint)v17 * p4;
  t += (udpint)v18 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[18];
  tot += (udpint)a[5] * a[17];
  tot += (udpint)a[6] * a[16];
  tot += (udpint)a[7] * a[15];
  tot += (udpint)a[8] * a[14];
  tot += (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 17u;
  s -= v12;
  t += (udpint)v13 * p9;
  t += (udpint)v14 * p8;
  t += (udpint)v15 * p7;
  t += (udpint)v16 * p6;
  t += (udpint)v17 * p5;
  t += (udpint)v18 * p4;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[18];
  tot += (udpint)a[6] * a[17];
  tot += (udpint)a[7] * a[16];
  tot += (udpint)a[8] * a[15];
  tot += (udpint)a[9] * a[14];
  tot += (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 << 17u;
  s -= v13;
  t += (udpint)v14 * p9;
  t += (udpint)v15 * p8;
  t += (udpint)v16 * p7;
  t += (udpint)v17 * p6;
  t += (udpint)v18 * p5;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[18];
  tot += (udpint)a[7] * a[17];
  tot += (udpint)a[8] * a[16];
  tot += (udpint)a[9] * a[15];
  tot += (udpint)a[10] * a[14];
  tot += (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 << 17u;
  s -= v14;
  t += (udpint)v15 * p9;
  t += (udpint)v16 * p8;
  t += (udpint)v17 * p7;
  t += (udpint)v18 * p6;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[18];
  tot += (udpint)a[8] * a[17];
  tot += (udpint)a[9] * a[16];
  tot += (udpint)a[10] * a[15];
  tot += (udpint)a[11] * a[14];
  tot += (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v7 << 17u;
  s -= v15;
  t += (udpint)v16 * p9;
  t += (udpint)v17 * p8;
  t += (udpint)v18 * p7;
  t += (udpint)s;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[18];
  tot += (udpint)a[9] * a[17];
  tot += (udpint)a[10] * a[16];
  tot += (udpint)a[11] * a[15];
  tot += (udpint)a[12] * a[14];
  tot *= 2;
  tot += (udpint)a[13] * a[13];
  t += tot;
  s = (spint)mask;
  t += (udpint)v8 << 17u;
  s -= v16;
  t += (udpint)v17 * p9;
  t += (udpint)v18 * p8;
  t += (udpint)s;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[18];
  tot += (udpint)a[10] * a[17];
  tot += (udpint)a[11] * a[16];
  tot += (udpint)a[12] * a[15];
  tot += (udpint)a[13] * a[14];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v9 << 17u;
  s -= v17;
  t += (udpint)v18 * p9;
  t += (udpint)s;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[18];
  tot += (udpint)a[11] * a[17];
  tot += (udpint)a[12] * a[16];
  tot += (udpint)a[13] * a[15];
  tot *= 2;
  tot += (udpint)a[14] * a[14];
  t += tot;
  s = (spint)mask;
  t += (udpint)v10 << 17u;
  s -= v18;
  t += (udpint)s;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[18];
  tot += (udpint)a[12] * a[17];
  tot += (udpint)a[13] * a[16];
  tot += (udpint)a[14] * a[15];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v11 << 17u;
  t += (udpint)s;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[18];
  tot += (udpint)a[13] * a[17];
  tot += (udpint)a[14] * a[16];
  tot *= 2;
  tot += (udpint)a[15] * a[15];
  t += tot;
  s = (spint)mask;
  t += (udpint)v12 << 17u;
  t += (udpint)s;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[18];
  tot += (udpint)a[14] * a[17];
  tot += (udpint)a[15] * a[16];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v13 << 17u;
  t += (udpint)s;
  c[12] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[14] * a[18];
  tot += (udpint)a[15] * a[17];
  tot *= 2;
  tot += (udpint)a[16] * a[16];
  t += tot;
  s = (spint)mask;
  t += (udpint)v14 << 17u;
  t += (udpint)s;
  c[13] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[15] * a[18];
  tot += (udpint)a[16] * a[17];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v15 << 17u;
  t += (udpint)s;
  c[14] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[16] * a[18];
  tot *= 2;
  tot += (udpint)a[17] * a[17];
  t += tot;
  s = (spint)mask;
  t += (udpint)v16 << 17u;
  t += (udpint)s;
  c[15] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[17] * a[18];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v17 << 17u;
  t += (udpint)s;
  c[16] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[18] * a[18];
  t += tot;
  s = (spint)mask;
  t += (udpint)v18 << 17u;
  t += (udpint)s;
  c[17] = ((spint)t & mask);
  t >>= 28;
  t -= 1u;
  c[18] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 19; i++) {
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
  spint x[19];
  spint t0[19];
  spint t1[19];
  spint t2[19];
  spint t3[19];
  spint t4[19];
  spint t5[19];
  spint t6[19];
  spint t7[19];
  spint t8[19];
  spint t9[19];
  spint t10[19];
  spint t11[19];
  spint t12[19];
  spint t13[19];
  spint t14[19];
  spint t15[19];
  spint t16[19];
  spint t17[19];
  spint t18[19];
  spint t19[19];
  spint t20[19];
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
  spint s[19];
  spint t[19];
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
  const spint c[19] = {0x603fb63u, 0x69c815cu, 0xf1cfbbau, 0x38a6efau,
                       0xde5cd23u, 0x1524db6u, 0xd09706cu, 0xeaaa8eau,
                       0xb4ace7fu, 0xd142b76u, 0x7bccff3u, 0x61a8e56u,
                       0x55bcc6du, 0x492d0d4u, 0xd8e03d1u, 0x44383d2u,
                       0xa3afe8cu, 0xa39a5b5u, 0x391u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[19];
  c[0] = 1;
  for (i = 1; i < 19; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[19];
  for (i = 0; i < 19; i++) {
    b[i] = n[i];
    t[i] = n[i + 19];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[19];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 19; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[19];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 19; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 19; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 19; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 19; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[19];
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
  for (i = 0; i < 19; i++) {
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
  for (i = 0; i < 19; i++) {
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
  spint t[19];
  spint b[19];
  spint v[19];
  spint z[19] = {0x63e2205u, 0xf9c7b7bu, 0xac810a7u, 0xb3da861u, 0x3a72472u,
                 0x836647du, 0xcd998b7u, 0x391dcdbu, 0x35f79ffu, 0x55276d5u,
                 0x5673a30u, 0xfa92fccu, 0xd7c12dau, 0xc7e6000u, 0xfa8c4fbu,
                 0xd708ad2u, 0x28c17f3u, 0x650d44bu, 0x9a0au};
  spint s[19];
  spint y[19];
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
  a[18] = ((a[18] << n)) | (a[17] >> (28u - n));
  for (i = 17; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 18; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[18] = a[18] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 28u;
  unsigned int m = r % 28u;
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
  spint c[19];
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
  for (i = 0; i < 19; i++) {
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
  spint c[19];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[19], d[19];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 19; i++) {
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
    HASH512_hash(&sh512,&h[2]); 
    h[0]=h[1]=0;  /* !!! */
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
    HASH512_hash(&sh512,&h[2]); 
    h[0]=h[1]=0;

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

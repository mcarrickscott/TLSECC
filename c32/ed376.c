// Elliptic curve based digital signature template
// See for example https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
//

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "Ed376curve.h" // elliptic curve API

/*** Insert code automatically generated from group.c here ***/
/* Note that much of this code is not needed and can be deleted */


// Automatically generated modular arithmetic C code
// Command line : python monty.py 32
// 002501103908950821434373181256356772360674006887285404329509619144461898813929743809681342407547708774670617348304951
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define dpint uint64_t
#define sdpint int64_t
#define Wordlength 32
#define Nlimbs 14
#define Radix 28
#define Nbits 381
#define Nbytes 48

#define MONTGOMERY
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
static spint flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0xc6a7037u) & carry;
  n[1] += ((spint)0x855d4f3u) & carry;
  n[2] += ((spint)0xd04f09fu) & carry;
  n[3] += ((spint)0x98f29f0u) & carry;
  n[4] += ((spint)0x9cd109au) & carry;
  n[5] += ((spint)0xb351487u) & carry;
  n[6] += ((spint)0x303a69u) & carry;
  n[13] += ((spint)0x10400u) & carry;
  (void)prop(n);
  return (carry & 1);
}

// Montgomery final subtract
static spint modfsb(spint *n) {
  n[0] -= (spint)0xc6a7037u;
  n[1] -= (spint)0x855d4f3u;
  n[2] -= (spint)0xd04f09fu;
  n[3] -= (spint)0x98f29f0u;
  n[4] -= (spint)0x9cd109au;
  n[5] -= (spint)0xb351487u;
  n[6] -= (spint)0x303a69u;
  n[13] -= (spint)0x10400u;
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
  n[0] -= (spint)0x18d4e06eu;
  n[1] -= (spint)0x10aba9e6u;
  n[2] -= (spint)0x1a09e13eu;
  n[3] -= (spint)0x131e53e0u;
  n[4] -= (spint)0x139a2134u;
  n[5] -= (spint)0x166a290eu;
  n[6] -= (spint)0x6074d2u;
  n[13] -= (spint)0x20800u;
  carry = prop(n);
  n[0] += ((spint)0x18d4e06eu) & carry;
  n[1] += ((spint)0x10aba9e6u) & carry;
  n[2] += ((spint)0x1a09e13eu) & carry;
  n[3] += ((spint)0x131e53e0u) & carry;
  n[4] += ((spint)0x139a2134u) & carry;
  n[5] += ((spint)0x166a290eu) & carry;
  n[6] += ((spint)0x6074d2u) & carry;
  n[13] += ((spint)0x20800u) & carry;
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
  n[0] += ((spint)0x18d4e06eu) & carry;
  n[1] += ((spint)0x10aba9e6u) & carry;
  n[2] += ((spint)0x1a09e13eu) & carry;
  n[3] += ((spint)0x131e53e0u) & carry;
  n[4] += ((spint)0x139a2134u) & carry;
  n[5] += ((spint)0x166a290eu) & carry;
  n[6] += ((spint)0x6074d2u) & carry;
  n[13] += ((spint)0x20800u) & carry;
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
  n[0] += ((spint)0x18d4e06eu) & carry;
  n[1] += ((spint)0x10aba9e6u) & carry;
  n[2] += ((spint)0x1a09e13eu) & carry;
  n[3] += ((spint)0x131e53e0u) & carry;
  n[4] += ((spint)0x139a2134u) & carry;
  n[5] += ((spint)0x166a290eu) & carry;
  n[6] += ((spint)0x6074d2u) & carry;
  n[13] += ((spint)0x20800u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1299425116202482891
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0xc6a7037u;
  spint p1 = 0x855d4f3u;
  spint p2 = 0xd04f09fu;
  spint p3 = 0x98f29f0u;
  spint p4 = 0x9cd109au;
  spint p5 = 0xb351487u;
  spint p6 = 0x303a69u;
  spint p13 = 0x10400u;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x69fba79u;
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
  t += (dpint)v5 * (dpint)p6;
  t += (dpint)v6 * (dpint)p5;
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
  t += (dpint)v9 * (dpint)p2;
  t += (dpint)v10 * (dpint)p1;
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
  t += (dpint)v6 * (dpint)p6;
  t += (dpint)v7 * (dpint)p5;
  t += (dpint)v8 * (dpint)p4;
  t += (dpint)v9 * (dpint)p3;
  t += (dpint)v10 * (dpint)p2;
  t += (dpint)v11 * (dpint)p1;
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
  t += (dpint)v0 * (dpint)p13;
  t += (dpint)v7 * (dpint)p6;
  t += (dpint)v8 * (dpint)p5;
  t += (dpint)v9 * (dpint)p4;
  t += (dpint)v10 * (dpint)p3;
  t += (dpint)v11 * (dpint)p2;
  t += (dpint)v12 * (dpint)p1;
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
  t += (dpint)v1 * (dpint)p13;
  t += (dpint)v8 * (dpint)p6;
  t += (dpint)v9 * (dpint)p5;
  t += (dpint)v10 * (dpint)p4;
  t += (dpint)v11 * (dpint)p3;
  t += (dpint)v12 * (dpint)p2;
  t += (dpint)v13 * (dpint)p1;
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
  t += (dpint)v2 * (dpint)p13;
  t += (dpint)v9 * (dpint)p6;
  t += (dpint)v10 * (dpint)p5;
  t += (dpint)v11 * (dpint)p4;
  t += (dpint)v12 * (dpint)p3;
  t += (dpint)v13 * (dpint)p2;
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
  t += (dpint)v3 * (dpint)p13;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  t += (dpint)v12 * (dpint)p4;
  t += (dpint)v13 * (dpint)p3;
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
  t += (dpint)v4 * (dpint)p13;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  t += (dpint)v13 * (dpint)p4;
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
  t += (dpint)v5 * (dpint)p13;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
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
  t += (dpint)v6 * (dpint)p13;
  t += (dpint)v13 * (dpint)p6;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  t += (dpint)v7 * (dpint)p13;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  t += (dpint)v8 * (dpint)p13;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)v9 * (dpint)p13;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)v10 * (dpint)p13;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)v11 * (dpint)p13;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)v12 * (dpint)p13;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[13];
  t += (dpint)v13 * (dpint)p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  c[13] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  dpint tot;
  dpint t = 0;
  spint p0 = 0xc6a7037u;
  spint p1 = 0x855d4f3u;
  spint p2 = 0xd04f09fu;
  spint p3 = 0x98f29f0u;
  spint p4 = 0x9cd109au;
  spint p5 = 0xb351487u;
  spint p6 = 0x303a69u;
  spint p13 = 0x10400u;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x69fba79u;
  tot = (dpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[2];
  tot *= 2;
  tot += (dpint)a[1] * a[1];
  t += tot;
  t += (dpint)v0 * p2;
  t += (dpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[3];
  tot += (dpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p3;
  t += (dpint)v1 * p2;
  t += (dpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[4];
  tot += (dpint)a[1] * a[3];
  tot *= 2;
  tot += (dpint)a[2] * a[2];
  t += tot;
  t += (dpint)v0 * p4;
  t += (dpint)v1 * p3;
  t += (dpint)v2 * p2;
  t += (dpint)v3 * p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (dpint)v4 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[5];
  tot += (dpint)a[1] * a[4];
  tot += (dpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p5;
  t += (dpint)v1 * p4;
  t += (dpint)v2 * p3;
  t += (dpint)v3 * p2;
  t += (dpint)v4 * p1;
  spint v5 = (((spint)t * ndash) & mask);
  t += (dpint)v5 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[6];
  tot += (dpint)a[1] * a[5];
  tot += (dpint)a[2] * a[4];
  tot *= 2;
  tot += (dpint)a[3] * a[3];
  t += tot;
  t += (dpint)v0 * p6;
  t += (dpint)v1 * p5;
  t += (dpint)v2 * p4;
  t += (dpint)v3 * p3;
  t += (dpint)v4 * p2;
  t += (dpint)v5 * p1;
  spint v6 = (((spint)t * ndash) & mask);
  t += (dpint)v6 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[7];
  tot += (dpint)a[1] * a[6];
  tot += (dpint)a[2] * a[5];
  tot += (dpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (dpint)v1 * p6;
  t += (dpint)v2 * p5;
  t += (dpint)v3 * p4;
  t += (dpint)v4 * p3;
  t += (dpint)v5 * p2;
  t += (dpint)v6 * p1;
  spint v7 = (((spint)t * ndash) & mask);
  t += (dpint)v7 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[8];
  tot += (dpint)a[1] * a[7];
  tot += (dpint)a[2] * a[6];
  tot += (dpint)a[3] * a[5];
  tot *= 2;
  tot += (dpint)a[4] * a[4];
  t += tot;
  t += (dpint)v2 * p6;
  t += (dpint)v3 * p5;
  t += (dpint)v4 * p4;
  t += (dpint)v5 * p3;
  t += (dpint)v6 * p2;
  t += (dpint)v7 * p1;
  spint v8 = (((spint)t * ndash) & mask);
  t += (dpint)v8 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[9];
  tot += (dpint)a[1] * a[8];
  tot += (dpint)a[2] * a[7];
  tot += (dpint)a[3] * a[6];
  tot += (dpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (dpint)v3 * p6;
  t += (dpint)v4 * p5;
  t += (dpint)v5 * p4;
  t += (dpint)v6 * p3;
  t += (dpint)v7 * p2;
  t += (dpint)v8 * p1;
  spint v9 = (((spint)t * ndash) & mask);
  t += (dpint)v9 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[10];
  tot += (dpint)a[1] * a[9];
  tot += (dpint)a[2] * a[8];
  tot += (dpint)a[3] * a[7];
  tot += (dpint)a[4] * a[6];
  tot *= 2;
  tot += (dpint)a[5] * a[5];
  t += tot;
  t += (dpint)v4 * p6;
  t += (dpint)v5 * p5;
  t += (dpint)v6 * p4;
  t += (dpint)v7 * p3;
  t += (dpint)v8 * p2;
  t += (dpint)v9 * p1;
  spint v10 = (((spint)t * ndash) & mask);
  t += (dpint)v10 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[11];
  tot += (dpint)a[1] * a[10];
  tot += (dpint)a[2] * a[9];
  tot += (dpint)a[3] * a[8];
  tot += (dpint)a[4] * a[7];
  tot += (dpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  t += (dpint)v5 * p6;
  t += (dpint)v6 * p5;
  t += (dpint)v7 * p4;
  t += (dpint)v8 * p3;
  t += (dpint)v9 * p2;
  t += (dpint)v10 * p1;
  spint v11 = (((spint)t * ndash) & mask);
  t += (dpint)v11 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[12];
  tot += (dpint)a[1] * a[11];
  tot += (dpint)a[2] * a[10];
  tot += (dpint)a[3] * a[9];
  tot += (dpint)a[4] * a[8];
  tot += (dpint)a[5] * a[7];
  tot *= 2;
  tot += (dpint)a[6] * a[6];
  t += tot;
  t += (dpint)v6 * p6;
  t += (dpint)v7 * p5;
  t += (dpint)v8 * p4;
  t += (dpint)v9 * p3;
  t += (dpint)v10 * p2;
  t += (dpint)v11 * p1;
  spint v12 = (((spint)t * ndash) & mask);
  t += (dpint)v12 * p0;
  t >>= 28;
  tot = (dpint)a[0] * a[13];
  tot += (dpint)a[1] * a[12];
  tot += (dpint)a[2] * a[11];
  tot += (dpint)a[3] * a[10];
  tot += (dpint)a[4] * a[9];
  tot += (dpint)a[5] * a[8];
  tot += (dpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p13;
  t += (dpint)v7 * p6;
  t += (dpint)v8 * p5;
  t += (dpint)v9 * p4;
  t += (dpint)v10 * p3;
  t += (dpint)v11 * p2;
  t += (dpint)v12 * p1;
  spint v13 = (((spint)t * ndash) & mask);
  t += (dpint)v13 * p0;
  t >>= 28;
  tot = (dpint)a[1] * a[13];
  tot += (dpint)a[2] * a[12];
  tot += (dpint)a[3] * a[11];
  tot += (dpint)a[4] * a[10];
  tot += (dpint)a[5] * a[9];
  tot += (dpint)a[6] * a[8];
  tot *= 2;
  tot += (dpint)a[7] * a[7];
  t += tot;
  t += (dpint)v1 * p13;
  t += (dpint)v8 * p6;
  t += (dpint)v9 * p5;
  t += (dpint)v10 * p4;
  t += (dpint)v11 * p3;
  t += (dpint)v12 * p2;
  t += (dpint)v13 * p1;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[2] * a[13];
  tot += (dpint)a[3] * a[12];
  tot += (dpint)a[4] * a[11];
  tot += (dpint)a[5] * a[10];
  tot += (dpint)a[6] * a[9];
  tot += (dpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (dpint)v2 * p13;
  t += (dpint)v9 * p6;
  t += (dpint)v10 * p5;
  t += (dpint)v11 * p4;
  t += (dpint)v12 * p3;
  t += (dpint)v13 * p2;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[3] * a[13];
  tot += (dpint)a[4] * a[12];
  tot += (dpint)a[5] * a[11];
  tot += (dpint)a[6] * a[10];
  tot += (dpint)a[7] * a[9];
  tot *= 2;
  tot += (dpint)a[8] * a[8];
  t += tot;
  t += (dpint)v3 * p13;
  t += (dpint)v10 * p6;
  t += (dpint)v11 * p5;
  t += (dpint)v12 * p4;
  t += (dpint)v13 * p3;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[4] * a[13];
  tot += (dpint)a[5] * a[12];
  tot += (dpint)a[6] * a[11];
  tot += (dpint)a[7] * a[10];
  tot += (dpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  t += (dpint)v4 * p13;
  t += (dpint)v11 * p6;
  t += (dpint)v12 * p5;
  t += (dpint)v13 * p4;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[5] * a[13];
  tot += (dpint)a[6] * a[12];
  tot += (dpint)a[7] * a[11];
  tot += (dpint)a[8] * a[10];
  tot *= 2;
  tot += (dpint)a[9] * a[9];
  t += tot;
  t += (dpint)v5 * p13;
  t += (dpint)v12 * p6;
  t += (dpint)v13 * p5;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[6] * a[13];
  tot += (dpint)a[7] * a[12];
  tot += (dpint)a[8] * a[11];
  tot += (dpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  t += (dpint)v6 * p13;
  t += (dpint)v13 * p6;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[7] * a[13];
  tot += (dpint)a[8] * a[12];
  tot += (dpint)a[9] * a[11];
  tot *= 2;
  tot += (dpint)a[10] * a[10];
  t += tot;
  t += (dpint)v7 * p13;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[8] * a[13];
  tot += (dpint)a[9] * a[12];
  tot += (dpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  t += (dpint)v8 * p13;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[9] * a[13];
  tot += (dpint)a[10] * a[12];
  tot *= 2;
  tot += (dpint)a[11] * a[11];
  t += tot;
  t += (dpint)v9 * p13;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[10] * a[13];
  tot += (dpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  t += (dpint)v10 * p13;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[11] * a[13];
  tot *= 2;
  tot += (dpint)a[12] * a[12];
  t += tot;
  t += (dpint)v11 * p13;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  t += (dpint)v12 * p13;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (dpint)a[13] * a[13];
  t += tot;
  t += (dpint)v13 * p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
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
  spint t11[14];
  spint t12[14];
  spint t13[14];
  spint t14[14];
  modcpy(w, x);
  modsqr(x, t1);
  modmul(x, t1, t0);
  modmul(t1, t0, t11);
  modmul(x, t11, t6);
  modmul(t1, t6, t14);
  modmul(t11, t14, z);
  modmul(t1, z, t12);
  modmul(t6, t12, t5);
  modmul(t6, t5, t13);
  modmul(t1, t13, t4);
  modmul(t0, t4, t7);
  modmul(x, t7, t8);
  modmul(t1, t8, t2);
  modmul(t6, t8, t3);
  modmul(t1, t3, t1);
  modmul(t14, t1, t9);
  modmul(t5, t7, t10);
  modmul(t14, t9, t7);
  modmul(t14, t7, t14);
  modnsqr(t14, 186);
  modmul(t0, t14, t14);
  modnsqr(t14, 11);
  modmul(t4, t14, t14);
  modnsqr(t14, 6);
  modmul(z, t14, t14);
  modnsqr(t14, 7);
  modmul(t13, t14, t13);
  modnsqr(t13, 8);
  modmul(t10, t13, t13);
  modnsqr(t13, 9);
  modmul(t1, t13, t13);
  modnsqr(t13, 8);
  modmul(t12, t13, t12);
  modnsqr(t12, 8);
  modmul(t7, t12, t12);
  modnsqr(t12, 3);
  modmul(t11, t12, t11);
  modnsqr(t11, 9);
  modmul(t8, t11, t11);
  modnsqr(t11, 8);
  modmul(t10, t11, t10);
  modnsqr(t10, 8);
  modmul(t9, t10, t9);
  modnsqr(t9, 6);
  modmul(t7, t9, t9);
  modnsqr(t9, 7);
  modmul(t3, t9, t9);
  modnsqr(t9, 2);
  modmul(t0, t9, t9);
  modnsqr(t9, 8);
  modmul(z, t9, t9);
  modnsqr(t9, 11);
  modmul(t3, t9, t9);
  modnsqr(t9, 6);
  modmul(t8, t9, t8);
  modnsqr(t8, 8);
  modmul(t7, t8, t7);
  modmul(t6, t7, t6);
  modnsqr(t6, 9);
  modmul(t5, t6, t5);
  modnsqr(t5, 6);
  modmul(t4, t5, t4);
  modnsqr(t4, 7);
  modmul(t3, t4, t4);
  modnsqr(t4, 6);
  modmul(t3, t4, t3);
  modnsqr(t3, 6);
  modmul(t2, t3, t2);
  modnsqr(t2, 7);
  modmul(t1, t2, t1);
  modnsqr(t1, 2);
  modmul(t0, t1, t0);
  modnsqr(t0, 10);
  modmul(z, t0, z);
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
  const spint c[14] = {0xdbcd4bau, 0x776d5c8u, 0x6c906a8u, 0x15d6ae9u,
                       0xea69e0eu, 0xfa0c01au, 0x704847du, 0x4255610u,
                       0x2eeefd8u, 0xee6c03cu, 0x7e6d239u, 0x10979e6u,
                       0x8db637eu, 0x1476u};
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

// Modular multiplication by an integer, c=a*b mod 2p
// uses special method for trinomials, otherwise Barrett-Dhem reduction
static void modmli(const spint *a, int b, spint *c) {
  spint p0 = 0xc6a7037u;
  spint p1 = 0x855d4f3u;
  spint p2 = 0xd04f09fu;
  spint p3 = 0x98f29f0u;
  spint p4 = 0x9cd109au;
  spint p5 = 0xb351487u;
  spint p6 = 0x303a69u;
  spint p13 = 0x10400u;
  spint mask = ((spint)1 << 28u) - (spint)1;
  dpint t = 0;
  spint q, h, r = 0x1f81f81f;
  t += (dpint)a[0] * (dpint)b;
  c[0] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[1] * (dpint)b;
  c[1] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[2] * (dpint)b;
  c[2] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[3] * (dpint)b;
  c[3] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[4] * (dpint)b;
  c[4] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[5] * (dpint)b;
  c[5] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[6] * (dpint)b;
  c[6] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[7] * (dpint)b;
  c[7] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[8] * (dpint)b;
  c[8] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[9] * (dpint)b;
  c[9] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[10] * (dpint)b;
  c[10] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[11] * (dpint)b;
  c[11] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[12] * (dpint)b;
  c[12] = (spint)t & mask;
  t = t >> 28u;
  t += (dpint)a[13] * (dpint)b;
  c[13] = (spint)t;

  // Barrett-Dhem reduction
  h = (spint)(t >> 13u);
  q = (spint)(((dpint)h * (dpint)r) >> 32u);
  t = (dpint)q * (dpint)p0;
  c[0] -= (spint)t & mask;
  c[1] -= (spint)(t >> 28u);
  t = (dpint)q * (dpint)p1;
  c[1] -= (spint)t & mask;
  c[2] -= (spint)(t >> 28u);
  t = (dpint)q * (dpint)p2;
  c[2] -= (spint)t & mask;
  c[3] -= (spint)(t >> 28u);
  t = (dpint)q * (dpint)p3;
  c[3] -= (spint)t & mask;
  c[4] -= (spint)(t >> 28u);
  t = (dpint)q * (dpint)p4;
  c[4] -= (spint)t & mask;
  c[5] -= (spint)(t >> 28u);
  t = (dpint)q * (dpint)p5;
  c[5] -= (spint)t & mask;
  c[6] -= (spint)(t >> 28u);
  t = (dpint)q * (dpint)p6;
  c[6] -= (spint)t & mask;
  c[7] -= (spint)(t >> 28u);
  c[13] -= q * p13;
  (void)prop(c);
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
  return modis1(r) | modis0(x);
}

// conditional move g to f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void __attribute__((noinline))
modcmv(int b, const spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, aux;
  static spint R = 0;
  R += 0x5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 14; i++) {
    s = g[i];
    t = f[i];
    f[i] = aux = c0 * t + c1 * s;
    f[i] = aux - w * (t + s);
  }
}

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void __attribute__((noinline))
modcsw(int b, volatile spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, v, aux;
  static spint R = 0;
  R += 0x5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 14; i++) {
    s = g[i];
    t = f[i];
    v = w * (t + s);
    f[i] = aux = c0 * t + c1 * s;
    f[i] = aux - v;
    g[i] = aux = c0 * s + c1 * t;
    g[i] = aux - v;
  }
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
  a[13] = ((a[13] << n)) + (a[12] >> (28u - n));
  for (i = 12; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) + (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 13; i++) {
    a[i] = (a[i] >> n) + ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[13] = a[13] >> n;
  return r;
}

// divide by 2. Shift right 1 bit (or add p and shift right one bit)
static void modhaf(spint *n) {
  int lsb;
  spint t[14];
  (void)prop(n);
  modcpy(n, t);
  lsb = modshr(1, t);
  n[0] += ((spint)0xc6a7037u);
  n[1] += ((spint)0x855d4f3u);
  n[2] += ((spint)0xd04f09fu);
  n[3] += ((spint)0x98f29f0u);
  n[4] += ((spint)0x9cd109au);
  n[5] += ((spint)0xb351487u);
  n[6] += ((spint)0x303a69u);
  n[13] += ((spint)0x10400u);
  (void)prop(n);
  modshr(1, n);
  modcmv(1 - lsb, t, n);
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
  res = (int)modfsb(a);
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

typedef spint gel[Nlimbs];  // group element definition
#define BYTES Nbytes        // Number of bytes in group element

// some general purpose functions

// reverse bytes of buff - for endian conversion
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
    char pad[128];
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

// reduce 768 bit array h to integer r modulo group order q, in constant time
// Consider h as 2^376.(2^376x + y) + z, where x,y and z < q (z is bottom 376 bits, y is next 376 bits, x is top 16 bits)
// Important that x and y < q, q is group order
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,z,c;

    mod2r(376,c);

    for (i=0;i<47;i++)  // bottom 47.8=376 bits
        buff[i]=h[i];
    buff[47]=0;
    reverse(buff);
    modimp(buff,z);

    for (i=0;i<47;i++)  // middle 376 bits
        buff[i]=h[i+47];
    buff[47]=0;
    reverse(buff);
    modimp(buff,y);  
    
    for (i=0;i<2;i++)  // top 16 bits
       buff[i]=h[94+i];   
    for (i=2;i<48;i++)
        buff[i]=0;
    reverse(buff);
    modimp(buff,x);    
    
    modmul(x,c,x); 
    modadd(x,y,x); 
    modmul(x,c,x); 
    modadd(x,z,r);  
}

// general purpose SHAKE256 hash function
// Input ilen bytes, output olen bytes
static void H(int ilen,int olen,char *s,char *digest)
{
    sha3 SHA3;
    SHA3_init(&SHA3,SHAKE256);
    for (int i=0;i<ilen;i++) 
        SHA3_process(&SHA3,s[i]);
    SHA3_shake(&SHA3,digest,olen); 
}

// API implementation

// Input private key
// Output public key
void ED376_KEY_PAIR(char *prv,char *pub)
{
    int sign;
    point P;
    char s[BYTES];
    ecn_ed376_gen(&P);  // get curve generator point

    H(BYTES,BYTES,prv,s);
// clamp s
    s[0]&=0xFC;
    s[47]&=0x7F; s[47]|=0x40;

    reverse(s);  // little endian to big endian
    ecn_ed376_mul(s,&P); 

    sign=ecn_ed376_get(&P,NULL,pub);  // get y coordinate and sign
    reverse(pub);              // big endian to little endian
    pub[47]|=(char)(sign<<7);
}

static const char dom4[10]={'S','i','g','E','d','3','8','3',0,0};

// Note that a per message "random" number usually generated by a Cryptographically Secure Random Number Generator (CSRNG) is often required, and if so is calculated externally and passed into the signature function
// Methods are
// 1. non-deterministic - a non-biased high entropy random number in the range from 0 - q (the group order) - recommended ii CSRNG is good
// 2. deterministic - derived from a hash of the message to be signed and the secret key (must be done like this for EdDSA, optional for ECDSA)
// 3. hedged - a combination of methods 1 and 2 - recommended if CSRNG is poor
// Note that this functionality is out-of-scope for this exercise (except for EdDSA). For examples see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf and RFC6979

// To create such a per message random number, consider the method reduce() from Ed448.c, which reduces a much larger number modulo q, as required to minimize bias

// input private key, per-message random number (or public key), message length, message to be signed
// Output signature.
void ED376_SIGN(char *prv,char *pub,int mlen,char *m,char *sig)
{
    int i,sign;
    char h[2*BYTES];
    char ipub[BYTES];
    sha3 SHA3;
    point R;
    gel r,s,d;     // some group elements..
    ecn_ed376_gen(&R);  // get curve generator point

    if (pub!=NULL)
    {
        for (i=0;i<BYTES;i++)
            ipub[i]=pub[i];
    } else {
        ED376_KEY_PAIR(prv,ipub);
    }

    SHA3_init(&SHA3,SHAKE256);
    H(BYTES,2*BYTES,prv,h);

// derive and clamp s
    h[0]&=0xFC;
    h[47]&=0x7F; h[47]|=0x40;
    reverse(h); 
    modimp(h,s);

    for (i=0;i<10;i++)
        SHA3_process(&SHA3,dom4[i]);
    for (i=BYTES;i<2*BYTES;i++ )
        SHA3_process(&SHA3,h[i]);
    for (i=0;i<mlen;i++)
        SHA3_process(&SHA3,m[i]);
    SHA3_shake(&SHA3,h,2*BYTES); 

    reduce(h,r);
    modexp(r,h);  // convert to big endian array
    ecn_ed376_mul(h,&R);

    sign=ecn_ed376_get(&R,NULL,sig);  // get y coordinate and sign
    reverse(sig);              // big endian to little endian
    sig[BYTES-1]|=(char)(sign<<7); // first part of signature

    SHA3_init(&SHA3,SHAKE256);
    for (i=0;i<10;i++)
        SHA3_process(&SHA3,dom4[i]);
    for (i=0;i<BYTES;i++ )
        SHA3_process(&SHA3,sig[i]);  // R
    for (i=0;i<BYTES;i++)
        SHA3_process(&SHA3,ipub[i]);  // Q
    for (i=0;i<mlen;i++)
        SHA3_process(&SHA3,m[i]);   // M
    SHA3_shake(&SHA3,h,2*BYTES);    

    reduce(h,d);
    modmul(d,s,d);
    modadd(d,r,d);

    modexp(d,&sig[BYTES]);
    reverse(&sig[BYTES]);
}

// input public key, message length message and signature. 
// Return 1 for success, 0 for failure
int ED376_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    int i;
    point G,R,Q;
    gel u;     // some group elements..
    int sign;
    sha3 SHA3;
    char h[2*BYTES];
    char buff[BYTES];

    ecn_ed376_gen(&G);  // get curve generator point

// reconstruct point R
    sign=(sig[BYTES-1]>>7)&1; 
    for (i=0;i<BYTES;i++)
        buff[i]=sig[i];
    buff[BYTES-1]&=0x7f;
    reverse(buff);
    ecn_ed376_set(sign,NULL,buff,&R);
    if (ecn_ed376_isinf(&R)) return 0;

// reconstruct point Q 
    sign=(pub[BYTES-1]>>7)&1;
    for (i=0;i<BYTES;i++)
        buff[i]=pub[i];
    buff[BYTES-1]&=0x7f;
    reverse(buff);
    ecn_ed376_set(sign,NULL,buff,&Q); 
    if (ecn_ed376_isinf(&Q)) return 0;

    for (i=0;i<BYTES;i++)
        buff[i]=sig[i+BYTES];
    reverse(buff);

    SHA3_init(&SHA3,SHAKE256);
    for (i=0;i<10;i++)
        SHA3_process(&SHA3,dom4[i]);
    for (i=0;i<BYTES;i++ )
        SHA3_process(&SHA3,sig[i]);  // R
    for (int i=0;i<BYTES;i++)
        SHA3_process(&SHA3,pub[i]);  // Q
    for (i=0;i<mlen;i++)
        SHA3_process(&SHA3,m[i]);   // M
    SHA3_shake(&SHA3,h,2*BYTES); 

    reduce(h,u); modneg(u,u); modexp(u,h);

    if (!modimp(buff,u)) return 0;  // out of range

    ecn_ed376_cof(&G); ecn_ed376_cof(&R); ecn_ed376_cof(&Q);
    ecn_ed376_mul2(buff,&G,h,&Q,&Q);

    if (ecn_ed376_cmp(&R,&Q))
        return 1;
    return 0;
}



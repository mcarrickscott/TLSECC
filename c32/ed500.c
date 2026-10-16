// Elliptic curve based digital signature template
// See for example https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
//

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "Ed500curve.h" // elliptic curve API

/*** Insert code automatically generated from group.c here ***/
/* Note that much of this code is not needed and can be deleted */


// Automatically generated modular arithmetic C code
// Command line : python monty.py 32
// 00022095386603298957622589030453586294277462333810790687329012216734648903125732812162322745424568361198616424824173658079693047834955346648597521750624369
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define dpint uint64_t
#define sdpint int64_t
#define Wordlength 32
#define Nlimbs 18
#define Radix 29
#define Nbits 503
#define Nbytes 64

#define MONTGOMERY
// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 29u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 29u;
  n[0] &= mask;
  for (i = 1; i < 17; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 29u;
  }
  n[17] += (spint)carry;
  return -((n[17] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static spint flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0xc2d0871u) & carry;
  n[1] += ((spint)0x1f2f8114u) & carry;
  n[2] += ((spint)0x8456b55u) & carry;
  n[3] += ((spint)0x22e9b1u) & carry;
  n[4] += ((spint)0x86f8c91u) & carry;
  n[5] += ((spint)0xd5e0df7u) & carry;
  n[6] += ((spint)0x1f16af2bu) & carry;
  n[7] += ((spint)0x11b41960u) & carry;
  n[8] += ((spint)0x2c885u) & carry;
  n[17] += ((spint)0x360u) & carry;
  (void)prop(n);
  return (carry & 1);
}

// Montgomery final subtract
static spint modfsb(spint *n) {
  n[0] -= (spint)0xc2d0871u;
  n[1] -= (spint)0x1f2f8114u;
  n[2] -= (spint)0x8456b55u;
  n[3] -= (spint)0x22e9b1u;
  n[4] -= (spint)0x86f8c91u;
  n[5] -= (spint)0xd5e0df7u;
  n[6] -= (spint)0x1f16af2bu;
  n[7] -= (spint)0x11b41960u;
  n[8] -= (spint)0x2c885u;
  n[17] -= (spint)0x360u;
  return flatten(n);
}

// Modular addition - can be used in all cases - reduce less than 2p
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
  n[0] -= (spint)0x185a10e2u;
  n[1] -= (spint)0x3e5f0228u;
  n[2] -= (spint)0x108ad6aau;
  n[3] -= (spint)0x45d362u;
  n[4] -= (spint)0x10df1922u;
  n[5] -= (spint)0x1abc1beeu;
  n[6] -= (spint)0x3e2d5e56u;
  n[7] -= (spint)0x236832c0u;
  n[8] -= (spint)0x5910au;
  n[17] -= (spint)0x6c0u;
  carry = prop(n);
  n[0] += ((spint)0x185a10e2u) & carry;
  n[1] += ((spint)0x3e5f0228u) & carry;
  n[2] += ((spint)0x108ad6aau) & carry;
  n[3] += ((spint)0x45d362u) & carry;
  n[4] += ((spint)0x10df1922u) & carry;
  n[5] += ((spint)0x1abc1beeu) & carry;
  n[6] += ((spint)0x3e2d5e56u) & carry;
  n[7] += ((spint)0x236832c0u) & carry;
  n[8] += ((spint)0x5910au) & carry;
  n[17] += ((spint)0x6c0u) & carry;
  (void)prop(n);
}

// Fast modular addition - can be used if output is consumed ONLY by
// multiplication or squaring
static void modaddf(const spint *a, const spint *b, spint *n) {
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
  (void)prop(n);
}

// Modular subtraction - can be used in all cases - reduce less than 2p
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
  carry = prop(n);
  n[0] += ((spint)0x185a10e2u) & carry;
  n[1] += ((spint)0x3e5f0228u) & carry;
  n[2] += ((spint)0x108ad6aau) & carry;
  n[3] += ((spint)0x45d362u) & carry;
  n[4] += ((spint)0x10df1922u) & carry;
  n[5] += ((spint)0x1abc1beeu) & carry;
  n[6] += ((spint)0x3e2d5e56u) & carry;
  n[7] += ((spint)0x236832c0u) & carry;
  n[8] += ((spint)0x5910au) & carry;
  n[17] += ((spint)0x6c0u) & carry;
  (void)prop(n);
}

// Fast modular subtraction - can be used if output is ONLY consumed by
// multiplication or squaring
static void modsubf(const spint *a, const spint *b, spint *n) {
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
  n[0] += ((spint)0x185a10e2u);
  n[1] += ((spint)0x3e5f0228u);
  n[2] += ((spint)0x108ad6aau);
  n[3] += ((spint)0x45d362u);
  n[4] += ((spint)0x10df1922u);
  n[5] += ((spint)0x1abc1beeu);
  n[6] += ((spint)0x3e2d5e56u);
  n[7] += ((spint)0x236832c0u);
  n[8] += ((spint)0x5910au);
  n[17] += ((spint)0x6c0u);
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
  carry = prop(n);
  n[0] += ((spint)0x185a10e2u) & carry;
  n[1] += ((spint)0x3e5f0228u) & carry;
  n[2] += ((spint)0x108ad6aau) & carry;
  n[3] += ((spint)0x45d362u) & carry;
  n[4] += ((spint)0x10df1922u) & carry;
  n[5] += ((spint)0x1abc1beeu) & carry;
  n[6] += ((spint)0x3e2d5e56u) & carry;
  n[7] += ((spint)0x236832c0u) & carry;
  n[8] += ((spint)0x5910au) & carry;
  n[17] += ((spint)0x6c0u) & carry;
  (void)prop(n);
}

// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0xc2d0871u;
  spint p1 = 0x1f2f8114u;
  spint p2 = 0x8456b55u;
  spint p3 = 0x22e9b1u;
  spint p4 = 0x86f8c91u;
  spint p5 = 0xd5e0df7u;
  spint p6 = 0x1f16af2bu;
  spint p7 = 0x11b41960u;
  spint p8 = 0x2c885u;
  spint p17 = 0x360u;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xe42476fu;
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
  t += (dpint)v0 * (dpint)p5;
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
  t += (dpint)v0 * (dpint)p6;
  t += (dpint)v1 * (dpint)p5;
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  t += (dpint)v5 * (dpint)p1;
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
  t += (dpint)v0 * (dpint)p7;
  t += (dpint)v1 * (dpint)p6;
  t += (dpint)v2 * (dpint)p5;
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  t += (dpint)v6 * (dpint)p1;
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
  t >>= 29;
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
  t >>= 29;
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
  t >>= 29;
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
  t += (dpint)v3 * (dpint)p8;
  t += (dpint)v4 * (dpint)p7;
  t += (dpint)v5 * (dpint)p6;
  t += (dpint)v6 * (dpint)p5;
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
  t += (dpint)v9 * (dpint)p2;
  t += (dpint)v10 * (dpint)p1;
  spint v11 = (((spint)t * ndash) & mask);
  t += (dpint)v11 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v4 * (dpint)p8;
  t += (dpint)v5 * (dpint)p7;
  t += (dpint)v6 * (dpint)p6;
  t += (dpint)v7 * (dpint)p5;
  t += (dpint)v8 * (dpint)p4;
  t += (dpint)v9 * (dpint)p3;
  t += (dpint)v10 * (dpint)p2;
  t += (dpint)v11 * (dpint)p1;
  spint v12 = (((spint)t * ndash) & mask);
  t += (dpint)v12 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v5 * (dpint)p8;
  t += (dpint)v6 * (dpint)p7;
  t += (dpint)v7 * (dpint)p6;
  t += (dpint)v8 * (dpint)p5;
  t += (dpint)v9 * (dpint)p4;
  t += (dpint)v10 * (dpint)p3;
  t += (dpint)v11 * (dpint)p2;
  t += (dpint)v12 * (dpint)p1;
  spint v13 = (((spint)t * ndash) & mask);
  t += (dpint)v13 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)v8 * (dpint)p6;
  t += (dpint)v9 * (dpint)p5;
  t += (dpint)v10 * (dpint)p4;
  t += (dpint)v11 * (dpint)p3;
  t += (dpint)v12 * (dpint)p2;
  t += (dpint)v13 * (dpint)p1;
  spint v14 = (((spint)t * ndash) & mask);
  t += (dpint)v14 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  t += (dpint)v9 * (dpint)p6;
  t += (dpint)v10 * (dpint)p5;
  t += (dpint)v11 * (dpint)p4;
  t += (dpint)v12 * (dpint)p3;
  t += (dpint)v13 * (dpint)p2;
  t += (dpint)v14 * (dpint)p1;
  spint v15 = (((spint)t * ndash) & mask);
  t += (dpint)v15 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v8 * (dpint)p8;
  t += (dpint)v9 * (dpint)p7;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  t += (dpint)v12 * (dpint)p4;
  t += (dpint)v13 * (dpint)p3;
  t += (dpint)v14 * (dpint)p2;
  t += (dpint)v15 * (dpint)p1;
  spint v16 = (((spint)t * ndash) & mask);
  t += (dpint)v16 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v0 * (dpint)p17;
  t += (dpint)v9 * (dpint)p8;
  t += (dpint)v10 * (dpint)p7;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  t += (dpint)v13 * (dpint)p4;
  t += (dpint)v14 * (dpint)p3;
  t += (dpint)v15 * (dpint)p2;
  t += (dpint)v16 * (dpint)p1;
  spint v17 = (((spint)t * ndash) & mask);
  t += (dpint)v17 * (dpint)p0;
  t >>= 29;
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
  t += (dpint)v1 * (dpint)p17;
  t += (dpint)v10 * (dpint)p8;
  t += (dpint)v11 * (dpint)p7;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
  t += (dpint)v14 * (dpint)p4;
  t += (dpint)v15 * (dpint)p3;
  t += (dpint)v16 * (dpint)p2;
  t += (dpint)v17 * (dpint)p1;
  c[0] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v2 * (dpint)p17;
  t += (dpint)v11 * (dpint)p8;
  t += (dpint)v12 * (dpint)p7;
  t += (dpint)v13 * (dpint)p6;
  t += (dpint)v14 * (dpint)p5;
  t += (dpint)v15 * (dpint)p4;
  t += (dpint)v16 * (dpint)p3;
  t += (dpint)v17 * (dpint)p2;
  c[1] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v3 * (dpint)p17;
  t += (dpint)v12 * (dpint)p8;
  t += (dpint)v13 * (dpint)p7;
  t += (dpint)v14 * (dpint)p6;
  t += (dpint)v15 * (dpint)p5;
  t += (dpint)v16 * (dpint)p4;
  t += (dpint)v17 * (dpint)p3;
  c[2] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v4 * (dpint)p17;
  t += (dpint)v13 * (dpint)p8;
  t += (dpint)v14 * (dpint)p7;
  t += (dpint)v15 * (dpint)p6;
  t += (dpint)v16 * (dpint)p5;
  t += (dpint)v17 * (dpint)p4;
  c[3] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v5 * (dpint)p17;
  t += (dpint)v14 * (dpint)p8;
  t += (dpint)v15 * (dpint)p7;
  t += (dpint)v16 * (dpint)p6;
  t += (dpint)v17 * (dpint)p5;
  c[4] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v6 * (dpint)p17;
  t += (dpint)v15 * (dpint)p8;
  t += (dpint)v16 * (dpint)p7;
  t += (dpint)v17 * (dpint)p6;
  c[5] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v7 * (dpint)p17;
  t += (dpint)v16 * (dpint)p8;
  t += (dpint)v17 * (dpint)p7;
  c[6] = ((spint)t & mask);
  t >>= 29;
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
  t += (dpint)v8 * (dpint)p17;
  t += (dpint)v17 * (dpint)p8;
  c[7] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[9] * b[17];
  t += (dpint)a[10] * b[16];
  t += (dpint)a[11] * b[15];
  t += (dpint)a[12] * b[14];
  t += (dpint)a[13] * b[13];
  t += (dpint)a[14] * b[12];
  t += (dpint)a[15] * b[11];
  t += (dpint)a[16] * b[10];
  t += (dpint)a[17] * b[9];
  t += (dpint)v9 * (dpint)p17;
  c[8] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[10] * b[17];
  t += (dpint)a[11] * b[16];
  t += (dpint)a[12] * b[15];
  t += (dpint)a[13] * b[14];
  t += (dpint)a[14] * b[13];
  t += (dpint)a[15] * b[12];
  t += (dpint)a[16] * b[11];
  t += (dpint)a[17] * b[10];
  t += (dpint)v10 * (dpint)p17;
  c[9] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[11] * b[17];
  t += (dpint)a[12] * b[16];
  t += (dpint)a[13] * b[15];
  t += (dpint)a[14] * b[14];
  t += (dpint)a[15] * b[13];
  t += (dpint)a[16] * b[12];
  t += (dpint)a[17] * b[11];
  t += (dpint)v11 * (dpint)p17;
  c[10] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[12] * b[17];
  t += (dpint)a[13] * b[16];
  t += (dpint)a[14] * b[15];
  t += (dpint)a[15] * b[14];
  t += (dpint)a[16] * b[13];
  t += (dpint)a[17] * b[12];
  t += (dpint)v12 * (dpint)p17;
  c[11] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[13] * b[17];
  t += (dpint)a[14] * b[16];
  t += (dpint)a[15] * b[15];
  t += (dpint)a[16] * b[14];
  t += (dpint)a[17] * b[13];
  t += (dpint)v13 * (dpint)p17;
  c[12] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[14] * b[17];
  t += (dpint)a[15] * b[16];
  t += (dpint)a[16] * b[15];
  t += (dpint)a[17] * b[14];
  t += (dpint)v14 * (dpint)p17;
  c[13] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[15] * b[17];
  t += (dpint)a[16] * b[16];
  t += (dpint)a[17] * b[15];
  t += (dpint)v15 * (dpint)p17;
  c[14] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[16] * b[17];
  t += (dpint)a[17] * b[16];
  t += (dpint)v16 * (dpint)p17;
  c[15] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[17] * b[17];
  t += (dpint)v17 * (dpint)p17;
  c[16] = ((spint)t & mask);
  t >>= 29;
  c[17] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  dpint tot;
  dpint t = 0;
  spint p0 = 0xc2d0871u;
  spint p1 = 0x1f2f8114u;
  spint p2 = 0x8456b55u;
  spint p3 = 0x22e9b1u;
  spint p4 = 0x86f8c91u;
  spint p5 = 0xd5e0df7u;
  spint p6 = 0x1f16af2bu;
  spint p7 = 0x11b41960u;
  spint p8 = 0x2c885u;
  spint p17 = 0x360u;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xe42476fu;
  tot = (dpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[2];
  tot *= 2;
  tot += (dpint)a[1] * a[1];
  t += tot;
  t += (dpint)v0 * p2;
  t += (dpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[3];
  tot += (dpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p3;
  t += (dpint)v1 * p2;
  t += (dpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * p0;
  t >>= 29;
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
  t >>= 29;
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
  t >>= 29;
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
  t >>= 29;
  tot = (dpint)a[0] * a[7];
  tot += (dpint)a[1] * a[6];
  tot += (dpint)a[2] * a[5];
  tot += (dpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p7;
  t += (dpint)v1 * p6;
  t += (dpint)v2 * p5;
  t += (dpint)v3 * p4;
  t += (dpint)v4 * p3;
  t += (dpint)v5 * p2;
  t += (dpint)v6 * p1;
  spint v7 = (((spint)t * ndash) & mask);
  t += (dpint)v7 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[8];
  tot += (dpint)a[1] * a[7];
  tot += (dpint)a[2] * a[6];
  tot += (dpint)a[3] * a[5];
  tot *= 2;
  tot += (dpint)a[4] * a[4];
  t += tot;
  t += (dpint)v0 * p8;
  t += (dpint)v1 * p7;
  t += (dpint)v2 * p6;
  t += (dpint)v3 * p5;
  t += (dpint)v4 * p4;
  t += (dpint)v5 * p3;
  t += (dpint)v6 * p2;
  t += (dpint)v7 * p1;
  spint v8 = (((spint)t * ndash) & mask);
  t += (dpint)v8 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[9];
  tot += (dpint)a[1] * a[8];
  tot += (dpint)a[2] * a[7];
  tot += (dpint)a[3] * a[6];
  tot += (dpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (dpint)v1 * p8;
  t += (dpint)v2 * p7;
  t += (dpint)v3 * p6;
  t += (dpint)v4 * p5;
  t += (dpint)v5 * p4;
  t += (dpint)v6 * p3;
  t += (dpint)v7 * p2;
  t += (dpint)v8 * p1;
  spint v9 = (((spint)t * ndash) & mask);
  t += (dpint)v9 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[10];
  tot += (dpint)a[1] * a[9];
  tot += (dpint)a[2] * a[8];
  tot += (dpint)a[3] * a[7];
  tot += (dpint)a[4] * a[6];
  tot *= 2;
  tot += (dpint)a[5] * a[5];
  t += tot;
  t += (dpint)v2 * p8;
  t += (dpint)v3 * p7;
  t += (dpint)v4 * p6;
  t += (dpint)v5 * p5;
  t += (dpint)v6 * p4;
  t += (dpint)v7 * p3;
  t += (dpint)v8 * p2;
  t += (dpint)v9 * p1;
  spint v10 = (((spint)t * ndash) & mask);
  t += (dpint)v10 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[11];
  tot += (dpint)a[1] * a[10];
  tot += (dpint)a[2] * a[9];
  tot += (dpint)a[3] * a[8];
  tot += (dpint)a[4] * a[7];
  tot += (dpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  t += (dpint)v3 * p8;
  t += (dpint)v4 * p7;
  t += (dpint)v5 * p6;
  t += (dpint)v6 * p5;
  t += (dpint)v7 * p4;
  t += (dpint)v8 * p3;
  t += (dpint)v9 * p2;
  t += (dpint)v10 * p1;
  spint v11 = (((spint)t * ndash) & mask);
  t += (dpint)v11 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[12];
  tot += (dpint)a[1] * a[11];
  tot += (dpint)a[2] * a[10];
  tot += (dpint)a[3] * a[9];
  tot += (dpint)a[4] * a[8];
  tot += (dpint)a[5] * a[7];
  tot *= 2;
  tot += (dpint)a[6] * a[6];
  t += tot;
  t += (dpint)v4 * p8;
  t += (dpint)v5 * p7;
  t += (dpint)v6 * p6;
  t += (dpint)v7 * p5;
  t += (dpint)v8 * p4;
  t += (dpint)v9 * p3;
  t += (dpint)v10 * p2;
  t += (dpint)v11 * p1;
  spint v12 = (((spint)t * ndash) & mask);
  t += (dpint)v12 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[13];
  tot += (dpint)a[1] * a[12];
  tot += (dpint)a[2] * a[11];
  tot += (dpint)a[3] * a[10];
  tot += (dpint)a[4] * a[9];
  tot += (dpint)a[5] * a[8];
  tot += (dpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  t += (dpint)v5 * p8;
  t += (dpint)v6 * p7;
  t += (dpint)v7 * p6;
  t += (dpint)v8 * p5;
  t += (dpint)v9 * p4;
  t += (dpint)v10 * p3;
  t += (dpint)v11 * p2;
  t += (dpint)v12 * p1;
  spint v13 = (((spint)t * ndash) & mask);
  t += (dpint)v13 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[14];
  tot += (dpint)a[1] * a[13];
  tot += (dpint)a[2] * a[12];
  tot += (dpint)a[3] * a[11];
  tot += (dpint)a[4] * a[10];
  tot += (dpint)a[5] * a[9];
  tot += (dpint)a[6] * a[8];
  tot *= 2;
  tot += (dpint)a[7] * a[7];
  t += tot;
  t += (dpint)v6 * p8;
  t += (dpint)v7 * p7;
  t += (dpint)v8 * p6;
  t += (dpint)v9 * p5;
  t += (dpint)v10 * p4;
  t += (dpint)v11 * p3;
  t += (dpint)v12 * p2;
  t += (dpint)v13 * p1;
  spint v14 = (((spint)t * ndash) & mask);
  t += (dpint)v14 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[15];
  tot += (dpint)a[1] * a[14];
  tot += (dpint)a[2] * a[13];
  tot += (dpint)a[3] * a[12];
  tot += (dpint)a[4] * a[11];
  tot += (dpint)a[5] * a[10];
  tot += (dpint)a[6] * a[9];
  tot += (dpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (dpint)v7 * p8;
  t += (dpint)v8 * p7;
  t += (dpint)v9 * p6;
  t += (dpint)v10 * p5;
  t += (dpint)v11 * p4;
  t += (dpint)v12 * p3;
  t += (dpint)v13 * p2;
  t += (dpint)v14 * p1;
  spint v15 = (((spint)t * ndash) & mask);
  t += (dpint)v15 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[16];
  tot += (dpint)a[1] * a[15];
  tot += (dpint)a[2] * a[14];
  tot += (dpint)a[3] * a[13];
  tot += (dpint)a[4] * a[12];
  tot += (dpint)a[5] * a[11];
  tot += (dpint)a[6] * a[10];
  tot += (dpint)a[7] * a[9];
  tot *= 2;
  tot += (dpint)a[8] * a[8];
  t += tot;
  t += (dpint)v8 * p8;
  t += (dpint)v9 * p7;
  t += (dpint)v10 * p6;
  t += (dpint)v11 * p5;
  t += (dpint)v12 * p4;
  t += (dpint)v13 * p3;
  t += (dpint)v14 * p2;
  t += (dpint)v15 * p1;
  spint v16 = (((spint)t * ndash) & mask);
  t += (dpint)v16 * p0;
  t >>= 29;
  tot = (dpint)a[0] * a[17];
  tot += (dpint)a[1] * a[16];
  tot += (dpint)a[2] * a[15];
  tot += (dpint)a[3] * a[14];
  tot += (dpint)a[4] * a[13];
  tot += (dpint)a[5] * a[12];
  tot += (dpint)a[6] * a[11];
  tot += (dpint)a[7] * a[10];
  tot += (dpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  t += (dpint)v0 * p17;
  t += (dpint)v9 * p8;
  t += (dpint)v10 * p7;
  t += (dpint)v11 * p6;
  t += (dpint)v12 * p5;
  t += (dpint)v13 * p4;
  t += (dpint)v14 * p3;
  t += (dpint)v15 * p2;
  t += (dpint)v16 * p1;
  spint v17 = (((spint)t * ndash) & mask);
  t += (dpint)v17 * p0;
  t >>= 29;
  tot = (dpint)a[1] * a[17];
  tot += (dpint)a[2] * a[16];
  tot += (dpint)a[3] * a[15];
  tot += (dpint)a[4] * a[14];
  tot += (dpint)a[5] * a[13];
  tot += (dpint)a[6] * a[12];
  tot += (dpint)a[7] * a[11];
  tot += (dpint)a[8] * a[10];
  tot *= 2;
  tot += (dpint)a[9] * a[9];
  t += tot;
  t += (dpint)v1 * p17;
  t += (dpint)v10 * p8;
  t += (dpint)v11 * p7;
  t += (dpint)v12 * p6;
  t += (dpint)v13 * p5;
  t += (dpint)v14 * p4;
  t += (dpint)v15 * p3;
  t += (dpint)v16 * p2;
  t += (dpint)v17 * p1;
  c[0] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[2] * a[17];
  tot += (dpint)a[3] * a[16];
  tot += (dpint)a[4] * a[15];
  tot += (dpint)a[5] * a[14];
  tot += (dpint)a[6] * a[13];
  tot += (dpint)a[7] * a[12];
  tot += (dpint)a[8] * a[11];
  tot += (dpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  t += (dpint)v2 * p17;
  t += (dpint)v11 * p8;
  t += (dpint)v12 * p7;
  t += (dpint)v13 * p6;
  t += (dpint)v14 * p5;
  t += (dpint)v15 * p4;
  t += (dpint)v16 * p3;
  t += (dpint)v17 * p2;
  c[1] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[3] * a[17];
  tot += (dpint)a[4] * a[16];
  tot += (dpint)a[5] * a[15];
  tot += (dpint)a[6] * a[14];
  tot += (dpint)a[7] * a[13];
  tot += (dpint)a[8] * a[12];
  tot += (dpint)a[9] * a[11];
  tot *= 2;
  tot += (dpint)a[10] * a[10];
  t += tot;
  t += (dpint)v3 * p17;
  t += (dpint)v12 * p8;
  t += (dpint)v13 * p7;
  t += (dpint)v14 * p6;
  t += (dpint)v15 * p5;
  t += (dpint)v16 * p4;
  t += (dpint)v17 * p3;
  c[2] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[4] * a[17];
  tot += (dpint)a[5] * a[16];
  tot += (dpint)a[6] * a[15];
  tot += (dpint)a[7] * a[14];
  tot += (dpint)a[8] * a[13];
  tot += (dpint)a[9] * a[12];
  tot += (dpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  t += (dpint)v4 * p17;
  t += (dpint)v13 * p8;
  t += (dpint)v14 * p7;
  t += (dpint)v15 * p6;
  t += (dpint)v16 * p5;
  t += (dpint)v17 * p4;
  c[3] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[5] * a[17];
  tot += (dpint)a[6] * a[16];
  tot += (dpint)a[7] * a[15];
  tot += (dpint)a[8] * a[14];
  tot += (dpint)a[9] * a[13];
  tot += (dpint)a[10] * a[12];
  tot *= 2;
  tot += (dpint)a[11] * a[11];
  t += tot;
  t += (dpint)v5 * p17;
  t += (dpint)v14 * p8;
  t += (dpint)v15 * p7;
  t += (dpint)v16 * p6;
  t += (dpint)v17 * p5;
  c[4] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[6] * a[17];
  tot += (dpint)a[7] * a[16];
  tot += (dpint)a[8] * a[15];
  tot += (dpint)a[9] * a[14];
  tot += (dpint)a[10] * a[13];
  tot += (dpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  t += (dpint)v6 * p17;
  t += (dpint)v15 * p8;
  t += (dpint)v16 * p7;
  t += (dpint)v17 * p6;
  c[5] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[7] * a[17];
  tot += (dpint)a[8] * a[16];
  tot += (dpint)a[9] * a[15];
  tot += (dpint)a[10] * a[14];
  tot += (dpint)a[11] * a[13];
  tot *= 2;
  tot += (dpint)a[12] * a[12];
  t += tot;
  t += (dpint)v7 * p17;
  t += (dpint)v16 * p8;
  t += (dpint)v17 * p7;
  c[6] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[8] * a[17];
  tot += (dpint)a[9] * a[16];
  tot += (dpint)a[10] * a[15];
  tot += (dpint)a[11] * a[14];
  tot += (dpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  t += (dpint)v8 * p17;
  t += (dpint)v17 * p8;
  c[7] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[9] * a[17];
  tot += (dpint)a[10] * a[16];
  tot += (dpint)a[11] * a[15];
  tot += (dpint)a[12] * a[14];
  tot *= 2;
  tot += (dpint)a[13] * a[13];
  t += tot;
  t += (dpint)v9 * p17;
  c[8] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[10] * a[17];
  tot += (dpint)a[11] * a[16];
  tot += (dpint)a[12] * a[15];
  tot += (dpint)a[13] * a[14];
  tot *= 2;
  t += tot;
  t += (dpint)v10 * p17;
  c[9] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[11] * a[17];
  tot += (dpint)a[12] * a[16];
  tot += (dpint)a[13] * a[15];
  tot *= 2;
  tot += (dpint)a[14] * a[14];
  t += tot;
  t += (dpint)v11 * p17;
  c[10] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[12] * a[17];
  tot += (dpint)a[13] * a[16];
  tot += (dpint)a[14] * a[15];
  tot *= 2;
  t += tot;
  t += (dpint)v12 * p17;
  c[11] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[13] * a[17];
  tot += (dpint)a[14] * a[16];
  tot *= 2;
  tot += (dpint)a[15] * a[15];
  t += tot;
  t += (dpint)v13 * p17;
  c[12] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[14] * a[17];
  tot += (dpint)a[15] * a[16];
  tot *= 2;
  t += tot;
  t += (dpint)v14 * p17;
  c[13] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[15] * a[17];
  tot *= 2;
  tot += (dpint)a[16] * a[16];
  t += tot;
  t += (dpint)v15 * p17;
  c[14] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[16] * a[17];
  tot *= 2;
  t += tot;
  t += (dpint)v16 * p17;
  c[15] = ((spint)t & mask);
  t >>= 29;
  tot = (dpint)a[17] * a[17];
  t += tot;
  t += (dpint)v17 * p17;
  c[16] = ((spint)t & mask);
  t >>= 29;
  c[17] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 18; i++) {
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
  spint x[18];
  spint t0[18];
  spint t1[18];
  spint t2[18];
  spint t3[18];
  spint t4[18];
  spint t5[18];
  spint t6[18];
  spint t7[18];
  spint t8[18];
  spint t9[18];
  spint t10[18];
  spint t11[18];
  spint t12[18];
  spint t13[18];
  spint t14[18];
  spint t15[18];
  spint t16[18];
  modcpy(w, x);
  modsqr(x, t11);
  modmul(x, t11, t14);
  modmul(t11, t14, t7);
  modmul(x, t7, t6);
  modmul(t11, t6, t0);
  modmul(x, t0, t10);
  modmul(t11, t10, t15);
  modmul(t6, t10, t12);
  modmul(t11, t12, t2);
  modmul(t0, t2, t4);
  modmul(t11, t4, t8);
  modmul(t11, t8, t9);
  modmul(t11, t9, t3);
  modmul(t11, t3, z);
  modmul(t11, z, t1);
  modmul(t0, t1, t13);
  modmul(t11, t13, t0);
  modmul(t11, t0, t5);
  modmul(t6, t5, t6);
  modmul(t11, t6, t11);
  modmul(t6, t11, t16);
  modnsqr(t16, 250);
  modmul(t15, t16, t16);
  modnsqr(t16, 7);
  modmul(t2, t16, t16);
  modnsqr(t16, 8);
  modmul(t15, t16, t15);
  modnsqr(t15, 8);
  modmul(t8, t15, t15);
  modnsqr(t15, 2);
  modmul(x, t15, t15);
  modnsqr(t15, 10);
  modmul(t4, t15, t15);
  modnsqr(t15, 3);
  modmul(t14, t15, t14);
  modnsqr(t14, 10);
  modmul(t3, t14, t14);
  modnsqr(t14, 9);
  modmul(t0, t14, t14);
  modnsqr(t14, 7);
  modmul(t5, t14, t14);
  modnsqr(t14, 8);
  modmul(t13, t14, t13);
  modnsqr(t13, 7);
  modmul(t6, t13, t13);
  modnsqr(t13, 5);
  modmul(t12, t13, t12);
  modnsqr(t12, 11);
  modmul(t11, t12, t12);
  modnsqr(t12, 6);
  modmul(t11, t12, t11);
  modnsqr(t11, 7);
  modmul(z, t11, t11);
  modnsqr(t11, 6);
  modmul(t5, t11, t11);
  modnsqr(t11, 6);
  modmul(t1, t11, t11);
  modnsqr(t11, 6);
  modmul(t10, t11, t10);
  modnsqr(t10, 4);
  modmul(x, t10, t10);
  modnsqr(t10, 12);
  modmul(t2, t10, t10);
  modnsqr(t10, 6);
  modmul(t9, t10, t9);
  modnsqr(t9, 7);
  modmul(t8, t9, t8);
  modnsqr(t8, 6);
  modmul(t7, t8, t7);
  modnsqr(t7, 9);
  modmul(t2, t7, t7);
  modnsqr(t7, 7);
  modmul(t0, t7, t7);
  modnsqr(t7, 7);
  modmul(t6, t7, t6);
  modnsqr(t6, 7);
  modmul(t5, t6, t5);
  modnsqr(t5, 5);
  modmul(t4, t5, t4);
  modnsqr(t4, 6);
  modmul(t3, t4, t3);
  modnsqr(t3, 11);
  modmul(t2, t3, t2);
  modnsqr(t2, 7);
  modmul(t1, t2, t1);
  modnsqr(t1, 10);
  modmul(t0, t1, t0);
  modnsqr(t0, 10);
  modmul(z, t0, z);
  modsqr(z, z);
  modmul(x, z, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  int i;
  spint s[18];
  spint t[18];
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
  const spint c[18] = {0x11d3dc0du, 0x58aac3cu, 0x78cf84du,  0x1eefcf47u,
                       0x8d700dcu,  0xdafdba0u, 0x1b67bf3fu, 0x1f1b89c7u,
                       0x5d10037u,  0xed5a1dfu, 0x1fa9a139u, 0x10aebc17u,
                       0x3755164u,  0xad0cdcu,  0x1916a5d6u, 0x3f501c3u,
                       0xf4e673bu,  0x271u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[18];
  c[0] = 1;
  for (i = 1; i < 18; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[18];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 18; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 29u) &
          (((c0 ^ (spint)1) - (spint)1) >> 29u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[18];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 18; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 29u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 18; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 18; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 18; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Modular multiplication by an integer, c=a*b mod 2p
// uses special method for trinomials, otherwise Barrett-Dhem reduction
static void modmli(const spint *a, int b, spint *c) {
  spint p0 = 0xc2d0871u;
  spint p1 = 0x1f2f8114u;
  spint p2 = 0x8456b55u;
  spint p3 = 0x22e9b1u;
  spint p4 = 0x86f8c91u;
  spint p5 = 0xd5e0df7u;
  spint p6 = 0x1f16af2bu;
  spint p7 = 0x11b41960u;
  spint p8 = 0x2c885u;
  spint p17 = 0x360u;
  spint mask = ((spint)1 << 29u) - (spint)1;
  dpint t = 0;
  spint q, h, r = 0x25ed097b;
  t += (dpint)a[0] * (dpint)b;
  c[0] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[1] * (dpint)b;
  c[1] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[2] * (dpint)b;
  c[2] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[3] * (dpint)b;
  c[3] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[4] * (dpint)b;
  c[4] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[5] * (dpint)b;
  c[5] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[6] * (dpint)b;
  c[6] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[7] * (dpint)b;
  c[7] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[8] * (dpint)b;
  c[8] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[9] * (dpint)b;
  c[9] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[10] * (dpint)b;
  c[10] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[11] * (dpint)b;
  c[11] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[12] * (dpint)b;
  c[12] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[13] * (dpint)b;
  c[13] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[14] * (dpint)b;
  c[14] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[15] * (dpint)b;
  c[15] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[16] * (dpint)b;
  c[16] = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[17] * (dpint)b;
  c[17] = (spint)t;

  // Barrett-Dhem reduction
  h = (spint)(t >> 7u);
  q = (spint)(((dpint)h * (dpint)r) >> 32u);
  t = (dpint)q * (dpint)p0;
  c[0] -= (spint)t & mask;
  c[1] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p1;
  c[1] -= (spint)t & mask;
  c[2] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p2;
  c[2] -= (spint)t & mask;
  c[3] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p3;
  c[3] -= (spint)t & mask;
  c[4] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p4;
  c[4] -= (spint)t & mask;
  c[5] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p5;
  c[5] -= (spint)t & mask;
  c[6] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p6;
  c[6] -= (spint)t & mask;
  c[7] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p7;
  c[7] -= (spint)t & mask;
  c[8] -= (spint)(t >> 29u);
  t = (dpint)q * (dpint)p8;
  c[8] -= (spint)t & mask;
  c[9] -= (spint)(t >> 29u);
  c[17] -= q * p17;
  (void)prop(c);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[18];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
  modnsqr(r, 3);
  return modis1(r) | modis0(x);
}

// conditional move g to f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void __attribute__((noinline)) modcmv(int b, const spint *g,
                                             volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, aux;
  static spint R = 0;
  R += 0x5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 18; i++) {
    s = g[i];
    t = f[i];
    f[i] = aux = c0 * t + c1 * s;
    f[i] = aux - w * (t + s);
  }
}

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void __attribute__((noinline)) modcsw(int b, volatile spint *g,
                                             volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, v, aux;
  static spint R = 0;
  R += 0x5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 18; i++) {
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
  int k;
  spint t[18];
  spint b[18];
  spint v[18];
  spint z[18] = {0xc40dd0cu,  0x2daea70u, 0x169e0d26u, 0x6f2e0eu,   0x1317067eu,
                 0x12924007u, 0xdc7eb25u, 0x1bc4a36bu, 0x15829bceu, 0x8d65c9bu,
                 0x63a3023u,  0x5fde2fcu, 0x10f45c82u, 0xed393ebu,  0x16ac35ffu,
                 0x113b56c8u, 0x1e2df0bu, 0x336u};
  spint s[18];
  spint y[18];
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
    modcmv(d, v, s);
    modsqr(z, z);
    modmul(t, z, v);
    modcmv(d, v, t);
  }
  modcpy(s, r);
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
  int i;
  a[17] = ((a[17] << n)) + (a[16] >> (29u - n));
  for (i = 16; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x1fffffff) + (a[i - 1] >> (29u - n));
  }
  a[0] = (a[0] << n) & (spint)0x1fffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 17; i++) {
    a[i] = (a[i] >> n) + ((a[i + 1] << (29u - n)) & (spint)0x1fffffff);
  }
  a[17] = a[17] >> n;
  return r;
}

// divide by 2. Shift right 1 bit (or add p and shift right one bit)
static void modhaf(spint *n) {
  int lsb;
  spint t[18];
  (void)prop(n);
  modcpy(n, t);
  lsb = modshr(1, t);
  n[0] += ((spint)0xc2d0871u);
  n[1] += ((spint)0x1f2f8114u);
  n[2] += ((spint)0x8456b55u);
  n[3] += ((spint)0x22e9b1u);
  n[4] += ((spint)0x86f8c91u);
  n[5] += ((spint)0xd5e0df7u);
  n[6] += ((spint)0x1f16af2bu);
  n[7] += ((spint)0x11b41960u);
  n[8] += ((spint)0x2c885u);
  n[17] += ((spint)0x360u);
  (void)prop(n);
  modshr(1, n);
  modcmv(1 - lsb, t, n);
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 29u;
  unsigned int m = r % 29u;
  modzer(a);
  if (r >= 64 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
  nres(a, a);
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[18];
  redc(a, c);
  for (i = 63; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 18; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 64; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = (int)modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[18];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[18], d[18];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 18; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
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

// reduce 1024 bit array h to integer r modulo group order q, in constant time
// Consider h as 2^496.(2^496x + y) + z, where x,y and z < q (z is bottom 496 bits, y is next 496 bits, x is top 16 bits)
// Important that x and y < q, q is group order
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,z,c;

    mod2r(496,c);

    for (i=0;i<62;i++)  // bottom 62.8=496 bits
        buff[i]=h[i];
    buff[62]=0; buff[63]=0;
    reverse(buff);
    modimp(buff,z);

    for (i=0;i<62;i++)  // middle 496 bits
        buff[i]=h[i+62];
    buff[62]=0; buff[63]=0;
    reverse(buff);
    modimp(buff,y);  
    
    for (i=0;i<4;i++)  // top 16 bits
       buff[i]=h[124+i];   
    for (i=4;i<64;i++)
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
void ED500_KEY_PAIR(char *prv,char *pub)
{
    int sign;
    point P;
    char s[BYTES];
    ecn_ed500_gen(&P);  // get curve generator point

    H(BYTES,BYTES,prv,s);
// clamp s
    s[0]&=0xFC;
    s[62]&=0x7F; s[62]|=0x40; s[63]=0;

    reverse(s);  // little endian to big endian
    ecn_ed500_mul(s,&P); 

    sign=ecn_ed500_get(&P,NULL,pub);  // get y coordinate and sign
    reverse(pub);              // big endian to little endian
    pub[BYTES-1]|=(char)(sign<<7);
}

static const char dom4[10]={'S','i','g','E','d','5','0','0',0,0};

// Note that a per message "random" number usually generated by a Cryptographically Secure Random Number Generator (CSRNG) is often required, and if so is calculated externally and passed into the signature function
// Methods are
// 1. non-deterministic - a non-biased high entropy random number in the range from 0 - q (the group order) - recommended ii CSRNG is good
// 2. deterministic - derived from a hash of the message to be signed and the secret key (must be done like this for EdDSA, optional for ECDSA)
// 3. hedged - a combination of methods 1 and 2 - recommended if CSRNG is poor
// Note that this functionality is out-of-scope for this exercise (except for EdDSA). For examples see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf and RFC6979

// To create such a per message random number, consider the method reduce() from Ed448.c, which reduces a much larger number modulo q, as required to minimize bias

// input private key, per-message random number (or public key), message length, message to be signed
// Output signature.
void ED500_SIGN(char *prv,char *pub,int mlen,char *m,char *sig)
{
    int i,sign;
    char h[2*BYTES];
    char ipub[BYTES];
    sha3 SHA3;
    point R;
    gel r,s,d;     // some group elements..
    ecn_ed500_gen(&R);  // get curve generator point

    if (pub!=NULL)
    {
        for (i=0;i<BYTES;i++)
            ipub[i]=pub[i];
    } else {
        ED500_KEY_PAIR(prv,ipub);
    }

    SHA3_init(&SHA3,SHAKE256);
    H(BYTES,2*BYTES,prv,h);

// derive and clamp s
    h[0]&=0xFC;
    h[62]&=0x7F; h[62]|=0x40; h[63]=0;
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
    ecn_ed500_mul(h,&R);

    sign=ecn_ed500_get(&R,NULL,sig);  // get y coordinate and sign
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
int ED500_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    int i;
    point G,R,Q;
    gel u;     // some group elements..
    int sign;
    sha3 SHA3;
    char h[2*BYTES];
    char buff[BYTES];

    ecn_ed500_gen(&G);  // get curve generator point

// reconstruct point R
    sign=(sig[BYTES-1]>>7)&1; 
    for (i=0;i<BYTES;i++)
        buff[i]=sig[i];
    buff[BYTES-1]&=0x7f;
    reverse(buff);
    ecn_ed500_set(sign,NULL,buff,&R);
    if (ecn_ed500_isinf(&R)) return 0;

// reconstruct point Q 
    sign=(pub[BYTES-1]>>7)&1;
    for (i=0;i<BYTES;i++)
        buff[i]=pub[i];
    buff[BYTES-1]&=0x7f;
    reverse(buff);
    ecn_ed500_set(sign,NULL,buff,&Q); 
    if (ecn_ed500_isinf(&Q)) return 0;

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

    ecn_ed500_cof(&G); ecn_ed500_cof(&R); ecn_ed500_cof(&Q);
    ecn_ed500_mul2(buff,&G,h,&Q,&Q);

    if (ecn_ed500_cmp(&R,&Q))
        return 1;
    return 0;
}



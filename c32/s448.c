// Ed448 Implementation
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
// and RFC8032
// python curve.py 64 ED448
// This completes edwards.rs for this curve. Then
// gcc -O2 Ed448.c edwards.c hash.c -o Ed448

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "Ed448curve.h"   // elliptic curve API

/*** Insert code automatically generated from group.c here ***/
/* Note that much of this code is not needed and can be deleted */

// Command line : python monty.py 32 ed448

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 16
#define Radix 28
#define Nbits 446
#define Nbytes 56

#define MONTGOMERY
#define ED448

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 28u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 28u;
  n[0] &= mask;
  for (i = 1; i < 15; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[15] += (spint)carry;
  return -((n[15] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0xb5844f3u) & carry;
  n[1] += ((spint)0x78c292au) & carry;
  n[2] += ((spint)0x58f5523u) & carry;
  n[3] += ((spint)0xc2728dcu) & carry;
  n[4] += ((spint)0x690216cu) & carry;
  n[5] += ((spint)0x49aed63u) & carry;
  n[6] += ((spint)0x9c44edbu) & carry;
  n[7] += ((spint)0x7cca23eu) & carry;
  n[8] -= (spint)1u & carry;
  n[15] += ((spint)0x4000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] -= (spint)0xb5844f3u;
  n[1] -= (spint)0x78c292au;
  n[2] -= (spint)0x58f5523u;
  n[3] -= (spint)0xc2728dcu;
  n[4] -= (spint)0x690216cu;
  n[5] -= (spint)0x49aed63u;
  n[6] -= (spint)0x9c44edbu;
  n[7] -= (spint)0x7cca23eu;
  n[8] += (spint)1u;
  n[15] -= (spint)0x4000000u;
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
  n[0] -= (spint)0x16b089e6u;
  n[1] -= (spint)0xf185254u;
  n[2] -= (spint)0xb1eaa46u;
  n[3] -= (spint)0x184e51b8u;
  n[4] -= (spint)0xd2042d8u;
  n[5] -= (spint)0x935dac6u;
  n[6] -= (spint)0x13889db6u;
  n[7] -= (spint)0xf99447cu;
  n[8] += (spint)2u;
  n[15] -= (spint)0x8000000u;
  carry = prop(n);
  n[0] += ((spint)0x16b089e6u) & carry;
  n[1] += ((spint)0xf185254u) & carry;
  n[2] += ((spint)0xb1eaa46u) & carry;
  n[3] += ((spint)0x184e51b8u) & carry;
  n[4] += ((spint)0xd2042d8u) & carry;
  n[5] += ((spint)0x935dac6u) & carry;
  n[6] += ((spint)0x13889db6u) & carry;
  n[7] += ((spint)0xf99447cu) & carry;
  n[8] -= (spint)2u & carry;
  n[15] += ((spint)0x8000000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0x16b089e6u) & carry;
  n[1] += ((spint)0xf185254u) & carry;
  n[2] += ((spint)0xb1eaa46u) & carry;
  n[3] += ((spint)0x184e51b8u) & carry;
  n[4] += ((spint)0xd2042d8u) & carry;
  n[5] += ((spint)0x935dac6u) & carry;
  n[6] += ((spint)0x13889db6u) & carry;
  n[7] += ((spint)0xf99447cu) & carry;
  n[8] -= (spint)2u & carry;
  n[15] += ((spint)0x8000000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0x16b089e6u) & carry;
  n[1] += ((spint)0xf185254u) & carry;
  n[2] += ((spint)0xb1eaa46u) & carry;
  n[3] += ((spint)0x184e51b8u) & carry;
  n[4] += ((spint)0xd2042d8u) & carry;
  n[5] += ((spint)0x935dac6u) & carry;
  n[6] += ((spint)0x13889db6u) & carry;
  n[7] += ((spint)0xf99447cu) & carry;
  n[8] -= (spint)2u & carry;
  n[15] += ((spint)0x8000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1465199085000135691
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0xb5844f3u;
  spint p1 = 0x78c292au;
  spint p2 = 0x58f5523u;
  spint p3 = 0xc2728dcu;
  spint p4 = 0x690216cu;
  spint p5 = 0x49aed63u;
  spint p6 = 0x9c44edbu;
  spint p7 = 0x7cca23eu;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xe918bc5u;
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
  t += (dpint)(spint)(q - v0);
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
  spint s = (spint)mask;
  s -= v1;
  t += (dpint)v2 * (dpint)p7;
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
  s -= v2;
  t += (dpint)v3 * (dpint)p7;
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
  s -= v3;
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
  s -= v4;
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
  s -= v5;
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
  s -= v6;
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
  t += (dpint)(udpint)((udpint)v0 << 26u);
  s -= v7;
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
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v1 << 26u);
  s -= v8;
  t += (dpint)v9 * (dpint)p7;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  t += (dpint)v12 * (dpint)p4;
  t += (dpint)v13 * (dpint)p3;
  t += (dpint)v14 * (dpint)p2;
  t += (dpint)v15 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 28;
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
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v2 << 26u);
  s -= v9;
  t += (dpint)v10 * (dpint)p7;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  t += (dpint)v13 * (dpint)p4;
  t += (dpint)v14 * (dpint)p3;
  t += (dpint)v15 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 28;
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
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v3 << 26u);
  s -= v10;
  t += (dpint)v11 * (dpint)p7;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
  t += (dpint)v14 * (dpint)p4;
  t += (dpint)v15 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 28;
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
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 26u);
  s -= v11;
  t += (dpint)v12 * (dpint)p7;
  t += (dpint)v13 * (dpint)p6;
  t += (dpint)v14 * (dpint)p5;
  t += (dpint)v15 * (dpint)p4;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 28;
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
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v5 << 26u);
  s -= v12;
  t += (dpint)v13 * (dpint)p7;
  t += (dpint)v14 * (dpint)p6;
  t += (dpint)v15 * (dpint)p5;
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 28;
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
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v6 << 26u);
  s -= v13;
  t += (dpint)v14 * (dpint)p7;
  t += (dpint)v15 * (dpint)p6;
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[15];
  t += (dpint)a[8] * b[14];
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)a[14] * b[8];
  t += (dpint)a[15] * b[7];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v7 << 26u);
  s -= v14;
  t += (dpint)v15 * (dpint)p7;
  t += (dpint)s;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[15];
  t += (dpint)a[9] * b[14];
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)a[14] * b[9];
  t += (dpint)a[15] * b[8];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v8 << 26u);
  s -= v15;
  t += (dpint)s;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[15];
  t += (dpint)a[10] * b[14];
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)a[14] * b[10];
  t += (dpint)a[15] * b[9];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v9 << 26u);
  t += (dpint)s;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[15];
  t += (dpint)a[11] * b[14];
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)a[14] * b[11];
  t += (dpint)a[15] * b[10];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v10 << 26u);
  t += (dpint)s;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[15];
  t += (dpint)a[12] * b[14];
  t += (dpint)a[13] * b[13];
  t += (dpint)a[14] * b[12];
  t += (dpint)a[15] * b[11];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v11 << 26u);
  t += (dpint)s;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[15];
  t += (dpint)a[13] * b[14];
  t += (dpint)a[14] * b[13];
  t += (dpint)a[15] * b[12];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v12 << 26u);
  t += (dpint)s;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[15];
  t += (dpint)a[14] * b[14];
  t += (dpint)a[15] * b[13];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v13 << 26u);
  t += (dpint)s;
  c[12] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[14] * b[15];
  t += (dpint)a[15] * b[14];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v14 << 26u);
  t += (dpint)s;
  c[13] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[15] * b[15];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v15 << 26u);
  t += (dpint)s;
  c[14] = ((spint)t & mask);
  t >>= 28;
  t -= (dpint)1u;
  c[15] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0xb5844f3u;
  spint p1 = 0x78c292au;
  spint p2 = 0x58f5523u;
  spint p3 = 0xc2728dcu;
  spint p4 = 0x690216cu;
  spint p5 = 0x49aed63u;
  spint p6 = 0x9c44edbu;
  spint p7 = 0x7cca23eu;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xe918bc5u;
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
  t += (udpint)(spint)(q - v0);
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
  spint s = (spint)mask;
  s -= v1;
  t += (udpint)v2 * p7;
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
  s -= v2;
  t += (udpint)v3 * p7;
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
  s -= v3;
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
  s -= v4;
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
  s -= v5;
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
  s -= v6;
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
  t += (udpint)v0 << 26u;
  s -= v7;
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
  tot = (udpint)a[1] * a[15];
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
  t += (udpint)v1 << 26u;
  s -= v8;
  t += (udpint)v9 * p7;
  t += (udpint)v10 * p6;
  t += (udpint)v11 * p5;
  t += (udpint)v12 * p4;
  t += (udpint)v13 * p3;
  t += (udpint)v14 * p2;
  t += (udpint)v15 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[15];
  tot += (udpint)a[3] * a[14];
  tot += (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 << 26u;
  s -= v9;
  t += (udpint)v10 * p7;
  t += (udpint)v11 * p6;
  t += (udpint)v12 * p5;
  t += (udpint)v13 * p4;
  t += (udpint)v14 * p3;
  t += (udpint)v15 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[15];
  tot += (udpint)a[4] * a[14];
  tot += (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 << 26u;
  s -= v10;
  t += (udpint)v11 * p7;
  t += (udpint)v12 * p6;
  t += (udpint)v13 * p5;
  t += (udpint)v14 * p4;
  t += (udpint)v15 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[15];
  tot += (udpint)a[5] * a[14];
  tot += (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 26u;
  s -= v11;
  t += (udpint)v12 * p7;
  t += (udpint)v13 * p6;
  t += (udpint)v14 * p5;
  t += (udpint)v15 * p4;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[15];
  tot += (udpint)a[6] * a[14];
  tot += (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 << 26u;
  s -= v12;
  t += (udpint)v13 * p7;
  t += (udpint)v14 * p6;
  t += (udpint)v15 * p5;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[15];
  tot += (udpint)a[7] * a[14];
  tot += (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 << 26u;
  s -= v13;
  t += (udpint)v14 * p7;
  t += (udpint)v15 * p6;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[15];
  tot += (udpint)a[8] * a[14];
  tot += (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  s = (spint)mask;
  t += (udpint)v7 << 26u;
  s -= v14;
  t += (udpint)v15 * p7;
  t += (udpint)s;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[15];
  tot += (udpint)a[9] * a[14];
  tot += (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v8 << 26u;
  s -= v15;
  t += (udpint)s;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[15];
  tot += (udpint)a[10] * a[14];
  tot += (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  s = (spint)mask;
  t += (udpint)v9 << 26u;
  t += (udpint)s;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[15];
  tot += (udpint)a[11] * a[14];
  tot += (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v10 << 26u;
  t += (udpint)s;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[15];
  tot += (udpint)a[12] * a[14];
  tot *= 2;
  tot += (udpint)a[13] * a[13];
  t += tot;
  s = (spint)mask;
  t += (udpint)v11 << 26u;
  t += (udpint)s;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[15];
  tot += (udpint)a[13] * a[14];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v12 << 26u;
  t += (udpint)s;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[15];
  tot *= 2;
  tot += (udpint)a[14] * a[14];
  t += tot;
  s = (spint)mask;
  t += (udpint)v13 << 26u;
  t += (udpint)s;
  c[12] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[14] * a[15];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v14 << 26u;
  t += (udpint)s;
  c[13] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[15] * a[15];
  t += tot;
  s = (spint)mask;
  t += (udpint)v15 << 26u;
  t += (udpint)s;
  c[14] = ((spint)t & mask);
  t >>= 28;
  t -= 1u;
  c[15] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 16; i++) {
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
  spint x[16];
  spint t0[16];
  spint t1[16];
  spint t2[16];
  spint t3[16];
  spint t4[16];
  spint t5[16];
  spint t6[16];
  spint t7[16];
  spint t8[16];
  spint t9[16];
  spint t10[16];
  spint t11[16];
  spint t12[16];
  spint t13[16];
  spint t14[16];
  spint t15[16];
  modcpy(w, x);
  modsqr(x, t12);
  modmul(x, t12, t1);
  modmul(x, t1, z);
  modmul(x, z, t5);
  modmul(z, t5, t4);
  modmul(t12, t4, t7);
  modmul(t12, t7, t2);
  modmul(t12, t2, z);
  modmul(t12, z, t0);
  modmul(t12, t0, t9);
  modmul(t12, t9, t3);
  modmul(t12, t3, t8);
  modmul(t12, t8, t10);
  modmul(t12, t10, t6);
  modmul(t12, t6, t11);
  modmul(t12, t11, t12);
  modsqr(t12, t14);
  modsqr(t14, t13);
  modcpy(t13, t15);
  modnsqr(t15, 5);
  modmul(t13, t15, t13);
  modcpy(t13, t15);
  modnsqr(t15, 4);
  modmul(t14, t15, t15);
  modnsqr(t15, 11);
  modmul(t13, t15, t13);
  modcpy(t13, t15);
  modnsqr(t15, 4);
  modmul(t14, t15, t14);
  modnsqr(t14, 26);
  modmul(t13, t14, t13);
  modcpy(t13, t14);
  modnsqr(t14, 55);
  modmul(t13, t14, t13);
  modcpy(t13, t14);
  modnsqr(t14, 110);
  modmul(t13, t14, t13);
  modmul(t1, t13, t13);
  modnsqr(t13, 6);
  modmul(t12, t13, t13);
  modnsqr(t13, 7);
  modmul(t10, t13, t13);
  modnsqr(t13, 6);
  modmul(t0, t13, t13);
  modnsqr(t13, 8);
  modmul(t12, t13, t12);
  modnsqr(t12, 6);
  modmul(t9, t12, t12);
  modnsqr(t12, 5);
  modmul(t0, t12, t12);
  modnsqr(t12, 8);
  modmul(t9, t12, t12);
  modnsqr(t12, 4);
  modmul(t7, t12, t12);
  modnsqr(t12, 6);
  modmul(t6, t12, t12);
  modnsqr(t12, 5);
  modmul(t4, t12, t12);
  modnsqr(t12, 6);
  modmul(t2, t12, t12);
  modnsqr(t12, 6);
  modmul(t11, t12, t11);
  modnsqr(t11, 5);
  modmul(t3, t11, t11);
  modnsqr(t11, 5);
  modmul(t0, t11, t11);
  modnsqr(t11, 4);
  modmul(t7, t11, t11);
  modnsqr(t11, 5);
  modmul(t4, t11, t11);
  modnsqr(t11, 7);
  modmul(x, t11, t11);
  modnsqr(t11, 8);
  modmul(t7, t11, t11);
  modnsqr(t11, 6);
  modmul(t10, t11, t10);
  modsqr(t10, t10);
  modmul(x, t10, t10);
  modnsqr(t10, 9);
  modmul(t9, t10, t9);
  modnsqr(t9, 4);
  modmul(t4, t9, t9);
  modnsqr(t9, 6);
  modmul(t0, t9, t9);
  modnsqr(t9, 5);
  modmul(t8, t9, t8);
  modnsqr(t8, 7);
  modmul(t7, t8, t7);
  modnsqr(t7, 7);
  modmul(z, t7, t7);
  modnsqr(t7, 6);
  modmul(t3, t7, t7);
  modnsqr(t7, 5);
  modmul(t4, t7, t7);
  modnsqr(t7, 8);
  modmul(t6, t7, t6);
  modnsqr(t6, 2);
  modmul(t1, t6, t6);
  modnsqr(t6, 5);
  modmul(t1, t6, t6);
  modnsqr(t6, 7);
  modmul(t5, t6, t5);
  modnsqr(t5, 6);
  modmul(t4, t5, t4);
  modnsqr(t4, 6);
  modmul(t3, t4, t3);
  modnsqr(t3, 5);
  modmul(t2, t3, t2);
  modnsqr(t2, 3);
  modmul(t1, t2, t1);
  modnsqr(t1, 9);
  modmul(t0, t1, t0);
  modnsqr(t0, 6);
  modmul(z, t0, z);
  modnsqr(z, 2);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[16];
  spint t[16];
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
  const spint c[16] = {0x49b9b60u, 0x5392570u, 0x195d9e3u, 0x2c4bc1bu,
                       0x8597af3u, 0x2388ea1u, 0x80d66deu, 0x5ee4d83u,
                       0xe17cf72u, 0xc47c44au, 0xcc14ba3u, 0x70af1a9u,
                       0xcb7e4d0u, 0x292052bu, 0x9f823b7u, 0x3402a93u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[16];
  c[0] = 1;
  for (i = 1; i < 16; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[16];
  for (i = 0; i < 16; i++) {
    b[i] = n[i];
    t[i] = n[i + 16];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[16];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 16; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[16];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 16; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 16; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 16; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 16; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[16];
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
static void modcmv(int d, const spint *g, spint *f) {
  int i;
  spint t;
  spint r0 = f[0] ^ g[1];
  spint r1 = f[1] ^ g[0];
  spint c0 = (1 - (d - r0));
  spint c1 = d+r1;
  for (i = 0; i < 16; i++) {
      t=f[i];
      f[i] =c0*t+c1*g[i];
      f[i]-=r0*t+r1*g[i]; 
  }
}

// conditional swap g and f if d=1
static void modcsw(int d, spint *g, spint *f) {
  int i;
  spint t, s;
  spint r0 = f[0] ^ g[1];
  spint r1 = f[1] ^ g[0];
  spint c0 = (1 - (d - r0));
  spint c1 = d+r1;
  for (i = 0; i < 16; i++) {
      t=f[i]; s=g[i];
      f[i] =c0*t+c1*s;
      g[i] =c0*s+c1*t;
      f[i]-=r0*t+r1*s;
      g[i]-=r0*s+r1*t;
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  spint s[16];
  spint y[16];
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
  a[15] = ((a[15] << n)) | (a[14] >> (28u - n));
  for (i = 14; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 15; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[15] = a[15] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 28u;
  unsigned int m = r % 28u;
  modzer(a);
  if (r >= 56 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
  nres(a, a);
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[16];
  redc(a, c);
  for (i = 55; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 16; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 56; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[16];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[16], d[16];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 16; i++) {
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
    toHex(56,b,buff);
    puts(buff);
}

// output a point (x,y)
void outputxy(point *P)
{
    if (ecn448isinf(P)) {
        printf("P= O\n");
    } else {
        char x[BYTES],y[BYTES];
        char buff[(2*BYTES)+1];
        ecn448get(P,x,y);
        toHex(BYTES,x,buff);
        printf("Px= "); puts(buff);
        toHex(BYTES,y,buff);
        printf("Py= "); puts(buff);
    }
}
*/

// reduce 114 byte array h to integer r modulo group order q, in constant time
// Consider h as 2^440.(2^440x + y)  + z, where x,y and z < q (z is bottom 55 bytes, y is next 55 bytes, x is top 4 bytes)
// Important that x,y and z < q, 55 bytes = 440 bits, q is 446 bits
static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,z,c;

    mod2r(440,c);
   
    for (i=0;i<55;i++)  // bottom 55 bytes
        buff[i]=h[i];
    buff[55]=0;
    reverse(buff);
    modimp(buff,z);  
    
    for (i=0;i<55;i++)  // middle 55 bytes
        buff[i]=h[i+55];
    buff[55]=0;
    reverse(buff);
    modimp(buff,y);  
    
    for (i=0;i<4;i++)  // top 4 bytes
       buff[i]=h[110+i];   
    for (i=4;i<56;i++)
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

// Input private key - 57 random bytes
// Output public key - 57 bytes
void ED448_KEY_PAIR(char *prv,char *pub)
{
    int sign;
    point P;
    ecn448gen(&P);
    char s[BYTES];

    H(BYTES+1,BYTES,prv,s);
// clamp s
    s[0]&=0xFC;
    s[55]|=0x80;

    reverse(s);  // little endian to big endian
    ecn448mul(s,&P); 

    sign=ecn448get(&P,NULL,pub);  // get y coordinate and sign
    reverse(pub);              // big endian to little endian
    pub[56]=(char)(sign<<7);
}

const char dom4[10]={'S','i','g','E','d','4','4','8',0,0};

// input private key, public key, message to be signed. Output signature
void ED448_SIGN(char *prv,char *pub,int mlen,char *m,char *sig)
{
    int i,sign;
    char h[2*BYTES+2];
    char ipub[BYTES+1];
    gel r,s,d;
    sha3 SHA3;
    point R;
    ecn448gen(&R);

    if (pub!=NULL)
    {
        for (i=0;i<BYTES+1;i++)
            ipub[i]=pub[i];
    } else {
        ED448_KEY_PAIR(prv,ipub);
    }

    SHA3_init(&SHA3,SHAKE256);
    H(BYTES+1,2*BYTES+2,prv,h);

// derive and clamp s
    h[0]&=0xFC;
    h[55]|=0x80;
    reverse(h); 
    modimp(h,s);

    for (i=0;i<10;i++)
        SHA3_process(&SHA3,dom4[i]);
    for (i=BYTES+1;i<2*BYTES+2;i++ )
        SHA3_process(&SHA3,h[i]);
    for (i=0;i<mlen;i++)
        SHA3_process(&SHA3,m[i]);
    SHA3_shake(&SHA3,h,2*BYTES+2); 

    reduce(h,r);
    modexp(r,h);  // convert to big endian array
    ecn448mul(h,&R);

    sign=ecn448get(&R,NULL,sig);  // get y coordinate and sign
    reverse(sig);              // big endian to little endian
    sig[BYTES]=(char)(sign<<7); // first part of signature

    SHA3_init(&SHA3,SHAKE256);
    for (i=0;i<10;i++)
        SHA3_process(&SHA3,dom4[i]);
    for (i=0;i<BYTES+1;i++ )
        SHA3_process(&SHA3,sig[i]);  // R
    for (i=0;i<BYTES+1;i++)
        SHA3_process(&SHA3,ipub[i]);  // Q
    for (i=0;i<mlen;i++)
        SHA3_process(&SHA3,m[i]);   // M
    SHA3_shake(&SHA3,h,2*BYTES+2); 

    reduce(h,d);
    modmul(d,s,d);
    modadd(d,r,d);

    modexp(d,&sig[BYTES+1]);
    reverse(&sig[BYTES+1]);
    sig[2*BYTES+1]=0;           // second part of signature
}

int ED448_VERIFY(char *pub,int mlen,char *m,char *sig) 
{
    int i;
    char buff[BYTES];
    point G,R,Q;
    gel u;
    int sign;
    sha3 SHA3;
    char h[2*BYTES+2];

    ecn448gen(&G);

// reconstruct point R
    for (i=0;i<BYTES;i++)
        buff[i]=sig[i];
    reverse(buff);
    sign=sig[BYTES]>>7;
    ecn448set(sign,NULL,buff,&R);
    if (ecn448isinf(&R)) return 0;

// reconstruct point Q 
    for (i=0;i<BYTES;i++)
        buff[i]=pub[i];
    reverse(buff);
    sign=(pub[BYTES]>>7)&1;
    ecn448set(sign,NULL,buff,&Q);
    if (ecn448isinf(&Q)) return 0;

    for (i=0;i<BYTES;i++)
        buff[i]=sig[i+BYTES+1];
    reverse(buff);

    SHA3_init(&SHA3,SHAKE256);
    for (i=0;i<10;i++)
        SHA3_process(&SHA3,dom4[i]);
    for (i=0;i<BYTES+1;i++ )
        SHA3_process(&SHA3,sig[i]);  // R
    for (int i=0;i<BYTES+1;i++)
        SHA3_process(&SHA3,pub[i]);  // Q
    for (i=0;i<mlen;i++)
        SHA3_process(&SHA3,m[i]);   // M
    SHA3_shake(&SHA3,h,2*BYTES+2); 

    reduce(h,u); modneg(u,u); modexp(u,h);

    if (!modimp(buff,u)) return 0;  // out of range

    ecn448cof(&G); ecn448cof(&R); ecn448cof(&Q);
    ecn448mul2(buff,&G,h,&Q,&Q);

    if (ecn448cmp(&R,&Q))
        return 1;
    return 0;
}


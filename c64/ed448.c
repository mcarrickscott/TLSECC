// Ed448 Implementation
// see https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-5.pdf
// and RFC8032
// python curve.py 64 ED448
// This completes edwards.c for this curve. Then
// gcc -O2 Ed448.c edwards.c hash.c -o Ed448

#include <stdio.h>
#include <stdint.h>
#include "hash.h"  // Some useful hash functions

#include "Ed448curve.h"   // elliptic curve API

/*** Insert code automatically generated from group.c here ***/
/* Note that much of this code is not needed and can be deleted */


// Command line : python monty.py 64 ed448

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 8
#define Radix 56
#define Nbits 446
#define Nbytes 56

#define MONTGOMERY
#define ED448

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 56u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 56u;
  n[0] &= mask;
  for (i = 1; i < 7; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 56u;
  }
  n[7] += (spint)carry;
  return -((n[7] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x78c292ab5844f3u) & carry;
  n[1] += ((spint)0xc2728dc58f5523u) & carry;
  n[2] += ((spint)0x49aed63690216cu) & carry;
  n[3] += ((spint)0x7cca23e9c44edbu) & carry;
  n[4] -= (spint)1u & carry;
  n[7] += ((spint)0x40000000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] -= (spint)0x78c292ab5844f3u;
  n[1] -= (spint)0xc2728dc58f5523u;
  n[2] -= (spint)0x49aed63690216cu;
  n[3] -= (spint)0x7cca23e9c44edbu;
  n[4] += (spint)1u;
  n[7] -= (spint)0x40000000000000u;
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
  n[0] -= (spint)0xf1852556b089e6u;
  n[1] -= (spint)0x184e51b8b1eaa46u;
  n[2] -= (spint)0x935dac6d2042d8u;
  n[3] -= (spint)0xf99447d3889db6u;
  n[4] += (spint)2u;
  n[7] -= (spint)0x80000000000000u;
  carry = prop(n);
  n[0] += ((spint)0xf1852556b089e6u) & carry;
  n[1] += ((spint)0x184e51b8b1eaa46u) & carry;
  n[2] += ((spint)0x935dac6d2042d8u) & carry;
  n[3] += ((spint)0xf99447d3889db6u) & carry;
  n[4] -= (spint)2u & carry;
  n[7] += ((spint)0x80000000000000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0xf1852556b089e6u) & carry;
  n[1] += ((spint)0x184e51b8b1eaa46u) & carry;
  n[2] += ((spint)0x935dac6d2042d8u) & carry;
  n[3] += ((spint)0xf99447d3889db6u) & carry;
  n[4] -= (spint)2u & carry;
  n[7] += ((spint)0x80000000000000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0xf1852556b089e6u) & carry;
  n[1] += ((spint)0x184e51b8b1eaa46u) & carry;
  n[2] += ((spint)0x935dac6d2042d8u) & carry;
  n[3] += ((spint)0xf99447d3889db6u) & carry;
  n[4] -= (spint)2u & carry;
  n[7] += ((spint)0x80000000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 53255119132055870337481458170721706
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0x78c292ab5844f3u;
  spint p1 = 0xc2728dc58f5523u;
  spint p2 = 0x49aed63690216cu;
  spint p3 = 0x7cca23e9c44edbu;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xbd440fae918bc5u;
  t += (dpint)a[0] * b[0];
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)v0 * (dpint)p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  t += (dpint)v1 * (dpint)p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v0 * (dpint)p3;
  t += (dpint)v1 * (dpint)p2;
  t += (dpint)v2 * (dpint)p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)(spint)(q - v0);
  t += (dpint)v1 * (dpint)p3;
  t += (dpint)v2 * (dpint)p2;
  t += (dpint)v3 * (dpint)p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (dpint)v4 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  spint s = (spint)mask;
  s -= v1;
  t += (dpint)v2 * (dpint)p3;
  t += (dpint)v3 * (dpint)p2;
  t += (dpint)v4 * (dpint)p1;
  t += (dpint)s;
  spint v5 = (((spint)t * ndash) & mask);
  t += (dpint)v5 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  s = (spint)mask;
  s -= v2;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  t += (dpint)v5 * (dpint)p1;
  t += (dpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (dpint)v6 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[7];
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)a[7] * b[0];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v0 << 54u);
  s -= v3;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  t += (dpint)v6 * (dpint)p1;
  t += (dpint)s;
  spint v7 = (((spint)t * ndash) & mask);
  t += (dpint)v7 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[1] * b[7];
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  t += (dpint)a[7] * b[1];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v1 << 54u);
  s -= v4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  t += (dpint)v7 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v2 << 54u);
  s -= v5;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)v7 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v3 << 54u);
  s -= v6;
  t += (dpint)v7 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 54u);
  s -= v7;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v5 << 54u);
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v6 << 54u);
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[7] * b[7];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v7 << 54u);
  t += (dpint)s;
  c[6] = ((spint)t & mask);
  t >>= 56;
  t -= (dpint)1u;
  c[7] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0x78c292ab5844f3u;
  spint p1 = 0xc2728dc58f5523u;
  spint p2 = 0x49aed63690216cu;
  spint p3 = 0x7cca23e9c44edbu;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xbd440fae918bc5u;
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (udpint)v0 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (udpint)v1 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  t += (udpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (udpint)v2 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p3;
  t += (udpint)v1 * p2;
  t += (udpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (udpint)v3 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)(spint)(q - v0);
  t += (udpint)v1 * p3;
  t += (udpint)v2 * p2;
  t += (udpint)v3 * p1;
  spint v4 = (((spint)t * ndash) & mask);
  t += (udpint)v4 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  spint s = (spint)mask;
  s -= v1;
  t += (udpint)v2 * p3;
  t += (udpint)v3 * p2;
  t += (udpint)v4 * p1;
  t += (udpint)s;
  spint v5 = (((spint)t * ndash) & mask);
  t += (udpint)v5 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  s = (spint)mask;
  s -= v2;
  t += (udpint)v3 * p3;
  t += (udpint)v4 * p2;
  t += (udpint)v5 * p1;
  t += (udpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (udpint)v6 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v0 << 54u;
  s -= v3;
  t += (udpint)v4 * p3;
  t += (udpint)v5 * p2;
  t += (udpint)v6 * p1;
  t += (udpint)s;
  spint v7 = (((spint)t * ndash) & mask);
  t += (udpint)v7 * p0;
  t >>= 56;
  tot = (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  s = (spint)mask;
  t += (udpint)v1 << 54u;
  s -= v4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  t += (udpint)v7 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 << 54u;
  s -= v5;
  t += (udpint)v6 * p3;
  t += (udpint)v7 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 << 54u;
  s -= v6;
  t += (udpint)v7 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 54u;
  s -= v7;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 << 54u;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 << 54u;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[7] * a[7];
  t += tot;
  s = (spint)mask;
  t += (udpint)v7 << 54u;
  t += (udpint)s;
  c[6] = ((spint)t & mask);
  t >>= 56;
  t -= 1u;
  c[7] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 8; i++) {
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
  spint x[8];
  spint t0[8];
  spint t1[8];
  spint t2[8];
  spint t3[8];
  spint t4[8];
  spint t5[8];
  spint t6[8];
  spint t7[8];
  spint t8[8];
  spint t9[8];
  spint t10[8];
  spint t11[8];
  spint t12[8];
  spint t13[8];
  spint t14[8];
  spint t15[8];
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
  spint s[8];
  spint t[8];
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
  const spint c[8] = {0x539257049b9b60u, 0x2c4bc1b195d9e3u, 0x2388ea18597af3u,
                      0x5ee4d8380d66deu, 0xc47c44ae17cf72u, 0x70af1a9cc14ba3u,
                      0x292052bcb7e4d0u, 0x3402a939f823b7u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[8];
  c[0] = 1;
  for (i = 1; i < 8; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[8];
  for (i = 0; i < 8; i++) {
    b[i] = n[i];
    t[i] = n[i + 8];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[8];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 8; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 56u) &
          (((c0 ^ (spint)1) - (spint)1) >> 56u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[8];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 8; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 56u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 8; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 8; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 8; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[8];
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
static void modcmv(int d, const spint *g, volatile spint *f) {
  int i;
  spint c0,c1,s,t,r,aux;
  static spint R=0;
  R+=0x3cc3c33c5aa5a55a;
  r=R;
  c0=(~d)&(r+1);
  c1=d+r;
  for (i = 0; i < 8; i++) {
      s=g[i]; t=f[i];
      f[i] = aux = c0*t+c1*s;
      f[i] = aux-r*(t+s); 
  }
}

// conditional swap g and f if d=1
static void modcsw(int d, volatile spint *g, volatile spint *f) {
  int i;
  spint c0,c1,s,t,r,w,aux;
  static spint R=0;
  R+=0x3cc3c33c5aa5a55a;
  r=R;
  c0=(~d)&(r+1);
  c1=d+r;
  for (i = 0; i < 8; i++) {
      s=g[i]; t=f[i];
      w=r*(t+s);
      f[i] = aux = c0*t+c1*s;
      f[i] = aux-w;  
      g[i] = aux = c0*s+c1*t;
      g[i] = aux-w;   
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  spint s[8];
  spint y[8];
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
  a[7] = ((a[7] << n)) | (a[6] >> (56u - n));
  for (i = 6; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xffffffffffffff) | (a[i - 1] >> (56u - n));
  }
  a[0] = (a[0] << n) & (spint)0xffffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 7; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (56u - n)) & (spint)0xffffffffffffff);
  }
  a[7] = a[7] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 56u;
  unsigned int m = r % 56u;
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
  spint c[8];
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
  for (i = 0; i < 8; i++) {
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
  spint c[8];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[8], d[8];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 8; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 56) & 1;
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


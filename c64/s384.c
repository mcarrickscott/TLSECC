
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


// Command line : python monty.py 64 nist384

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 7
#define Radix 56
#define Nbits 384
#define Nbytes 48

#define MONTGOMERY
#define NIST384

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 56u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 56u;
  n[0] &= mask;
  for (i = 1; i < 6; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 56u;
  }
  n[6] += (spint)carry;
  return -((n[6] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0xec196accc52973u) & carry;
  n[1] += ((spint)0xdb248b0a77aecu) & carry;
  n[2] += ((spint)0x81f4372ddf581au) & carry;
  n[3] += ((spint)0xffffffffc7634du) & carry;
  n[4] -= (spint)1u & carry;
  n[6] += ((spint)0x1000000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] -= (spint)0xec196accc52973u;
  n[1] -= (spint)0xdb248b0a77aecu;
  n[2] -= (spint)0x81f4372ddf581au;
  n[3] -= (spint)0xffffffffc7634du;
  n[4] += (spint)1u;
  n[6] -= (spint)0x1000000000000u;
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
  n[0] -= (spint)0x1d832d5998a52e6u;
  n[1] -= (spint)0x1b6491614ef5d8u;
  n[2] -= (spint)0x103e86e5bbeb034u;
  n[3] -= (spint)0x1ffffffff8ec69au;
  n[4] += (spint)2u;
  n[6] -= (spint)0x2000000000000u;
  carry = prop(n);
  n[0] += ((spint)0x1d832d5998a52e6u) & carry;
  n[1] += ((spint)0x1b6491614ef5d8u) & carry;
  n[2] += ((spint)0x103e86e5bbeb034u) & carry;
  n[3] += ((spint)0x1ffffffff8ec69au) & carry;
  n[4] -= (spint)2u & carry;
  n[6] += ((spint)0x2000000000000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0x1d832d5998a52e6u) & carry;
  n[1] += ((spint)0x1b6491614ef5d8u) & carry;
  n[2] += ((spint)0x103e86e5bbeb034u) & carry;
  n[3] += ((spint)0x1ffffffff8ec69au) & carry;
  n[4] -= (spint)2u & carry;
  n[6] += ((spint)0x2000000000000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0x1d832d5998a52e6u) & carry;
  n[1] += ((spint)0x1b6491614ef5d8u) & carry;
  n[2] += ((spint)0x103e86e5bbeb034u) & carry;
  n[3] += ((spint)0x1ffffffff8ec69au) & carry;
  n[4] -= (spint)2u & carry;
  n[6] += ((spint)0x2000000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 49260895723021306548442156017426496
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0xec196accc52973u;
  spint p1 = 0xdb248b0a77aecu;
  spint p2 = 0x81f4372ddf581au;
  spint p3 = 0xffffffffc7634du;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xd46089e88fdc45u;
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
  t += (dpint)(udpint)((udpint)v0 << 48u);
  s -= v2;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  t += (dpint)v5 * (dpint)p1;
  t += (dpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (dpint)v6 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v1 << 48u);
  s -= v3;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  t += (dpint)v6 * (dpint)p1;
  t += (dpint)s;
  c[0] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v2 << 48u);
  s -= v4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  t += (dpint)s;
  c[1] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v3 << 48u);
  s -= v5;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 48u);
  s -= v6;
  t += (dpint)s;
  c[3] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v5 << 48u);
  t += (dpint)s;
  c[4] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[6] * b[6];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v6 << 48u);
  t += (dpint)s;
  c[5] = ((spint)t & mask);
  t >>= 56;
  t -= (dpint)1u;
  c[6] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0xec196accc52973u;
  spint p1 = 0xdb248b0a77aecu;
  spint p2 = 0x81f4372ddf581au;
  spint p3 = 0xffffffffc7634du;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xd46089e88fdc45u;
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
  t += (udpint)v0 << 48u;
  s -= v2;
  t += (udpint)v3 * p3;
  t += (udpint)v4 * p2;
  t += (udpint)v5 * p1;
  t += (udpint)s;
  spint v6 = (((spint)t * ndash) & mask);
  t += (udpint)v6 * p0;
  t >>= 56;
  tot = (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v1 << 48u;
  s -= v3;
  t += (udpint)v4 * p3;
  t += (udpint)v5 * p2;
  t += (udpint)v6 * p1;
  t += (udpint)s;
  c[0] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  s = (spint)mask;
  t += (udpint)v2 << 48u;
  s -= v4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  t += (udpint)s;
  c[1] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v3 << 48u;
  s -= v5;
  t += (udpint)v6 * p3;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 48u;
  s -= v6;
  t += (udpint)s;
  c[3] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  t += (udpint)v5 << 48u;
  t += (udpint)s;
  c[4] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[6] * a[6];
  t += tot;
  s = (spint)mask;
  t += (udpint)v6 << 48u;
  t += (udpint)s;
  c[5] = ((spint)t & mask);
  t >>= 56;
  t -= 1u;
  c[6] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 7; i++) {
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
  spint x[7];
  spint t0[7];
  spint t1[7];
  spint t2[7];
  spint t3[7];
  spint t4[7];
  spint t5[7];
  spint t6[7];
  spint t7[7];
  spint t8[7];
  spint t9[7];
  spint t10[7];
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
  spint s[7];
  spint t[7];
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
  const spint c[7] = {0x35fd09360e3cb4u, 0x73c4e1290eb460u, 0x43a6addff2dbd2u,
                      0x1cc5bf02d164eeu, 0x95d40d49174aabu, 0x3fb05b7a282668u,
                      0xee012b39bf21u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[7];
  c[0] = 1;
  for (i = 1; i < 7; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[7];
  for (i = 0; i < 7; i++) {
    b[i] = n[i];
    t[i] = n[i + 7];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[7];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 7; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 56u) &
          (((c0 ^ (spint)1) - (spint)1) >> 56u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[7];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 7; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 56u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 7; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 7; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 7; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[7];
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
  for (i = 0; i < 7; i++) {
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
  for (i = 0; i < 7; i++) {
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
  spint s[7];
  spint y[7];
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
  a[6] = ((a[6] << n)) | (a[5] >> (56u - n));
  for (i = 5; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xffffffffffffff) | (a[i - 1] >> (56u - n));
  }
  a[0] = (a[0] << n) & (spint)0xffffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 6; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (56u - n)) & (spint)0xffffffffffffff);
  }
  a[6] = a[6] >> n;
  return r;
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[7];
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
  for (i = 0; i < 7; i++) {
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
  spint c[7];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[7], d[7];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 7; i++) {
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
// precalculate c=nres(2^376 mod q) - see ec384_order.py
#if Wordlength==64
static const spint constant_c[7]={0xab3b9af6e24bde,0x970d17e866e2cd,0xf8ce7f1c6d4b85,0x174aab1ca68def,0x28266895d40d49,0x39bf213fb05b7a,0x8d0c84ee012b};
#endif

#if Wordlength==32
static const spint constant_c[14]={0x6e24bde,0xab3b9af,0x866e2cd,0x970d17e,0xc6d4b85,0xf8ce7f1,0xca68def,0x174aab1,0x5d40d49,0x2826689,0xfb05b7a,0x39bf213,0x4ee012b,0x8d0c8};
#endif

static void reduce(char *h,spint *r)
{
    int i;
    char buff[BYTES];
    gel x,y,z;

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

    modmul(x,constant_c,x);  // 2^376.x 
    modadd(x,y,r);
}

//#define PREHASHED   // define for test vectors

// API

#include "tlsecc.h"

// Input private key - 48 random bytes
// Output public key - 97 bytes (0x04<x>|<y>), or 49 if compressed (0x02<x>.. or 0x03<x>)
void NIST384_KEY_GEN(int compress,char *prv,char *pub)
{
    point P;
    ecn384gen(&P);

    ecn384mul(prv,&P); 

    if (compress) {
        pub[0]=0x02+ecn384get(&P,&pub[1],NULL); // 0x02 or 0x03
    } else {
        pub[0]=0x04; // no compression
        ecn384get(&P,&pub[1],&pub[BYTES+1]);  // get x and y
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
    int i,res;
    char rb[BYTES],u[BYTES],v[BYTES];
    gel e,r,s;
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
    modmul(r,s,r); modexp(r,v);  // export to byte array
    modmul(s,e,s); modexp(s,u); 

    if (pub[0]==0x04) {
        ecn384set(0,&pub[1],&pub[BYTES+1],&Q);
    } else {
        ecn384set((int)pub[0]&1,&pub[1],NULL,&Q);
    }

    ecn384mul2(u,&G,v,&Q,&Q);
    if (ecn384isinf(&Q)) return 0;

    ecn384get(&Q,rb,NULL);

    res=1;
    for (i=0;i<BYTES;i++) {
        if (sig[i]!=rb[i]) res=0;
    }
    
    return res;
}

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
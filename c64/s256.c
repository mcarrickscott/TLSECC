
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


// Command line : python monty.py 64 nist256

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 5
#define Radix 52
#define Nbits 256
#define Nbytes 32

#define MONTGOMERY
#define NIST256

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 52u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 52u;
  n[0] &= mask;
  for (i = 1; i < 4; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 52u;
  }
  n[4] += (spint)carry;
  return -((n[4] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x9cac2fc632551u) & carry;
  n[1] += ((spint)0xada7179e84f3bu) & carry;
  n[2] += ((spint)0xfffffffbce6fau) & carry;
  n[3] += ((spint)0xfffffffffu) & carry;
  n[4] += ((spint)0xffffffff0000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] -= (spint)0x9cac2fc632551u;
  n[1] -= (spint)0xada7179e84f3bu;
  n[2] -= (spint)0xfffffffbce6fau;
  n[3] -= (spint)0xfffffffffu;
  n[4] -= (spint)0xffffffff0000u;
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
  n[0] -= (spint)0x139585f8c64aa2u;
  n[1] -= (spint)0x15b4e2f3d09e76u;
  n[2] -= (spint)0x1fffffff79cdf4u;
  n[3] -= (spint)0x1ffffffffeu;
  n[4] -= (spint)0x1fffffffe0000u;
  carry = prop(n);
  n[0] += ((spint)0x139585f8c64aa2u) & carry;
  n[1] += ((spint)0x15b4e2f3d09e76u) & carry;
  n[2] += ((spint)0x1fffffff79cdf4u) & carry;
  n[3] += ((spint)0x1ffffffffeu) & carry;
  n[4] += ((spint)0x1fffffffe0000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0x139585f8c64aa2u) & carry;
  n[1] += ((spint)0x15b4e2f3d09e76u) & carry;
  n[2] += ((spint)0x1fffffff79cdf4u) & carry;
  n[3] += ((spint)0x1ffffffffeu) & carry;
  n[4] += ((spint)0x1fffffffe0000u) & carry;
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
  carry = prop(n);
  n[0] += ((spint)0x139585f8c64aa2u) & carry;
  n[1] += ((spint)0x15b4e2f3d09e76u) & carry;
  n[2] += ((spint)0x1fffffff79cdf4u) & carry;
  n[3] += ((spint)0x1ffffffffeu) & carry;
  n[4] += ((spint)0x1fffffffe0000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 149133484957480209596535528793216
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0x9cac2fc632551u;
  spint p1 = 0xada7179e84f3bu;
  spint p2 = 0xfffffffbce6fau;
  spint p3 = 0xfffffffffu;
  spint p4 = 0xffffffff0000u;
  spint q = ((spint)1 << 52u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x1c8aaee00bc4fu;
  t += (dpint)a[0] * b[0];
  spint v0 = (((spint)t * ndash) & mask);
  t += (dpint)v0 * (dpint)p0;
  t >>= 52;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)v0 * (dpint)p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (dpint)v1 * (dpint)p0;
  t >>= 52;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  t += (dpint)v1 * (dpint)p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (dpint)v2 * (dpint)p0;
  t >>= 52;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v0 * (dpint)p3;
  t += (dpint)v1 * (dpint)p2;
  t += (dpint)v2 * (dpint)p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (dpint)v3 * (dpint)p0;
  t >>= 52;
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
  t >>= 52;
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)v1 * (dpint)p4;
  t += (dpint)v2 * (dpint)p3;
  t += (dpint)v3 * (dpint)p2;
  t += (dpint)v4 * (dpint)p1;
  c[0] = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)v3 * (dpint)p3;
  t += (dpint)v4 * (dpint)p2;
  c[1] = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
  c[2] = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[4] * b[4];
  t += (dpint)v4 * (dpint)p4;
  c[3] = ((spint)t & mask);
  t >>= 52;
  c[4] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p0 = 0x9cac2fc632551u;
  spint p1 = 0xada7179e84f3bu;
  spint p2 = 0xfffffffbce6fau;
  spint p3 = 0xfffffffffu;
  spint p4 = 0xffffffff0000u;
  spint q = ((spint)1 << 52u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x1c8aaee00bc4fu;
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = (((spint)t * ndash) & mask);
  t += (udpint)v0 * p0;
  t >>= 52;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p1;
  spint v1 = (((spint)t * ndash) & mask);
  t += (udpint)v1 * p0;
  t >>= 52;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  t += (udpint)v1 * p1;
  spint v2 = (((spint)t * ndash) & mask);
  t += (udpint)v2 * p0;
  t >>= 52;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p3;
  t += (udpint)v1 * p2;
  t += (udpint)v2 * p1;
  spint v3 = (((spint)t * ndash) & mask);
  t += (udpint)v3 * p0;
  t >>= 52;
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
  t >>= 52;
  tot = (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p4;
  t += (udpint)v2 * p3;
  t += (udpint)v3 * p2;
  t += (udpint)v4 * p1;
  c[0] = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v2 * p4;
  t += (udpint)v3 * p3;
  t += (udpint)v4 * p2;
  c[1] = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v3 * p4;
  t += (udpint)v4 * p3;
  c[2] = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v4 * p4;
  c[3] = ((spint)t & mask);
  t >>= 52;
  c[4] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 5; i++) {
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
  spint x[5];
  spint t0[5];
  spint t1[5];
  spint t2[5];
  spint t3[5];
  spint t4[5];
  spint t5[5];
  spint t6[5];
  spint t7[5];
  spint t8[5];
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
  spint s[5];
  spint t[5];
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
  const spint c[5] = {0x5cc0dea6dc3bau, 0x192a067d8a084u, 0xbec59615571bbu,
                      0x1fc245b2392b6u, 0xe12d9559d956u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[5];
  c[0] = 1;
  for (i = 1; i < 5; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[5];
  for (i = 0; i < 5; i++) {
    b[i] = n[i];
    t[i] = n[i + 5];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[5];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 5; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 52u) &
          (((c0 ^ (spint)1) - (spint)1) >> 52u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[5];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 5; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 52u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 5; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 5; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 5; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[5];
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
  for (i = 0; i < 5; i++) {
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
  for (i = 0; i < 5; i++) {
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
  spint t[5];
  spint b[5];
  spint v[5];
  spint z[5] = {0x2d7fbb41e6602u, 0xd004378daf059u, 0x42a3dfc1546cau,
                0x992ba807ace8u, 0xffc97f062a77u};
  spint s[5];
  spint y[5];
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
  a[4] = ((a[4] << n)) | (a[3] >> (52u - n));
  for (i = 3; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffffffffff) | (a[i - 1] >> (52u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 4; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (52u - n)) & (spint)0xfffffffffffff);
  }
  a[4] = a[4] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 52u;
  unsigned int m = r % 52u;
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
  spint c[5];
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
  for (i = 0; i < 5; i++) {
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
  spint c[5];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[5], d[5];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 5; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 52) & 1;
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
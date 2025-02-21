
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

// Automatically generated modular arithmetic C code
// Command line : python montyms64.py nist384
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define Wordlength 64
#define Nlimbs 7
#define Radix 56
#define Nbits 384
#define Nbytes 48

#define MONTGOMERY
#define NIST384

#include <intrin.h>

// t+=a*b
static inline void accum(spint *tl, spint *th, spint a, spint b) {
  unsigned char carry;
  spint wl, wh;
  wl = _mulx_u64(a, b, &wh);
  carry = _addcarryx_u64(0, wl, *tl, tl);
  _addcarryx_u64(carry, wh, *th, th);
}

// t+=(a+b)*c
static inline void accumx(spint *tl, spint *th, spint a, spint bl, spint bh,
                          spint c) {
  unsigned char carry;
  spint wl, wh, l, h;
  carry = _addcarry_u64(0, a, bl, &l);
  _addcarry_u64(carry, 0, bh, &h);
  wl = _mulx_u64(l, c, &wh);
  wh += h * c;
  carry = _addcarryx_u64(0, wl, *tl, tl);
  _addcarryx_u64(carry, wh, *th, th);
}

// t>>s
static inline void shiftr(spint *tl, spint *th, char s) {
  *tl = __shiftright128(*tl, *th, s);
  *th = (*th >> s);
}

// t<<s
static inline void shiftl(spint *tl, spint *th, char s) {
  *th = __shiftleft128(*tl, *th, s);
  *tl = (*tl << s);
}

// t+= (v<<s)
static inline void accumsl(spint *tl, spint *th, spint v, char s) {
  unsigned char carry;
  spint l, h;
  h = __shiftleft128(v, 0, s);
  l = (v << s);
  carry = _addcarry_u64(0, l, *tl, tl);
  _addcarry_u64(carry, h, *th, th);
}

// r=t>>s
static inline spint shiftout(spint tl, spint th, char s) {
  return __shiftright(tl, th, s);
}

// t+=s
static inline void add(spint *tl, spint *th, spint sl, spint sh) {
  unsigned char carry;
  carry = _addcarryx_u64(0, *tl, sl, tl);
  _addcarryx_u64(carry, *th, sh, th);
}

// t-=s
static inline void sub(spint *tl, spint *th, spint sl, spint sh) {
  unsigned char carry;
  carry = _subborrow_u64(0, *tl, sl, tl);
  _subborrow_u64(carry, *th, sh, th);
}

// t=a*b
static inline void mul(spint *tl, spint *th, spint a, spint b) {
  *tl = _mulx_u64(a, b, th);
}

// t*=m
static inline void muli(spint *tl, spint *th, spint m) {
  unsigned char carry;
  spint w;
  *tl = _mulx_u64(*tl, m, &w);
  *th = w + (*th) * m;
}

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
  spint tl = 0, th = 0;
  spint p0 = 0xec196accc52973u;
  spint p1 = 0xdb248b0a77aecu;
  spint p2 = 0x81f4372ddf581au;
  spint p3 = 0xffffffffc7634du;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xd46089e88fdc45u;
  accum(&tl, &th, a[0], b[0]);
  spint v0 = ((tl * ndash) & mask);
  accum(&tl, &th, v0, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[1]);
  accum(&tl, &th, a[1], b[0]);
  accum(&tl, &th, v0, p1);
  spint v1 = ((tl * ndash) & mask);
  accum(&tl, &th, v1, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[2]);
  accum(&tl, &th, a[1], b[1]);
  accum(&tl, &th, a[2], b[0]);
  accum(&tl, &th, v0, p2);
  accum(&tl, &th, v1, p1);
  spint v2 = ((tl * ndash) & mask);
  accum(&tl, &th, v2, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[3]);
  accum(&tl, &th, a[1], b[2]);
  accum(&tl, &th, a[2], b[1]);
  accum(&tl, &th, a[3], b[0]);
  accum(&tl, &th, v0, p3);
  accum(&tl, &th, v1, p2);
  accum(&tl, &th, v2, p1);
  spint v3 = ((tl * ndash) & mask);
  accum(&tl, &th, v3, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[4]);
  accum(&tl, &th, a[1], b[3]);
  accum(&tl, &th, a[2], b[2]);
  accum(&tl, &th, a[3], b[1]);
  accum(&tl, &th, a[4], b[0]);
  add(&tl, &th, q - v0, 0);
  accum(&tl, &th, v1, p3);
  accum(&tl, &th, v2, p2);
  accum(&tl, &th, v3, p1);
  spint v4 = ((tl * ndash) & mask);
  accum(&tl, &th, v4, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[5]);
  accum(&tl, &th, a[1], b[4]);
  accum(&tl, &th, a[2], b[3]);
  accum(&tl, &th, a[3], b[2]);
  accum(&tl, &th, a[4], b[1]);
  accum(&tl, &th, a[5], b[0]);
  spint s = (spint)mask;
  s -= v1;
  accum(&tl, &th, v2, p3);
  accum(&tl, &th, v3, p2);
  accum(&tl, &th, v4, p1);
  add(&tl, &th, s, 0);
  spint v5 = ((tl * ndash) & mask);
  accum(&tl, &th, v5, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[6]);
  accum(&tl, &th, a[1], b[5]);
  accum(&tl, &th, a[2], b[4]);
  accum(&tl, &th, a[3], b[3]);
  accum(&tl, &th, a[4], b[2]);
  accum(&tl, &th, a[5], b[1]);
  accum(&tl, &th, a[6], b[0]);
  s = (spint)mask;
  accumsl(&tl, &th, v0, 48u);
  s -= v2;
  accum(&tl, &th, v3, p3);
  accum(&tl, &th, v4, p2);
  accum(&tl, &th, v5, p1);
  add(&tl, &th, s, 0);
  spint v6 = ((tl * ndash) & mask);
  accum(&tl, &th, v6, p0);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[1], b[6]);
  accum(&tl, &th, a[2], b[5]);
  accum(&tl, &th, a[3], b[4]);
  accum(&tl, &th, a[4], b[3]);
  accum(&tl, &th, a[5], b[2]);
  accum(&tl, &th, a[6], b[1]);
  s = (spint)mask;
  accumsl(&tl, &th, v1, 48u);
  s -= v3;
  accum(&tl, &th, v4, p3);
  accum(&tl, &th, v5, p2);
  accum(&tl, &th, v6, p1);
  add(&tl, &th, s, 0);
  c[0] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[2], b[6]);
  accum(&tl, &th, a[3], b[5]);
  accum(&tl, &th, a[4], b[4]);
  accum(&tl, &th, a[5], b[3]);
  accum(&tl, &th, a[6], b[2]);
  s = (spint)mask;
  accumsl(&tl, &th, v2, 48u);
  s -= v4;
  accum(&tl, &th, v5, p3);
  accum(&tl, &th, v6, p2);
  add(&tl, &th, s, 0);
  c[1] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[3], b[6]);
  accum(&tl, &th, a[4], b[5]);
  accum(&tl, &th, a[5], b[4]);
  accum(&tl, &th, a[6], b[3]);
  s = (spint)mask;
  accumsl(&tl, &th, v3, 48u);
  s -= v5;
  accum(&tl, &th, v6, p3);
  add(&tl, &th, s, 0);
  c[2] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[4], b[6]);
  accum(&tl, &th, a[5], b[5]);
  accum(&tl, &th, a[6], b[4]);
  s = (spint)mask;
  accumsl(&tl, &th, v4, 48u);
  s -= v6;
  add(&tl, &th, s, 0);
  c[3] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[5], b[6]);
  accum(&tl, &th, a[6], b[5]);
  s = (spint)mask;
  accumsl(&tl, &th, v5, 48u);
  add(&tl, &th, s, 0);
  c[4] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[6], b[6]);
  s = (spint)mask;
  accumsl(&tl, &th, v6, 48u);
  add(&tl, &th, s, 0);
  c[5] = (tl & mask);
  shiftr(&tl, &th, 56);
  sub(&tl, &th, 1u, 0);
  c[6] = tl;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  spint totl, toth;
  spint tl = 0, th = 0;
  spint p0 = 0xec196accc52973u;
  spint p1 = 0xdb248b0a77aecu;
  spint p2 = 0x81f4372ddf581au;
  spint p3 = 0xffffffffc7634du;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0xd46089e88fdc45u;
  mul(&totl, &toth, a[0], a[0]);
  tl = totl;
  th = toth;
  spint v0 = ((tl * ndash) & mask);
  accum(&tl, &th, v0, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[1]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p1);
  spint v1 = ((tl * ndash) & mask);
  accum(&tl, &th, v1, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[2]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[1], a[1]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p2);
  accum(&tl, &th, v1, p1);
  spint v2 = ((tl * ndash) & mask);
  accum(&tl, &th, v2, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[3]);
  accum(&totl, &toth, a[1], a[2]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p3);
  accum(&tl, &th, v1, p2);
  accum(&tl, &th, v2, p1);
  spint v3 = ((tl * ndash) & mask);
  accum(&tl, &th, v3, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[4]);
  accum(&totl, &toth, a[1], a[3]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[2], a[2]);
  add(&tl, &th, totl, toth);
  add(&tl, &th, q - v0, 0);
  accum(&tl, &th, v1, p3);
  accum(&tl, &th, v2, p2);
  accum(&tl, &th, v3, p1);
  spint v4 = ((tl * ndash) & mask);
  accum(&tl, &th, v4, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[5]);
  accum(&totl, &toth, a[1], a[4]);
  accum(&totl, &toth, a[2], a[3]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  spint s = (spint)mask;
  s -= v1;
  accum(&tl, &th, v2, p3);
  accum(&tl, &th, v3, p2);
  accum(&tl, &th, v4, p1);
  add(&tl, &th, s, 0);
  spint v5 = ((tl * ndash) & mask);
  accum(&tl, &th, v5, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[6]);
  accum(&totl, &toth, a[1], a[5]);
  accum(&totl, &toth, a[2], a[4]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[3], a[3]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v0, 48u);
  s -= v2;
  accum(&tl, &th, v3, p3);
  accum(&tl, &th, v4, p2);
  accum(&tl, &th, v5, p1);
  add(&tl, &th, s, 0);
  spint v6 = ((tl * ndash) & mask);
  accum(&tl, &th, v6, p0);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[1], a[6]);
  accum(&totl, &toth, a[2], a[5]);
  accum(&totl, &toth, a[3], a[4]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v1, 48u);
  s -= v3;
  accum(&tl, &th, v4, p3);
  accum(&tl, &th, v5, p2);
  accum(&tl, &th, v6, p1);
  add(&tl, &th, s, 0);
  c[0] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[2], a[6]);
  accum(&totl, &toth, a[3], a[5]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[4], a[4]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v2, 48u);
  s -= v4;
  accum(&tl, &th, v5, p3);
  accum(&tl, &th, v6, p2);
  add(&tl, &th, s, 0);
  c[1] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[3], a[6]);
  accum(&totl, &toth, a[4], a[5]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v3, 48u);
  s -= v5;
  accum(&tl, &th, v6, p3);
  add(&tl, &th, s, 0);
  c[2] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[4], a[6]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[5], a[5]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v4, 48u);
  s -= v6;
  add(&tl, &th, s, 0);
  c[3] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[5], a[6]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v5, 48u);
  add(&tl, &th, s, 0);
  c[4] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[6], a[6]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  accumsl(&tl, &th, v6, 48u);
  add(&tl, &th, s, 0);
  c[5] = (tl & mask);
  shiftr(&tl, &th, 56);
  sub(&tl, &th, 1u, 0);
  c[6] = tl;
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

// Modular multiplication by an integer, c=a*b mod 2p
static void inline modmli(const spint *a, int b, spint *c) {
  spint t[7];
  modint(b, t);
  modmul(a, t, c);
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
  return modis1(r) | modis0(x);
}

// conditional move g to f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void modcmv(int b, const spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, aux;
  spint r = 0x3cc3c33c5aa5a55au;
  c0 = (~b) & (r + 1);
  c1 = b + r;
  for (i = 0; i < 7; i++) {
    s = g[i];
    t = f[i];
    f[i] = aux = c0 * t + c1 * s;
    f[i] = aux - r * (t + s);
  }
}

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void modcsw(int b, volatile spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, aux;
  spint r = 0x3cc3c33c5aa5a55au;
  c0 = (~b) & (r + 1);
  c1 = b + r;
  for (i = 0; i < 7; i++) {
    s = g[i];
    t = f[i];
    w = r * (t + s);
    f[i] = aux = c0 * t + c1 * s;
    f[i] = aux - w;
    g[i] = aux = c0 * s + c1 * t;
    g[i] = aux - w;
  }
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

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 56u;
  unsigned int m = r % 56u;
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

// choice of hash functions
int NIST384_PREHASH(int sha,int mlen,char *m,char * th)
{
    int i;
    char h[64];

    if (sha==48)
    {
        hash384 sh384;
        HASH384_init(&sh384);
        for (i=0;i<mlen;i++)
            HASH384_process(&sh384,m[i]);
        HASH384_hash(&sh384,h);       
        for (i=0;i<48;i++) th[i]=h[i];  
        return 1;
    }
    if (sha==64)
    {
        hash512 sh512;
        HASH512_init(&sh512);
        for (i=0;i<mlen;i++)
            HASH512_process(&sh512,m[i]);
        HASH512_hash(&sh512,h);       
        for (i=0;i<48;i++) th[i]=h[i]; 
        return 1;
    }
    return 0;
}


// input private key, per-message random number, truncated hash of message to be signed. Output signature.
// ran must be Nbytes+8 in length, in this case 56 bytes
void NIST384_SIGN(char *prv,char *ran,char *thm,char *sig)
{
    char h[BYTES];
    point R;
    gel e,r,s,k;

    modimp(thm,e);

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
int NIST384_VERIFY(char *pub,char *thm,char *sig) 
{
    point G,Q;
    int i;
    char rb[BYTES],u[BYTES],v[BYTES];
    gel e,r,s,rds;

    modimp(thm,e);

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
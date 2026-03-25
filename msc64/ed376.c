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
// Command line : python montyms64.py
// 002501103908950821434373181256356772360674006887285404329509619144461898813929743809681342407547708774670617348304951
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define Wordlength 64
#define Nlimbs 7
#define Radix 55
#define Nbits 381
#define Nbytes 48

#define MONTGOMERY
#include <intrin.h>

#define ARCH_X86_64 // remove for other architectures like ARM64 - Note umulh()
                    // intrinsic is still required
#ifndef ARCH_X86_64
// t+=a*b
static inline void accum(spint *tl, spint *th, spint a, spint b) {
  spint wl, wh;
  wl = a * b;
  wh = __umulh(a, b);
  *tl += wl;
  *th += (*tl < wl);
  *th += wh;
}

// t+=(a+b)*c
static inline void accumx(spint *tl, spint *th, spint a, spint bl, spint bh,
                          spint c) {
  spint wl, wh, l, h;
  l = a;
  l += bl;
  h = bh + (l < bl);
  wl = l * c;
  wh = __umulh(l, c) + h * c;
  *tl += wl;
  *th += (*tl < wl);
  *th += wh;
}

// t>>s
static inline void shiftr(spint *tl, spint *th, char s) {
  *tl = ((*tl) >> s) | ((*th) << (64 - s));
  *th = (*th) >> s;
}

// t<<s
static inline void shiftl(spint *tl, spint *th, char s) {
  *th = ((*th) << s) | ((*tl) >> (64 - s));
  *tl = (*tl) << s;
}

// t+= (v<<s)
static inline void accumsl(spint *tl, spint *th, spint v, char s) {
  spint wl, wh;
  wh = v >> (64 - s);
  wl = v << s;
  *tl += wl;
  *th += (*tl < wl);
  *th += wh;
}

// r=t>>s
static inline spint shiftout(spint tl, spint th, char s) {
  return (tl >> s) | (th << (64 - s));
}

// t+=s
static inline void add(spint *tl, spint *th, spint sl, spint sh) {
  *tl += sl;
  *th += (*tl < sl);
  *th += sh;
}

// t-=s
static inline void sub(spint *tl, spint *th, spint sl, spint sh) {
  pint l = *tl;
  *tl -= sl;
  *th -= ((*tl) > l);
  *th -= sh;
}

// t=a*b
static inline void mul(spint *tl, spint *th, spint a, spint b) {
  *tl = a * b;
  *th = __umulh(a, b);
}

// return high word of a*b
static inline spint mulhi(spint a, spint b) { return __umulh(a, b); }

// t*=m
static inline void muli(spint *tl, spint *th, spint m) {
  spint w = __umulh(*tl, m);
  *tl = *tl * m;
  *th = w + (*th) * m;
}
#else
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
  return __shiftright128(tl, th, s);
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

// t=a*b
static inline spint mulhi(spint a, spint b) {
  spint r;
  _mulx_u64(a, b, &r);
  return r;
}

// t*=m
static inline void muli(spint *tl, spint *th, spint m) {
  unsigned char carry;
  spint w;
  *tl = _mulx_u64(*tl, m, &w);
  *th = w + (*th) * m;
}

#endif

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 55u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 55u;
  n[0] &= mask;
  for (i = 1; i < 6; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 55u;
  }
  n[6] += (spint)carry;
  return -((n[6] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static spint inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] += ((spint)0x55d4f3c6a7037u) & carry;
  n[1] += ((spint)0x31e53e1a09e13fu) & carry;
  n[2] += ((spint)0x4d4521e734426au) & carry;
  n[3] += ((spint)0x181d34du) & carry;
  n[6] += ((spint)0x4100000000000u) & carry;
  (void)prop(n);
  return (carry & 1);
}

// Montgomery final subtract
static spint inline modfsb(spint *n) {
  n[0] -= (spint)0x55d4f3c6a7037u;
  n[1] -= (spint)0x31e53e1a09e13fu;
  n[2] -= (spint)0x4d4521e734426au;
  n[3] -= (spint)0x181d34du;
  n[6] -= (spint)0x4100000000000u;
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
  n[0] -= (spint)0xaba9e78d4e06eu;
  n[1] -= (spint)0x63ca7c3413c27eu;
  n[2] -= (spint)0x9a8a43ce6884d4u;
  n[3] -= (spint)0x303a69au;
  n[6] -= (spint)0x8200000000000u;
  carry = prop(n);
  n[0] += ((spint)0xaba9e78d4e06eu) & carry;
  n[1] += ((spint)0x63ca7c3413c27eu) & carry;
  n[2] += ((spint)0x9a8a43ce6884d4u) & carry;
  n[3] += ((spint)0x303a69au) & carry;
  n[6] += ((spint)0x8200000000000u) & carry;
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
  n[0] += ((spint)0xaba9e78d4e06eu) & carry;
  n[1] += ((spint)0x63ca7c3413c27eu) & carry;
  n[2] += ((spint)0x9a8a43ce6884d4u) & carry;
  n[3] += ((spint)0x303a69au) & carry;
  n[6] += ((spint)0x8200000000000u) & carry;
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
  n[0] += ((spint)0xaba9e78d4e06eu) & carry;
  n[1] += ((spint)0x63ca7c3413c27eu) & carry;
  n[2] += ((spint)0x9a8a43ce6884d4u) & carry;
  n[3] += ((spint)0x303a69au) & carry;
  n[6] += ((spint)0x8200000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 10471732199883177096670556355664090
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  spint tl = 0, th = 0;
  spint p0 = 0x55d4f3c6a7037u;
  spint p1 = 0x31e53e1a09e13fu;
  spint p2 = 0x4d4521e734426au;
  spint p3 = 0x181d34du;
  spint p6 = 0x4100000000000u;
  spint q = ((spint)1 << 55u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x20b816e69fba79u;
  accum(&tl, &th, a[0], b[0]);
  spint v0 = ((tl * ndash) & mask);
  accum(&tl, &th, v0, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[0], b[1]);
  accum(&tl, &th, a[1], b[0]);
  accum(&tl, &th, v0, p1);
  spint v1 = ((tl * ndash) & mask);
  accum(&tl, &th, v1, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[0], b[2]);
  accum(&tl, &th, a[1], b[1]);
  accum(&tl, &th, a[2], b[0]);
  accum(&tl, &th, v0, p2);
  accum(&tl, &th, v1, p1);
  spint v2 = ((tl * ndash) & mask);
  accum(&tl, &th, v2, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[0], b[3]);
  accum(&tl, &th, a[1], b[2]);
  accum(&tl, &th, a[2], b[1]);
  accum(&tl, &th, a[3], b[0]);
  accum(&tl, &th, v0, p3);
  accum(&tl, &th, v1, p2);
  accum(&tl, &th, v2, p1);
  spint v3 = ((tl * ndash) & mask);
  accum(&tl, &th, v3, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[0], b[4]);
  accum(&tl, &th, a[1], b[3]);
  accum(&tl, &th, a[2], b[2]);
  accum(&tl, &th, a[3], b[1]);
  accum(&tl, &th, a[4], b[0]);
  accum(&tl, &th, v1, p3);
  accum(&tl, &th, v2, p2);
  accum(&tl, &th, v3, p1);
  spint v4 = ((tl * ndash) & mask);
  accum(&tl, &th, v4, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[0], b[5]);
  accum(&tl, &th, a[1], b[4]);
  accum(&tl, &th, a[2], b[3]);
  accum(&tl, &th, a[3], b[2]);
  accum(&tl, &th, a[4], b[1]);
  accum(&tl, &th, a[5], b[0]);
  accum(&tl, &th, v2, p3);
  accum(&tl, &th, v3, p2);
  accum(&tl, &th, v4, p1);
  spint v5 = ((tl * ndash) & mask);
  accum(&tl, &th, v5, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[0], b[6]);
  accum(&tl, &th, a[1], b[5]);
  accum(&tl, &th, a[2], b[4]);
  accum(&tl, &th, a[3], b[3]);
  accum(&tl, &th, a[4], b[2]);
  accum(&tl, &th, a[5], b[1]);
  accum(&tl, &th, a[6], b[0]);
  accum(&tl, &th, v0, p6);
  accum(&tl, &th, v3, p3);
  accum(&tl, &th, v4, p2);
  accum(&tl, &th, v5, p1);
  spint v6 = ((tl * ndash) & mask);
  accum(&tl, &th, v6, p0);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[1], b[6]);
  accum(&tl, &th, a[2], b[5]);
  accum(&tl, &th, a[3], b[4]);
  accum(&tl, &th, a[4], b[3]);
  accum(&tl, &th, a[5], b[2]);
  accum(&tl, &th, a[6], b[1]);
  accum(&tl, &th, v1, p6);
  accum(&tl, &th, v4, p3);
  accum(&tl, &th, v5, p2);
  accum(&tl, &th, v6, p1);
  c[0] = (tl & mask);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[2], b[6]);
  accum(&tl, &th, a[3], b[5]);
  accum(&tl, &th, a[4], b[4]);
  accum(&tl, &th, a[5], b[3]);
  accum(&tl, &th, a[6], b[2]);
  accum(&tl, &th, v2, p6);
  accum(&tl, &th, v5, p3);
  accum(&tl, &th, v6, p2);
  c[1] = (tl & mask);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[3], b[6]);
  accum(&tl, &th, a[4], b[5]);
  accum(&tl, &th, a[5], b[4]);
  accum(&tl, &th, a[6], b[3]);
  accum(&tl, &th, v3, p6);
  accum(&tl, &th, v6, p3);
  c[2] = (tl & mask);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[4], b[6]);
  accum(&tl, &th, a[5], b[5]);
  accum(&tl, &th, a[6], b[4]);
  accum(&tl, &th, v4, p6);
  c[3] = (tl & mask);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[5], b[6]);
  accum(&tl, &th, a[6], b[5]);
  accum(&tl, &th, v5, p6);
  c[4] = (tl & mask);
  shiftr(&tl, &th, 55);
  accum(&tl, &th, a[6], b[6]);
  accum(&tl, &th, v6, p6);
  c[5] = (tl & mask);
  shiftr(&tl, &th, 55);
  c[6] = tl;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  spint totl, toth;
  spint tl = 0, th = 0;
  spint p0 = 0x55d4f3c6a7037u;
  spint p1 = 0x31e53e1a09e13fu;
  spint p2 = 0x4d4521e734426au;
  spint p3 = 0x181d34du;
  spint p6 = 0x4100000000000u;
  spint q = ((spint)1 << 55u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x20b816e69fba79u;
  mul(&totl, &toth, a[0], a[0]);
  tl = totl;
  th = toth;
  spint v0 = ((tl * ndash) & mask);
  accum(&tl, &th, v0, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[0], a[1]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p1);
  spint v1 = ((tl * ndash) & mask);
  accum(&tl, &th, v1, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[0], a[2]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[1], a[1]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p2);
  accum(&tl, &th, v1, p1);
  spint v2 = ((tl * ndash) & mask);
  accum(&tl, &th, v2, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[0], a[3]);
  accum(&totl, &toth, a[1], a[2]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p3);
  accum(&tl, &th, v1, p2);
  accum(&tl, &th, v2, p1);
  spint v3 = ((tl * ndash) & mask);
  accum(&tl, &th, v3, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[0], a[4]);
  accum(&totl, &toth, a[1], a[3]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[2], a[2]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v1, p3);
  accum(&tl, &th, v2, p2);
  accum(&tl, &th, v3, p1);
  spint v4 = ((tl * ndash) & mask);
  accum(&tl, &th, v4, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[0], a[5]);
  accum(&totl, &toth, a[1], a[4]);
  accum(&totl, &toth, a[2], a[3]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v2, p3);
  accum(&tl, &th, v3, p2);
  accum(&tl, &th, v4, p1);
  spint v5 = ((tl * ndash) & mask);
  accum(&tl, &th, v5, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[0], a[6]);
  accum(&totl, &toth, a[1], a[5]);
  accum(&totl, &toth, a[2], a[4]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[3], a[3]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v0, p6);
  accum(&tl, &th, v3, p3);
  accum(&tl, &th, v4, p2);
  accum(&tl, &th, v5, p1);
  spint v6 = ((tl * ndash) & mask);
  accum(&tl, &th, v6, p0);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[1], a[6]);
  accum(&totl, &toth, a[2], a[5]);
  accum(&totl, &toth, a[3], a[4]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v1, p6);
  accum(&tl, &th, v4, p3);
  accum(&tl, &th, v5, p2);
  accum(&tl, &th, v6, p1);
  c[0] = (tl & mask);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[2], a[6]);
  accum(&totl, &toth, a[3], a[5]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[4], a[4]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v2, p6);
  accum(&tl, &th, v5, p3);
  accum(&tl, &th, v6, p2);
  c[1] = (tl & mask);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[3], a[6]);
  accum(&totl, &toth, a[4], a[5]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v3, p6);
  accum(&tl, &th, v6, p3);
  c[2] = (tl & mask);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[4], a[6]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[5], a[5]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v4, p6);
  c[3] = (tl & mask);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[5], a[6]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v5, p6);
  c[4] = (tl & mask);
  shiftr(&tl, &th, 55);
  mul(&totl, &toth, a[6], a[6]);
  add(&tl, &th, totl, toth);
  accum(&tl, &th, v6, p6);
  c[5] = (tl & mask);
  shiftr(&tl, &th, 55);
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
  spint t11[7];
  spint t12[7];
  spint t13[7];
  spint t14[7];
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
  const spint c[7] = {0x7d2010f854686u,  0x6b93767ba32bd2u, 0x107cea69daa242u,
                      0x7b084aad41ba6au, 0xe7b9b00f0bbbbu,  0x3f084bcf33f369u,
                      0x30a9bc768db63u};
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
  return ((spint)1 & ((d - (spint)1) >> 55u) &
          (((c0 ^ (spint)1) - (spint)1) >> 55u));
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
  return ((spint)1 & ((d - (spint)1) >> 55u));
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
  spint p0 = 0x55d4f3c6a7037u;
  spint p1 = 0x31e53e1a09e13fu;
  spint p2 = 0x4d4521e734426au;
  spint p3 = 0x181d34du;
  spint p6 = 0x4100000000000u;
  spint mask = ((spint)1 << 55u) - (spint)1;
  spint tl = 0, th = 0;
  spint q, h, r = 0xfc0fc0fc0fc0fc;
  accum(&tl, &th, a[0], b);
  c[0] = tl & mask;
  shiftr(&tl, &th, 55u);
  accum(&tl, &th, a[1], b);
  c[1] = tl & mask;
  shiftr(&tl, &th, 55u);
  accum(&tl, &th, a[2], b);
  c[2] = tl & mask;
  shiftr(&tl, &th, 55u);
  accum(&tl, &th, a[3], b);
  c[3] = tl & mask;
  shiftr(&tl, &th, 55u);
  accum(&tl, &th, a[4], b);
  c[4] = tl & mask;
  shiftr(&tl, &th, 55u);
  accum(&tl, &th, a[5], b);
  c[5] = tl & mask;
  shiftr(&tl, &th, 55u);
  accum(&tl, &th, a[6], b);
  c[6] = tl;

  // Barrett-Dhem reduction
  h = shiftout(tl, th, 42u);
  q = mulhi(h, r);
  mul(&tl, &th, q, p0);
  c[0] -= tl & mask;
  shiftr(&tl, &th, 55u);
  c[1] -= tl;
  mul(&tl, &th, q, p1);
  c[1] -= tl & mask;
  shiftr(&tl, &th, 55u);
  c[2] -= tl;
  mul(&tl, &th, q, p2);
  c[2] -= tl & mask;
  shiftr(&tl, &th, 55u);
  c[3] -= tl;
  mul(&tl, &th, q, p3);
  c[3] -= tl & mask;
  shiftr(&tl, &th, 55u);
  c[4] -= tl;
  c[6] -= q * p6;
  (void)prop(c);
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
static void __attribute__((noinline))
modcmv(int b, const spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, aux;
  static spint R = 0;
  R += 0x3cc3c33c5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 7; i++) {
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
  R += 0x3cc3c33c5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 7; i++) {
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
  a[6] = ((a[6] << n)) + (a[5] >> (55u - n));
  for (i = 5; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7fffffffffffff) + (a[i - 1] >> (55u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7fffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 6; i++) {
    a[i] = (a[i] >> n) + ((a[i + 1] << (55u - n)) & (spint)0x7fffffffffffff);
  }
  a[6] = a[6] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 55u;
  unsigned int m = r % 55u;
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
  res = (int)modfsb(a);
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
    eq &= (((c[i] ^ d[i]) - 1) >> 55) & 1;
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

const char dom4[10]={'S','i','g','E','d','3','8','3',0,0};

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



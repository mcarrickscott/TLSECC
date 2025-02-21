// TLSECC X448 key exchange API

// Script generated code - Command line : python pseudo.py 32/64 X448

/* Drop script generated code in here, 32 or 64 bit */


// Automatically generated modular arithmetic C code
// Command line : python montyms64.py X448
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define Wordlength 64
#define Nlimbs 8
#define Radix 56
#define Nbits 448
#define Nbytes 56

#define MONTGOMERY
#define X448

#define MULBYINT
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
  spint q = ((spint)1 << 56u);
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[4] -= (spint)1u & carry;
  n[7] += ((spint)1u * q) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  spint q = ((spint)1 << 56u);
  n[0] += (spint)1u;
  n[4] += (spint)1u;
  n[7] -= (spint)1u * q;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static void inline modadd(const spint *a, const spint *b, spint *n) {
  spint q = ((spint)1 << 56u);
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[5] = a[5] + b[5];
  n[6] = a[6] + b[6];
  n[7] = a[7] + b[7];
  n[0] += (spint)2u;
  n[4] += (spint)2u;
  n[7] -= (spint)2u * q;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[4] -= (spint)2u & carry;
  n[7] += ((spint)2u * q) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void inline modsub(const spint *a, const spint *b, spint *n) {
  spint q = ((spint)1 << 56u);
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
  n[0] -= (spint)2u & carry;
  n[4] -= (spint)2u & carry;
  n[7] += ((spint)2u * q) & carry;
  (void)prop(n);
}

// Modular negation
static void inline modneg(const spint *b, spint *n) {
  spint q = ((spint)1 << 56u);
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
  n[0] -= (spint)2u & carry;
  n[4] -= (spint)2u & carry;
  n[7] += ((spint)2u * q) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 41538374868283342313862929710055431
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  spint tl = 0, th = 0;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  accum(&tl, &th, a[0], b[0]);
  spint v0 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[1]);
  accum(&tl, &th, a[1], b[0]);
  spint v1 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[2]);
  accum(&tl, &th, a[1], b[1]);
  accum(&tl, &th, a[2], b[0]);
  spint v2 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[3]);
  accum(&tl, &th, a[1], b[2]);
  accum(&tl, &th, a[2], b[1]);
  accum(&tl, &th, a[3], b[0]);
  spint v3 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[4]);
  accum(&tl, &th, a[1], b[3]);
  accum(&tl, &th, a[2], b[2]);
  accum(&tl, &th, a[3], b[1]);
  accum(&tl, &th, a[4], b[0]);
  add(&tl, &th, q - v0, 0);
  spint v4 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[5]);
  accum(&tl, &th, a[1], b[4]);
  accum(&tl, &th, a[2], b[3]);
  accum(&tl, &th, a[3], b[2]);
  accum(&tl, &th, a[4], b[1]);
  accum(&tl, &th, a[5], b[0]);
  spint s = (spint)mask;
  s -= v1;
  add(&tl, &th, s, 0);
  spint v5 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[6]);
  accum(&tl, &th, a[1], b[5]);
  accum(&tl, &th, a[2], b[4]);
  accum(&tl, &th, a[3], b[3]);
  accum(&tl, &th, a[4], b[2]);
  accum(&tl, &th, a[5], b[1]);
  accum(&tl, &th, a[6], b[0]);
  s = (spint)mask;
  s -= v2;
  add(&tl, &th, s, 0);
  spint v6 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[0], b[7]);
  accum(&tl, &th, a[1], b[6]);
  accum(&tl, &th, a[2], b[5]);
  accum(&tl, &th, a[3], b[4]);
  accum(&tl, &th, a[4], b[3]);
  accum(&tl, &th, a[5], b[2]);
  accum(&tl, &th, a[6], b[1]);
  accum(&tl, &th, a[7], b[0]);
  s = (spint)mask;
  s -= v3;
  add(&tl, &th, s, 0);
  spint v7 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[1], b[7]);
  accum(&tl, &th, a[2], b[6]);
  accum(&tl, &th, a[3], b[5]);
  accum(&tl, &th, a[4], b[4]);
  accum(&tl, &th, a[5], b[3]);
  accum(&tl, &th, a[6], b[2]);
  accum(&tl, &th, a[7], b[1]);
  s = (spint)mask;
  s += v0;
  s -= v4;
  add(&tl, &th, s, 0);
  spint v8 = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[2], b[7]);
  accum(&tl, &th, a[3], b[6]);
  accum(&tl, &th, a[4], b[5]);
  accum(&tl, &th, a[5], b[4]);
  accum(&tl, &th, a[6], b[3]);
  accum(&tl, &th, a[7], b[2]);
  s = (spint)mask;
  s += v1;
  s -= v5;
  add(&tl, &th, s, 0);
  c[0] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[3], b[7]);
  accum(&tl, &th, a[4], b[6]);
  accum(&tl, &th, a[5], b[5]);
  accum(&tl, &th, a[6], b[4]);
  accum(&tl, &th, a[7], b[3]);
  s = (spint)mask;
  s += v2;
  s -= v6;
  add(&tl, &th, s, 0);
  c[1] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[4], b[7]);
  accum(&tl, &th, a[5], b[6]);
  accum(&tl, &th, a[6], b[5]);
  accum(&tl, &th, a[7], b[4]);
  s = (spint)mask;
  s += v3;
  s -= v7;
  add(&tl, &th, s, 0);
  c[2] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[5], b[7]);
  accum(&tl, &th, a[6], b[6]);
  accum(&tl, &th, a[7], b[5]);
  s = (spint)mask;
  s += v4;
  s -= v8;
  add(&tl, &th, s, 0);
  c[3] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[6], b[7]);
  accum(&tl, &th, a[7], b[6]);
  s = (spint)mask;
  s += v5;
  add(&tl, &th, s, 0);
  c[4] = (tl & mask);
  shiftr(&tl, &th, 56);
  accum(&tl, &th, a[7], b[7]);
  s = (spint)mask;
  s += v6;
  add(&tl, &th, s, 0);
  c[5] = (tl & mask);
  shiftr(&tl, &th, 56);
  s = (spint)mask;
  s += v7;
  add(&tl, &th, s, 0);
  c[6] = (tl & mask);
  shiftr(&tl, &th, 56);
  add(&tl, &th, v8 - (spint)1, 0);
  c[7] = tl;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
  spint totl, toth;
  spint tl = 0, th = 0;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  mul(&totl, &toth, a[0], a[0]);
  tl = totl;
  th = toth;
  spint v0 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[1]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  spint v1 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[2]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[1], a[1]);
  add(&tl, &th, totl, toth);
  spint v2 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[3]);
  accum(&totl, &toth, a[1], a[2]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  spint v3 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[4]);
  accum(&totl, &toth, a[1], a[3]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[2], a[2]);
  add(&tl, &th, totl, toth);
  add(&tl, &th, q - v0, 0);
  spint v4 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[5]);
  accum(&totl, &toth, a[1], a[4]);
  accum(&totl, &toth, a[2], a[3]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  spint s = (spint)mask;
  s -= v1;
  add(&tl, &th, s, 0);
  spint v5 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[6]);
  accum(&totl, &toth, a[1], a[5]);
  accum(&totl, &toth, a[2], a[4]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[3], a[3]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s -= v2;
  add(&tl, &th, s, 0);
  spint v6 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[0], a[7]);
  accum(&totl, &toth, a[1], a[6]);
  accum(&totl, &toth, a[2], a[5]);
  accum(&totl, &toth, a[3], a[4]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s -= v3;
  add(&tl, &th, s, 0);
  spint v7 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[1], a[7]);
  accum(&totl, &toth, a[2], a[6]);
  accum(&totl, &toth, a[3], a[5]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[4], a[4]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v0;
  s -= v4;
  add(&tl, &th, s, 0);
  spint v8 = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[2], a[7]);
  accum(&totl, &toth, a[3], a[6]);
  accum(&totl, &toth, a[4], a[5]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v1;
  s -= v5;
  add(&tl, &th, s, 0);
  c[0] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[3], a[7]);
  accum(&totl, &toth, a[4], a[6]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[5], a[5]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v2;
  s -= v6;
  add(&tl, &th, s, 0);
  c[1] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[4], a[7]);
  accum(&totl, &toth, a[5], a[6]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v3;
  s -= v7;
  add(&tl, &th, s, 0);
  c[2] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[5], a[7]);
  add(&totl, &toth, totl, toth);
  accum(&totl, &toth, a[6], a[6]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v4;
  s -= v8;
  add(&tl, &th, s, 0);
  c[3] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[6], a[7]);
  add(&totl, &toth, totl, toth);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v5;
  add(&tl, &th, s, 0);
  c[4] = (tl & mask);
  shiftr(&tl, &th, 56);
  mul(&totl, &toth, a[7], a[7]);
  add(&tl, &th, totl, toth);
  s = (spint)mask;
  s += v6;
  add(&tl, &th, s, 0);
  c[5] = (tl & mask);
  shiftr(&tl, &th, 56);
  s = (spint)mask;
  s += v7;
  add(&tl, &th, s, 0);
  c[6] = (tl & mask);
  shiftr(&tl, &th, 56);
  add(&tl, &th, v8 - (spint)1, 0);
  c[7] = tl;
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
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modsqr(z, z);
  modmul(x, z, z);
  modcpy(z, t0);
  modnsqr(t0, 3);
  modmul(z, t0, z);
  modcpy(z, t0);
  modnsqr(t0, 6);
  modmul(z, t0, t0);
  modcpy(t0, t1);
  modnsqr(t1, 12);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 6);
  modmul(z, t1, z);
  modnsqr(t1, 18);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 48);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 96);
  modmul(t0, t1, t0);
  modnsqr(t0, 30);
  modmul(z, t0, z);
  modsqr(z, t0);
  modmul(x, t0, t0);
  modnsqr(t0, 223);
  modmul(z, t0, z);
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
  const spint c[8] = {0x0u, 0x0u, 0x2u, 0x0u, 0x0u, 0x0u, 0x3u, 0x0u};
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

// Modular multiplication by an integer, c=a*b mod 2p
static void inline modmli(const spint *a, int b, spint *c) {
  spint tl = 0, th = 0;
  spint s;
  spint mask = ((spint)1 << 56u) - (spint)1;
  accum(&tl, &th, a[0], b);
  c[0] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[1], b);
  c[1] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[2], b);
  c[2] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[3], b);
  c[3] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[4], b);
  c[4] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[5], b);
  c[5] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[6], b);
  c[6] = tl & mask;
  shiftr(&tl, &th, 56u);
  accum(&tl, &th, a[7], b);
  c[7] = tl & mask;
  shiftr(&tl, &th, 56u);
  // reduction pass

  s = tl;
  c[0] += s;
  c[4] += s;
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
// strongly recommend inlining be disabled using compiler specific syntax
static void modcmv(int b, const spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, aux;
  spint r = 0x3cc3c33c5aa5a55au;
  c0 = (~b) & (r + 1);
  c1 = b + r;
  for (i = 0; i < 8; i++) {
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
  for (i = 0; i < 8; i++) {
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


#define A24 39081   // Montgomery curve constant (A-2)/4
#define COF 2       // Montgomery curve cofactor = 2^cof (2 or 3)
#define GENERATOR 5
#define TWIST_SECURE // If it is a twist secure curve

// reverse bytes. Useful when dealing with little-endian formats
static void reverse(char *w)
{
    int i;
    for (i = 0; i < (Nbytes/2); i++) {
        unsigned char ch = w[i];
        w[i] = w[Nbytes - i - 1]; 
        w[Nbytes - i - 1] = ch; 
    } 
}

// clamp input - see RFC7748
static void clamp(char *bk) {
    int s=(8-(Nbits%8))%8;
    bk[0]&=-(1<<COF);
    char mask=(unsigned char)(0xffu>>s);
    bk[Nbytes-1]&=mask;
    bk[Nbytes-1]|=(unsigned char)(0x80u>>s);
}

// return nth bit of byte array
static int bit(int n,const char *a) {
    return (int)((a[n/8u]&((unsigned char)1u<<(n%8u)))>>(n%8u));
}

// RFC7748 - Montgomery curve
// bv=bk*bu, bu,bv are x coordinates on elliptic curve
static void rfc7748(const char *bk, const char *bu,char *bv) {
    int i;
    int kt;
    int swap = 0;
    char ck[Nbytes];
    char cu[Nbytes];
    spint u[Nlimbs]; spint x1[Nlimbs]; spint x2[Nlimbs]; spint x3[Nlimbs]; spint z2[Nlimbs]; spint z3[Nlimbs];
    spint A[Nlimbs]; spint B[Nlimbs]; spint AA[Nlimbs]; spint BB[Nlimbs]; spint C[Nlimbs]; spint D[Nlimbs]; spint E[Nlimbs];

    for (i=0;i<Nbytes;i++) {
        ck[i]=bk[i];
        cu[i]=bu[i];
    }

    reverse(cu);  // convert from little to big endian
    
// clamp input
    clamp(ck);

// import into internal representation
    modimp(cu,u);

    modcpy(u,x1);  // x_1=u
    modone(x2);    // x_2=1
    modzer(z2);    // z_2=0
    modcpy(u,x3);  // x_3=u
    modone(z3);    // z_3=1

    for (i=Nbits-1;i>=0;i--)
    {
        kt=bit(i,ck);
        swap^=kt;
        modcsw(swap,x2,x3);
        modcsw(swap,z2,z3);
        swap=kt;
            
        modadd(x2,z2,A);        // A = x_2 + z_2
        modsqr(A,AA);           // AA = A^2
        modsub(x2,z2,B);        // B = x_2 - z_2
        modsqr(B,BB);           // BB = B^2

        modsub(AA,BB,E);        // E = AA - BB
        modadd(x3,z3,C);        // C = x_3 + z_3
        
        modsub(x3,z3,D);        // D = x_3 - z_3
        modmul(D,A,D);          // DA = D * A
        modmul(C,B,C);          // CB = C * B
 
        modadd(D,C,x3); modsqr(x3,x3);    // x_3 = (DA + CB)^2
        
        modsub(D,C,z3); modsqr(z3,z3); modmul(z3,x1,z3);  // z_3 = x_1 * (DA - CB)^2
        modmul(AA,BB,x2);       // x_2 = AA * BB
        modmli(E,A24,z2);        
        modadd(z2,AA,z2); modmul(z2,E,z2);   // z_2 = E * (AA + a24 * E)
    }
    modcsw(swap,x2,x3);
    modcsw(swap,z2,z3);

#ifdef TWIST_SECURE
    modpro(z2,A);       
    modinv(z2,A,z2);    // sufficient for twist secure curves like X25519 and X448 
#else
    // Do cheap point validation here - see https://eprint.iacr.org/2020/1497
    modmul(u,z2,B);     // wZ
    modmul(B,z2,A);     // wZ^2
    modpro(A,E);        // y
    modcpy(A,C);
    modmul(E,z2,D);     // y.Z2
    modsqr(D,D);     
    modmul(D,u,D);      // w.(y.z2)^2 
    for (i=0;i<(COF-2);i++) {  // COF is 2 or 3
        modsqr(C,C);
        modmul(C,A,C);
    }
    for (i=0;i<COF;i++) {
        modsqr(E,E);
    }
    modmul(C,E,C);
    modmul(C,B,z2);
    for (i=0;i<(COF-2);i++) {
        modsqr(D,D);
    }
    modone(A); modadd(D,A,D); modfsb(D); modshr(1,D); // 1 for QR, else 0
    modmul(x2,D,x2); // set to zero for bad input point
#endif
    modmul(x2,z2,x2);   

    modexp(x2,bv);
    reverse(bv); // convert to little endian
}

// API

#include "tlsecc.h"

void X448_KEY_PAIR(char *SK,char *PK)
{
    char bu[Nbytes]={};
    bu[0]=GENERATOR;
    rfc7748(SK,bu,PK);
}

int X448_SHARED_SECRET(char *SK,char *PK,char *SS)
{
    char ors=0;
    rfc7748(SK,PK,SS);

// all zeros is suspect...
    for (int i=0;i<Nbytes;i++) ors|=SS[i];
    if (ors==0) return 0;
    return 1;
}

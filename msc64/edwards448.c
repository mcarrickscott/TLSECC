// Edwards curve support 
// Use python scripts to generate code for ED25519 or ED448, or your own curve
// Ax*2+y^2=1+B*x^2*y^2
// Assumes A constant is -1 or +1
//
// Mike Scott 16th July 2024
// TII
//
// code for 32/64-bit processor for ED25519 curve can be generated  by 
//
// python curve.py 32/64 ED25519
//
// code for 32/64-bit processor for ED448 curve can be generated  by
//
// python curve.py 32/64 ED448

// make sure decoration and generic are both set to False in monty.py or pseudo.py

/*** Insert automatically generated code for modulus field.c here ***/


// Automatically generated modular arithmetic C code
// Command line : python montyms64.py ED448
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
#define ED448

#define MULBYINT
#include <intrin.h>

#define ARCH_X86_64 // remove for other architectures like ARM64 - Note umulh() intrinsic is still required
#ifndef ARCH_X86_64

// t+=a*b
static inline void accum(spint *tl, spint *th, spint a, spint b) {
  spint wl, wh;
  wl=a*b;
  wh=__umulh(a,b);
  *tl+=wl;
  *th+=(*tl<wl);
  *th+=wh;
}

// t+=(a+b)*c
static inline void accumx(spint *tl, spint *th, spint a, spint bl, spint bh,
                          spint c) {
  spint wl, wh, l, h;
  l=a;
  l+=bl;
  h=bh+(l<bl);
  wl=l*c;
  wh=__umulh(l,c)+h*c;
  *tl+=wl;
  *th+=(*tl<wl);
  *th+=wh;
}

// t>>s
static  inline void shiftr(spint *tl, spint *th, char s) {
  *tl=((*tl)>>s)|((*th)<<(64-s));
  *th=(*th)>>s;
}

// t<<s
static  inline void shiftl(spint *tl, spint *th, char s) {
  *th = ((*th)<<s)|((*tl)>>(64-s));
  *tl = (*tl) << s;
}

// t+= (v<<s)
static  inline void accumsl(spint *tl, spint *th, spint v, char s) {
  spint wl, wh;
  wh = v>>(64-s);
  wl = v << s;         
  *tl+=wl;
  *th+=(*tl<wl);
  *th+=wh;
}

// r=t>>s
static  inline spint shiftout(spint tl, spint th, char s) {
  return (tl>>s)|(th<<(64-s));
}

// t+=s
static  inline void add(spint *tl, spint *th, spint sl, spint sh) {
  *tl+=sl;
  *th+=(*tl<sl);
  *th+=sh;
}

// t-=s
static  inline void sub(spint *tl, spint *th, spint sl, spint sh) {
  spint l=*tl;
  *tl-=sl;
  *th-=((*tl)>l);
  *th-=sh;
}

// t=a*b
static  inline void mul(spint *tl, spint *th, spint a, spint b) {
  *tl=a*b;
  *th=__umulh(a,b);
}

// t*=m
static  inline void muli(spint *tl, spint *th, spint m) {
  spint w=__umulh(*tl,m);
  *tl = *tl*m;
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

#endif

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
  spint c0, c1, s, t, r, aux;
  static spint R=0;
  R+=0x3cc3c33c5aa5a55a;
  r=R;
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
  spint c0, c1, s, t, r, w, aux;
  static spint R=0;
  R+=0x3cc3c33c5aa5a55a;
  r=R;
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


/*** End of automatically generated code ***/

#include "Ed448curve.h"

#define BYTES Nbytes
#define LIMBS Nlimbs
#define TOPBIT (8*sizeof(int)-1)

/*** Insert automatically generated curve definition curve.c here ***/


#define COF 2
#define CONSTANT_A 1
#define CONSTANT_B -39081
static const spint constant_x[8]={0x420685f0ea8836,0x35bf93b17aa383,0xb7bc2914f8fe6d,0xe44cd37ab765fa,0x34f39b1b69235e,0x44d6fb9be886a8,0xee96c7295e6eb4,0xd16ef0905d88b9};
static const spint constant_y[8]={0xd81f4fba184177,0xac119c79a99632,0xda8e9ac23c2104,0x416ef259fc5486,0x46ff5902c1cc32,0x4fa9dd01223251,0xa1f0e6acaf9471,0x65f7687a33ab50};



/*** End of automatically generated code ***/


// return 1 if b==c, no branching 
static int teq(int b, int c)
{
    int x = b ^ c;
    x -= 1; // if x=0, x now -1
    return ((x >> TOPBIT) & 1);
}

// copy point
void ecn448cpy(point *Q,point *P)
{
    modcpy(Q->x,P->x);
    modcpy(Q->y,P->y);
    modcpy(Q->z,P->z);
}

// randomize projective point
void ecn448ran(int r,point *P)
{
    if (r>1)
    {
        modmli(P->x,r,P->x);
        modmli(P->y,r,P->y);
        modmli(P->z,r,P->z);
    }
}

// negate P
void ecn448neg(point *P)
{
    modneg(P->x,P->x);
}

// add Q to P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
void ecn448add(point *Q,point *P)
{
    spint A[Nlimbs],B[Nlimbs],C[Nlimbs],D[Nlimbs],E[Nlimbs],F[Nlimbs],G[Nlimbs];
    modmul(Q->z,P->z,A);
    modsqr(A,B);
    modmul(Q->x,P->x,C);
    modmul(Q->y,P->y,D);
    modmul(C,D,E);
#ifdef CONSTANT_B
#if CONSTANT_B>0
    modmli(E,CONSTANT_B,E);
    modsub(B,E,F);
    modadd(B,E,G);
#else
    modmli(E,-CONSTANT_B,E);
    modadd(B,E,F);
    modsub(B,E,G);
#endif
#else
    modmul(E,constant_b,E);
    modsub(B,E,F);
    modadd(B,E,G);
#endif
    modadd(P->x,P->y,B);
    modadd(Q->x,Q->y,E);
    modmul(B,E,P->x);
    modsub(P->x,C,P->x);
    modsub(P->x,D,P->x);
    modmul(P->x,F,P->x);
    modmul(P->x,A,P->x);
#if CONSTANT_A==-1
    modadd(D,C,P->y);
#else
    modsub(D,C,P->y);
#endif
    modmul(P->y,A,P->y);
    modmul(P->y,G,P->y);
    modmul(F,G,P->z);
}

// subtract Q from P
void ecn448sub(point *Q,point *P)
{
    point W;
    ecn448cpy(Q,&W); ecn448neg(&W);
    ecn448add(&W,P);
}

// double P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
void ecn448dbl(point *P)
{
    spint B[Nlimbs],C[Nlimbs],D[Nlimbs],E[Nlimbs],F[Nlimbs],H[Nlimbs],J[Nlimbs];
    modadd(P->x,P->y,B);
    modsqr(B,B);
    modsqr(P->x,C);
    modsqr(P->y,D);
    modsqr(P->z,H);
    modadd(H,H,H);
#if CONSTANT_A==-1
    modneg(C,E);
#else
    modcpy(C,E);
#endif
    modadd(E,D,F);
    modsub(F,H,J);
    modsub(B,C,P->x);
    modsub(P->x,D,P->x);
    modmul(P->x,J,P->x);
    modsub(E,D,P->y);
    modmul(P->y,F,P->y);
    modmul(F,J,P->z);
}
/*
      B = (X1+Y1)^2
      C = X1^2
      D = Y1^2
      H = 2Z1^2

      E = C                 
      F = E+D               F=C+D
      J = F-H               J=F-H
      X3 = (B-C-D)*J        X3=(B-F)*J
      Y3 = F*(E-D)          Y3=(C-D)*F
      Z3 = F*J              Z3=F*J

      E = -C
      F = E+D               F=-C+D          F=D-C           F=D-C
      J = F-H               J=F-H           J=-F-H          J=F+H
      X3 = (B-C-D)*J        X3=(B-C-D)*J    X3=(B-C-D)*J    X3=(C+D-B)*J
      Y3 = F*(E-D)          Y3=F*(-C-D)     Y3=F*(C+D)      Y3=F*(C+D)
      Z3 = F*J              Z3=F*J          Z3=-F*J         Z3=F*J


*/

// set to infinity
void ecn448inf(point *P)
{
    modzer(P->x);
    modone(P->y);
    modone(P->z);
}

// test for infinity
int ecn448isinf(point *P)
{
    return (modis0(P->x) && modcmp(P->y,P->z));
}

// set to affine
void ecn448affine(point *P)
{
    spint I[Nlimbs];
    if (modis0(P->z)) {
        ecn448inf(P);
        return;
    }
    modinv(P->z,NULL,I);
    modone(P->z);
    modmul(P->x,I,P->x);
    modmul(P->y,I,P->y);
}

// move Q to P if d=1
void ecn448cmv(int d,point *Q,point *P)
{
    modcmv(d,Q->x,P->x);
    modcmv(d,Q->y,P->y);
    modcmv(d,Q->z,P->z);
}

// return 1 if equal, else 0
int ecn448cmp(point *P,point *Q)
{
    spint a[Nlimbs],b[Nlimbs];
    modmul(P->x,Q->z,a);
    modmul(Q->x,P->z,b);
    if (!modcmp(a,b)) return 0;
    modmul(P->y,Q->z,a);
    modmul(Q->y,P->z,b);
    if (!modcmp(a,b)) return 0;
    return 1;
}

// extract (x,y) from point, if y is NULL compress and just return x and sign of y, if x is NULL compress and just return y and sign of x
int ecn448get(point *P,char *x,char *y)
{
    spint X[Nlimbs],Y[Nlimbs];
    ecn448affine(P);
    if (x!=NULL)
    {
        modcpy(P->x,X);
        modexp(X,x);
    }
    if (y!=NULL)
    {
        modcpy(P->y,Y);
        modexp(Y,y);
    }
    if (y==NULL) return modsign(P->y);
    if (x==NULL) return modsign(P->x);
    return 0;
}

// general purpose set point function
// sets P=O if point not on curve
// if x and y are not NULL tries to set (x,y)
// if y==NULL tries to set from x and sign s of y (decompression)
// if x==NULL tries to set from y and sign s of x
static void setxy(int s,const spint *x,const spint *y,point *P)
{
    spint X[Nlimbs],Y[Nlimbs],O[Nlimbs],U[Nlimbs],V[Nlimbs],H[Nlimbs];
    modone(O);

    if (x!=NULL && y!=NULL)
    {
        modsqr(x,X);
        modsqr(y,Y);
#if CONSTANT_A==-1
        modsub(Y,X,U);
#else
        modadd(Y,X,U);  //lhs
#endif
        modmul(X,Y,V);  //rhs
#ifdef CONSTANT_B
#if CONSTANT_B>0
        modmli(V,CONSTANT_B,V);
        modadd(O,V,V); // V=1+dx^2
#else
        modmli(V,-CONSTANT_B,V);
        modsub(O,V,V); // V=1-dx^2
#endif      
#else
        modmul(V,constant_b,V);
        modadd(O,V,V);
#endif
        modmul(U,O,U); modmul(V,O,V);
        if (modcmp(U,V)) {
            modcpy(x,P->x);
            modcpy(y,P->y);
            modone(P->z);
        } else {
            ecn448inf(P);
        }
        return;
    }
    if (y==NULL)
    {
        modsqr(x,X);
#if CONSTANT_A==-1             // U=1-ax^2
        modadd(O,X,U);
#else
        modsub(O,X,U); 
#endif
        modcpy(X,V);   // V=x^2
    } else {
        modsqr(y,Y);
        modsub(O,Y,U); // U=1-y^2
#if CONSTANT_A==-1
        modneg(O,O);   // O=-1
#endif
        modcpy(Y,V);   // V=y^2
    }
#ifdef CONSTANT_B
#if CONSTANT_B>0
    modmli(V,CONSTANT_B,V);
    modsub(O,V,V); // V=1-dV^2
#else
    modmli(V,-CONSTANT_B,V);
    modadd(O,V,V); // V=1+dV^2
#endif
#else
    modmul(V,constant_b,V);
    modsub(O,V,V); // V=1-dV^2      
#endif

    modsqr(U,O);  // O=U^2
    modmul(U,O,U); // U=U^3
    modmul(U,V,U); // U=U^3*V
    modpro(U,H);
    if (!modqr(H,U))
    { // point not on curve
        ecn448inf(P);
        return;
    }
    modsqrt(U,H,V); // V=sqrt
    modinv(U,H,U);  // U=inv
    modmul(U,V,U);
    modmul(U,O,U);
    int d=(modsign(U)-s)&1;
    modneg(U,V);
    modcmv(d,V,U);
    if (y==NULL)
    {
        modcpy(U,P->y);
        modcpy(x,P->x);
    } else {
        modcpy(U,P->x);
        modcpy(y,P->y);
    }
    modone(P->z);
}

// multiply point by small curve cofactor (here assumed to be 4 or 8)
void ecn448cof(point *P)
{
    int i;
    for (i=0;i<COF;i++)
        ecn448dbl(P);
}

// Is (x,y) of the right order? Must be checked by calling program!
// api visible version, x and y are big endian byte arrays
void ecn448set(int s,const char *x,const char *y,point *P)
{
    spint X[Nlimbs],Y[Nlimbs];
    if (x!=NULL && y!=NULL)
    {
        modimp(x,X);
        modimp(y,Y);
        setxy(s,X,Y,P);
        return;
    }
    if (x!=NULL)
    {
        modimp(x,X);
        setxy(s,X,NULL,P);
    }
    if (y!=NULL)
    {
        modimp(y,Y);
        setxy(s,NULL,Y,P);
    }
}

// set generator
void ecn448gen(point *P)
{
#ifdef CONSTANT_X
    spint X[Nlimbs];
    modint(CONSTANT_X,X);
    setxy(0,X,NULL,P);
#else
    setxy(0,constant_x,constant_y,P);
#endif
}

// select point from precomputed array in constant time
static void select(int b,point W[],point *P)
{
    point MP;  
    int m = b >> TOPBIT;
    int babs = (b ^ m) - m;

    ecn448cmv(teq(babs, 0),&W[0],P); // conditional move
    ecn448cmv(teq(babs, 1),&W[1],P);
    ecn448cmv(teq(babs, 2),&W[2],P);    
    ecn448cmv(teq(babs, 3),&W[3],P);
    ecn448cmv(teq(babs, 4),&W[4],P);
    ecn448cmv(teq(babs, 5),&W[5],P);
    ecn448cmv(teq(babs, 6),&W[6],P);    
    ecn448cmv(teq(babs, 7),&W[7],P);
    ecn448cmv(teq(babs, 8),&W[8],P);

    ecn448cpy(P,&MP);
    ecn448neg(&MP);  // minus P
    ecn448cmv((int)(m & 1),&MP,P);
}

// convert to double naf form
static void dnaf(const char *e,const char *f, signed char *w)
{
    int i,j,t;
    unsigned char ce=0;
    unsigned char cf=0;
    unsigned char m,n,p,q;
    for (i=0;i<Nbytes;i++)
    {
        m=n=e[Nbytes-i-1];
        t=3*(int)n+ce;
        ce=(unsigned char)(t>>8);
        n=(unsigned char)(t&0xff);
        p=q=f[Nbytes-i-1];
        t=3*(int)q+cf;
        cf=(unsigned char)(t>>8);
        q=(unsigned char)(t&0xff);
        for (j=0;j<8;j++)
        {
            w[8*i+j]=(n&1)-(m&1)+3*((q&1)-(p&1));
            n>>=1; m>>=1; p>>=1; q>>=1;
        }
    }
    for (j=0;j<8;j++)
    {
        w[8*Nbytes+j]=(ce&1)+3*(cf&1);
        ce>>=1; cf>>=1;
    }
}

// multiply point by scalar
// constant time
void ecn448mul(const char *e,point *P) 
{
    int i,j;
    point Q,W[9];
    signed char w[2*Nbytes+1];

    ecn448inf(&Q);
    ecn448inf(&W[0]);                         // O
    ecn448cpy(P,&W[1]);                       // P
    ecn448cpy(P,&W[2]); ecn448dbl(&W[2]);        // 2P
    ecn448cpy(&W[2],&W[3]); ecn448add(P,&W[3]);  // 3P
    ecn448cpy(&W[2],&W[4]); ecn448dbl(&W[4]);    // 4P
    ecn448cpy(&W[4],&W[5]); ecn448add(P,&W[5]);  // 5P
    ecn448cpy(&W[3],&W[6]); ecn448dbl(&W[6]);    // 6P
    ecn448cpy(&W[6],&W[7]); ecn448add(P,&W[7]);  // 7P
    ecn448cpy(&W[4],&W[8]); ecn448dbl(&W[8]);    // 8P

// convert exponent to signed digit
    for (i=j=0;i<Nbytes;i++,j+=2)
    {
        char c=e[Nbytes-i-1];
        w[j]=c&0xf;
        w[j+1]=(c>>4)&0xf;
    }
    w[2*Nbytes]=0;
    for (j=0;j<2*Nbytes;j++)
    {
        int t=7-w[j];
        int m=(t>>4)&1;
        w[j]-=(m<<4);
        w[j+1]+=m;
    }

//    printf("w= ");
//    for (i=0;i<2*Nbytes+1;i++) printf(" %d",(int)w[i]);
//    printf("\n");

    select(w[2*Nbytes],W,P);
    for (i = 2*Nbytes - 1; i >= 0; i--)
    {
        select(w[i],W,&Q);
        ecn448dbl(P);
        ecn448dbl(P);
        ecn448dbl(P);
        ecn448dbl(P);
        ecn448add(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
void ecn448mul2(const char *e,point *P,const char *f,point *Q,point *R)
{
    int i,j;
    point W[5];
    signed char w[8*Nbytes+8];
    ecn448inf(&W[0]);     // O
    ecn448cpy(P,&W[1]);   // P
    ecn448cpy(Q,&W[3]);   // Q
    ecn448cpy(Q,&W[2]); ecn448sub(P,&W[2]);  // Q-P
    ecn448cpy(Q,&W[4]); ecn448add(P,&W[4]);  // Q+P

    dnaf(e,f,w);

    i=8*Nbytes+7;
    while (w[i]==0) i--; // ignore leading zeros
    ecn448inf(R);
    while (i>=1)
    {
        ecn448dbl(R);
        j=w[i];
        if (j>0) ecn448add(&W[j],R);
        if (j<0) ecn448sub(&W[-j],R);
        i--;
    }
}

// Weierstrass curve support 
// Use python scripts to generate code for standard curves, or your own curves
// y^2=x^3+Ax+B
// Assumes A constant is -3 or 0
//
// Mike Scott 5th September 2024
// TII
//
// code for 32/64-bit processor for NIST curve can be generated  by 
//
// python curve.py 32/64 NIST384
//

// make sure decoration and generic are both set to False in monty.py or pseudo.py

/*** Insert automatically generated code for modulus field.c here ***/


// Command line : python monty.py 64 NIST384

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
  n[0] += ((spint)0xffffffffu) & carry;
  n[1] += ((spint)0xffff0000000000u) & carry;
  n[2] += ((spint)0xfffffffffeffffu) & carry;
  n[3] -= (spint)1u & carry;
  n[6] += ((spint)0x1000000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] -= (spint)0xffffffffu;
  n[1] -= (spint)0xffff0000000000u;
  n[2] -= (spint)0xfffffffffeffffu;
  n[3] += (spint)1u;
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
  n[0] -= (spint)0x1fffffffeu;
  n[1] -= (spint)0x1fffe0000000000u;
  n[2] -= (spint)0x1fffffffffdfffeu;
  n[3] += (spint)2u;
  n[6] -= (spint)0x2000000000000u;
  carry = prop(n);
  n[0] += ((spint)0x1fffffffeu) & carry;
  n[1] += ((spint)0x1fffe0000000000u) & carry;
  n[2] += ((spint)0x1fffffffffdfffeu) & carry;
  n[3] -= (spint)2u & carry;
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
  n[0] += ((spint)0x1fffffffeu) & carry;
  n[1] += ((spint)0x1fffe0000000000u) & carry;
  n[2] += ((spint)0x1fffffffffdfffeu) & carry;
  n[3] -= (spint)2u & carry;
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
  n[0] += ((spint)0x1fffffffeu) & carry;
  n[1] += ((spint)0x1fffe0000000000u) & carry;
  n[2] += ((spint)0x1fffffffffdfffeu) & carry;
  n[3] -= (spint)2u & carry;
  n[6] += ((spint)0x2000000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 46750875217739594658946410990338056
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p0 = 0xffffffffu;
  spint p1 = 0xffff0000000000u;
  spint p2 = 0xfffffffffeffffu;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x100000001u;
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
  t += (dpint)(spint)(q - v0);
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
  spint s = (spint)mask;
  s -= v1;
  t += (dpint)v2 * (dpint)p2;
  t += (dpint)v3 * (dpint)p1;
  t += (dpint)s;
  spint v4 = (((spint)t * ndash) & mask);
  t += (dpint)v4 * (dpint)p0;
  t >>= 56;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  s = (spint)mask;
  s -= v2;
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
  s -= v3;
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
  s -= v4;
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
  s -= v5;
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
  s -= v6;
  t += (dpint)s;
  c[2] = ((spint)t & mask);
  t >>= 56;
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  s = (spint)mask;
  t += (dpint)(udpint)((udpint)v4 << 48u);
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
  spint p0 = 0xffffffffu;
  spint p1 = 0xffff0000000000u;
  spint p2 = 0xfffffffffeffffu;
  spint q = ((spint)1 << 56u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  spint ndash = 0x100000001u;
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
  t += (udpint)(spint)(q - v0);
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
  spint s = (spint)mask;
  s -= v1;
  t += (udpint)v2 * p2;
  t += (udpint)v3 * p1;
  t += (udpint)s;
  spint v4 = (((spint)t * ndash) & mask);
  t += (udpint)v4 * p0;
  t >>= 56;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  s = (spint)mask;
  s -= v2;
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
  s -= v3;
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
  s -= v4;
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
  s -= v5;
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
  s -= v6;
  t += (udpint)s;
  c[2] = ((spint)t & mask);
  t >>= 56;
  tot = (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  s = (spint)mask;
  t += (udpint)v4 << 48u;
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
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modsqr(z, z);
  modmul(x, z, t1);
  modcpy(t1, z);
  modnsqr(z, 3);
  modmul(t1, z, z);
  modcpy(z, t0);
  modnsqr(t0, 6);
  modmul(z, t0, t0);
  modcpy(t0, t2);
  modnsqr(t2, 12);
  modmul(t0, t2, t0);
  modnsqr(t0, 6);
  modmul(z, t0, z);
  modsqr(z, t0);
  modmul(x, t0, t2);
  modsqr(t2, t0);
  modmul(x, t0, t0);
  modcpy(t0, t3);
  modnsqr(t3, 31);
  modmul(t2, t3, t2);
  modcpy(t2, t3);
  modnsqr(t3, 63);
  modmul(t2, t3, t2);
  modcpy(t2, t3);
  modnsqr(t3, 126);
  modmul(t2, t3, t2);
  modnsqr(t2, 3);
  modmul(t1, t2, t1);
  modnsqr(t1, 33);
  modmul(t0, t1, t0);
  modnsqr(t0, 94);
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
  const spint c[7] = {0xfe000000010000u, 0xffffffu, 0x2u, 0xfffffffe00u,
                      0x1000000020000u,  0x0u,      0x0u};
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
static void modcmv(int d, const spint *g, volatile spint *f) {
  int i;
  spint t;
  spint r0 = f[0] ^ g[1];
  spint r1 = f[1] ^ g[0];
  spint c0 = (1 - (d - r0));
  spint c1 = d+r1;
  for (i = 0; i < 7; i++) {
      t=f[i];
      f[i] =c0*t+c1*g[i];
      f[i]-=r0*t+r1*g[i];
  }
}

// conditional swap g and f if d=1
static void modcsw(int d, volatile spint *g, volatile spint *f) {
  int i;
  spint t,s;
  spint r0 = f[0] ^ g[1];
  spint r1 = f[1] ^ g[0];
  spint c0 = (1 - (d - r0));
  spint c1 = d+r1;
  for (i = 0; i < 7; i++) {
      t=f[i]; s=g[i];
      f[i] =c0*t+c1*s;
      g[i] =c0*s+c1*t;
      f[i]-=r0*t+r1*s;
      g[i]-=r0*s+r1*t;
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

#include "nist384curve.h"

#define BYTES Nbytes
#define LIMBS Nlimbs
#define TOPBIT (8*sizeof(int)-1)

/*** Insert automatically generated curve definition curve.c here ***/


#define COF 1
#define CONSTANT_A -3
static const spint constant_b[7]={0x8870d0412dcccd,0xd9474c32ec0811,0x1920022fc429ad,0x938ae277f2209b,0x2094e3374bee94,0xf9b62b21f41f02,0x8114b604fbf};
static const spint constant_b3[7]={0x995270c3896667,0x8bd5e498c41834,0x4b60068f4c7d09,0xbaa0a767d661d1,0x61bea9a5e3cbbd,0xed228165dc5d06,0x1833e220ef3f};
static const spint constant_x[7]={0x7565fcc0b5284d,0xe2edd6ce383dd0,0x541b4d6e6de378,0xa30eff879c3afc,0xde2b6454868459,0x13812ff723614e,0x3aadc2299e15};
static const spint constant_y[7]={0x3dad2003a4fe2b,0xbfa6b4a9ac2304,0x2e83b050ccbfa8,0xf4ffd98bade756,0xa840c6c3521968,0xe9dd8002263969,0x78abc25a15c5};



/*** End of automatically generated code ***/

// return 1 if b==c, no branching 
static int teq(int b, int c)
{
    int x = b ^ c;
    x -= 1; // if x=0, x now -1
    return ((x >> TOPBIT) & 1);
}

// copy point
void ecn384cpy(point *Q,point *P)
{
    modcpy(Q->x,P->x);
    modcpy(Q->y,P->y);
    modcpy(Q->z,P->z);
}

// negate P
void ecn384neg(point *P)
{
    modneg(P->y,P->y);
}

// add Q to P
// complete formulae from https://eprint.iacr.org/2015/1060
void ecn384add(point *Q,point *P)
{
    spint B[Nlimbs],T0[Nlimbs],T1[Nlimbs],T2[Nlimbs],T3[Nlimbs],T4[Nlimbs];

    modmul(P->x,Q->x,T0);   // 1
    modmul(P->y,Q->y,T1);   // 2
    modmul(P->z,Q->z,T2);   // 3

    modadd(P->x,P->y,T3);   // 4
    modadd(Q->x,Q->y,T4);   // 5
    modmul(T3,T4,T3);       // 6

    modadd(T0,T1,T4);       // 7
    modsub(T3,T4,T3);       // 8
    modadd(P->y,P->z,T4);   // 9

    modadd(Q->y,Q->z,B); // 10
    modmul(T4,B,T4);     // 11
    modadd(T1,T2,B);        // 12  use B

    modsub(T4,B,T4);        // 13
    modadd(P->x,P->z,P->x); // 14
    modadd(Q->z,Q->x,P->y); // 15

    modmul(P->x,P->y,P->x); // 16
    modadd(T0,T2,P->y);     // 17
    modsub(P->x,P->y,P->y); // 18

#if CONSTANT_A==0
    modadd(T0,T0,P->x);   // 19
    modadd(T0,P->x,T0);   // 20

#ifdef CONSTANT_B
#if CONSTANT_B>0
    modmli(T2,3*CONSTANT_B,T2);      // 21
    modmli(P->y,3*CONSTANT_B,P->y);  // 24
#else
    modmli(T2,-3*CONSTANT_B,T2);  modneg(T2,T2);    // 21
    modmli(P->y,-3*CONSTANT_B,P->y);  modneg(P->y,P->y);// 24
#endif
#else
    modcpy(constant_b3,B);
    modmul(T2,B,T2);      // 21
    modmul(P->y,B,P->y);  // 24
#endif
    modadd(T1,T2,P->z);   // 22
    modsub(T1,T2,T1);     // 23

    modmul(P->y,T4,P->x); // 25
    modmul(T3,T1,T2);     // 26
    modsub(T2,P->x,P->x); // 27
    modmul(P->y,T0,P->y); // 28
    modmul(T1,P->z,T1);   // 29
    modadd(P->y,T1,P->y); // 30
    modmul(T0,T3,T0);     // 31
    modmul(P->z,T4,P->z); // 32
    modadd(P->z,T0,P->z); // 33
#else

#ifdef CONSTANT_B
#if CONSTANT_B>0
    modmli(T2,CONSTANT_B,P->z);      //19
    modsub(P->y,P->z,P->x); // 20
    modmli(P->y,CONSTANT_B,P->y);    // 25
#else
    modmli(T2,-CONSTANT_B,P->z);      //19
    modadd(P->y,P->z,P->x); // 20
    modmli(P->y,-CONSTANT_B,P->y); modneg(P->y,P->y);    // 25
#endif
#else
    modcpy(constant_b,B);
    modmul(B,T2,P->z);      //19
    modsub(P->y,P->z,P->x); // 20
    modmul(P->y,B,P->y);    // 25
#endif
    modadd(P->x,P->x,P->z); // 21

    modadd(P->x,P->z,P->x); // 22
    modsub(T1,P->x,P->z);   // 23
    modadd(P->x,T1,P->x);   // 24

    modadd(T2,T2,T1);       // 26
    modadd(T2,T1,T2);       // 27

    modsub(P->y,T2,P->y);   // 28
    modsub(P->y,T0,P->y);   // 29
    modadd(P->y,P->y,T1);   // 30

    modadd(P->y,T1,P->y);   // 31
    modadd(T0,T0,T1);       // 32
    modadd(T0,T1,T0);       // 33

    modsub(T0,T2,T0);       // 34
    modmul(T4,P->y,T1);     // 35
    modmul(T0,P->y,T2);     // 36

    modmul(P->x,P->z,P->y); // 37
    modadd(P->y,T2,P->y);   // 38
    modmul(P->x,T3,P->x);   // 39

    modsub(P->x,T1,P->x);   // 40
    modmul(P->z,T4,P->z);   // 41
    modmul(T3,T0,T1);       // 42

    modadd(P->z,T1,P->z);   // 43
#endif
}

// subtract Q from P
void ecn384sub(point *Q,point *P)
{
    point W;
    ecn384cpy(Q,&W); ecn384neg(&W);
    ecn384add(&W,P);
}

// double P
// complete formuale from https://eprint.iacr.org/2015/1060
void ecn384dbl(point *P)
{
    spint B[Nlimbs],T0[Nlimbs],T1[Nlimbs],T2[Nlimbs],T3[Nlimbs],T4[Nlimbs];

#if CONSTANT_A==0
    modsqr(P->y,T0); // 1
    modadd(T0,T0,T3);  // 2 T3=Z3
    modadd(T3,T3,T3);  // 3
    modadd(T3,T3,T3);  // 4
    modmul(P->x,P->y,T4); // 16 T4=X*Y
    modmul(P->y,P->z,T1); // 5
    modsqr(P->z,T2);      // 6
#ifdef CONSTANT_B
#if CONSTANT_B>0
    modmli(T2,3*CONSTANT_B,T2);      // 7
#else
    modmli(T2,-3*CONSTANT_B,T2); modneg(T2,T2);
#endif
#else
    modcpy(constant_b3,B);
    modmul(T2,B,T2);      // 7
#endif
    modmul(T2,T3,P->x);   // 8
    modadd(T0,T2,P->y);   // 9
    modmul(T3,T1,P->z); // 10
    modadd(T2,T2,T1);     // 11
    modadd(T2,T1,T2);     // 12
    modsub(T0,T2,T0);     // 13
    modmul(P->y,T0,P->y); // 14
    modadd(P->y,P->x,P->y); // 15
    modmul(T0,T4,P->x);    // 17
    modadd(P->x,P->x,P->x); // 18
#else
    modsqr(P->x,T0); // 1
    modsqr(P->y,T1); // 2
    modsqr(P->z,T2); // 3

    modmul(P->x,P->y,T3); // 4
    modmul(P->y,P->z,T4); // 28
    modadd(T3,T3,T3);     // 5
    modmul(P->z,P->x,P->z); // 6

    modadd(P->z,P->z,P->z); // 7

#ifdef CONSTANT_B
#if CONSTANT_B>0 
    modmli(T2,CONSTANT_B,P->y); // 8
    modsub(P->y,P->z,P->y); // 9
    modmli(P->z,CONSTANT_B,P->z); // 18
#else
    modmli(T2,-CONSTANT_B,P->y); modneg(P->y,P->y); // 8
    modsub(P->y,P->z,P->y); // 9
    modmli(P->z,-CONSTANT_B,P->z);  modneg(P->z,P->z); // 18
#endif
#else
    modcpy(constant_b,B);
    modmul(T2,B,P->y); // 8
    modsub(P->y,P->z,P->y); // 9
    modmul(P->z,B,P->z); // 18
#endif

    modadd(P->y,P->y,P->x); // 10
    modadd(P->y,P->x,P->y); // 11
    modsub(T1,P->y,P->x); // 12

    modadd(P->y,T1,P->y); // 13
    modmul(P->y,P->x,P->y); // 14
    modmul(P->x,T3,P->x); // 15

    modadd(T2,T2,T3); // 16
    modadd(T2,T3,T2); // 17

    modsub(P->z,T2,P->z); // 19
    modsub(P->z,T0,P->z); // 20
    modadd(P->z,P->z,T3); // 21

    modadd(P->z,T3,P->z); // 22
    modadd(T0,T0,T3); // 23
    modadd(T0,T3,T0); // 24

    modsub(T0,T2,T0); // 25
    modmul(T0,P->z,T0); // 26
    modadd(P->y,T0,P->y); // 27

    modadd(T4,T4,T4); // 29
    modmul(P->z,T4,P->z); // 30

    modsub(P->x,P->z,P->x); // 31
    modmul(T4,T1,P->z); // 32
    modadd(P->z,P->z,P->z); // 33

    modadd(P->z,P->z,P->z); // 34
#endif
}

// set to infinity
void ecn384inf(point *P)
{
    modzer(P->x);
    modone(P->y);
    modzer(P->z);
}

// test for infinity
int ecn384isinf(point *P)
{
    return (modis0(P->x) && modis0(P->z));
}

// set to affine
void ecn384affine(point *P)
{
    spint I[Nlimbs];
    if (modis0(P->z)) {
        modzer(P->x);
        modone(P->y);
        return;
    }
    modinv(P->z,NULL,I);
    modone(P->z);
    modmul(P->x,I,P->x);
    modmul(P->y,I,P->y);
}

// move Q to P if d=1
void ecn384cmv(int d,point *Q,point *P)
{
    modcmv(d,Q->x,P->x);
    modcmv(d,Q->y,P->y);
    modcmv(d,Q->z,P->z);
}

// return 1 if equal, else 0
int ecn384cmp(point *P,point *Q)
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

// extract (x,y) from point, if y is NULL compress and just return x and sign of y
int ecn384get(point *P,char *x,char *y)
{
    spint X[Nlimbs],Y[Nlimbs];
    ecn384affine(P);

    modcpy(P->x,X);
    modexp(X,x);

    if (y!=NULL)
    {
        modcpy(P->y,Y);
        modexp(Y,y);
        return 0;
    }
    else
        return modsign(P->y);
}
/*
int ecn384getxyz(point *P,char *x,char *y,char *z)
{
    spint X[Nlimbs],Y[Nlimbs];
    modexp(P->x,x);
    modexp(P->y,y);
    modexp(P->z,z);
    return 0;
}
*/
// weierstrass set point function
// sets P=O if point not on curve
// if y!=NULL tries to set (x,y)
// if y==NULL calculates y (decompresses x) and selects sign from s=0/1
static void setxy(int s,const spint *x,const spint *y,point *P)
{
    spint T[Nlimbs],V[Nlimbs],H[Nlimbs];
    modcpy(x,P->x);
    modsqr(x,V);
    modmul(V,x,V); // x^3
#if CONSTANT_A==-3
    modsub(V,x,V);
    modsub(V,x,V);
    modsub(V,x,V); // x^3-3x
#endif
#ifdef CONSTANT_B
#if CONSTANT_B>0
    modint(CONSTANT_B,T);
    modadd(V,T,V); // V=1+dx^2
#else
    modint(-CONSTANT_B,T);
    modsub(V,T,V); // V=1-dx^2
#endif      
#else
    modadd(V,constant_b,V);
#endif
    if (y!=NULL)
    {
        modsqr(y,T);
        if (modcmp(T,V)) {
            modcpy(y,P->y);
            modone(P->z);
        } else {
            ecn384inf(P);
        }
        return;
    }
    modpro(V,H);
    if (!modqr(H,V))
    { // point not on curve
        ecn384inf(P);
        return;
    }
    modsqrt(V,H,P->y);
    int d=(modsign(P->y)-s)&1;
    modneg(P->y,T);
    modcmv(d,T,P->y);
    modone(P->z);
}    

// multiply point by small curve cofactor (here assumed to be 1)
void ecn384cof(point *P)
{}

// api visible version, x and y are big endian byte arrays
void ecn384set(int s,const char *x,const char *y,point *P)
{
    spint X[Nlimbs],Y[Nlimbs];
    modimp(x,X);
    if (y!=NULL)
    {
        modimp(y,Y);
        setxy(s,X,Y,P);
        return;
    }
    setxy(s,X,NULL,P);
}

// set generator
void ecn384gen(point *P)
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

    ecn384cmv(teq(babs, 0),&W[0],P); // conditional move
    ecn384cmv(teq(babs, 1),&W[1],P); // conditional move
    ecn384cmv(teq(babs, 2),&W[2],P); // conditional move
    ecn384cmv(teq(babs, 3),&W[3],P); // conditional move
    ecn384cmv(teq(babs, 4),&W[4],P); // conditional move
    ecn384cmv(teq(babs, 5),&W[5],P); // conditional move
    ecn384cmv(teq(babs, 6),&W[6],P); // conditional move
    ecn384cmv(teq(babs, 7),&W[7],P); // conditional move
    ecn384cmv(teq(babs, 8),&W[8],P); // conditional move
    
    ecn384cpy(P,&MP);
    ecn384neg(&MP);  // minus P
    ecn384cmv((int)(m & 1),&MP,P);
}

// convert to double naf form
static void dnaf(const char *e,const char *f, char *w)
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
void ecn384mul(const char *e,point *P) 
{
    int i,j;
    point Q,W[9];
    signed char w[2*Nbytes+1];

    ecn384inf(&Q);
    ecn384inf(&W[0]);                         // O
    ecn384cpy(P,&W[1]);                       // P
    ecn384cpy(P,&W[2]); ecn384dbl(&W[2]);        // 2P
    ecn384cpy(&W[2],&W[3]); ecn384add(P,&W[3]);  // 3P
    ecn384cpy(&W[2],&W[4]); ecn384dbl(&W[4]);    // 4P
    ecn384cpy(&W[4],&W[5]); ecn384add(P,&W[5]);  // 5P
    ecn384cpy(&W[3],&W[6]); ecn384dbl(&W[6]);    // 6P
    ecn384cpy(&W[6],&W[7]); ecn384add(P,&W[7]);  // 7P
    ecn384cpy(&W[4],&W[8]); ecn384dbl(&W[8]);    // 8P

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
        ecn384dbl(P);
        ecn384dbl(P);
        ecn384dbl(P);
        ecn384dbl(P);
        ecn384add(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
void ecn384mul2(const char *e,point *P,const char *f,point *Q,point *R)
{
    int i,j;
    point W[5];
    signed char w[8*Nbytes+8];
    ecn384inf(&W[0]);     // O
    ecn384cpy(P,&W[1]);   // P
    ecn384cpy(Q,&W[3]);   // Q
    ecn384cpy(Q,&W[2]); ecn384sub(P,&W[2]);  // Q-P
    ecn384cpy(Q,&W[4]); ecn384add(P,&W[4]);  // Q+P

    dnaf(e,f,w);

    i=8*Nbytes+7;
    while (w[i]==0) i--; // ignore leading zeros
    ecn384inf(R);
    while (i>=1)
    {
        ecn384dbl(R);
        j=w[i];
        if (j>0) ecn384add(&W[j],R);
        if (j<0) ecn384sub(&W[-j],R);
        i--;
    }
}

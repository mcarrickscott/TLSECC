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
// python curve.py 32/64 NIST256
//

// make sure decoration and generic are both set to False in monty.py or pseudo.py

/*** Insert automatically generated code for modulus field.c here ***/



// Command line : python monty.py 32 NIST256

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 9
#define Radix 29
#define Nbits 256
#define Nbytes 32

#define MONTGOMERY
#define NIST256

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 29u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 29u;
  n[0] &= mask;
  for (i = 1; i < 8; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 29u;
  }
  n[8] += (spint)carry;
  return -((n[8] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[3] += ((spint)0x200u) & carry;
  n[6] += ((spint)0x40000u) & carry;
  n[7] += ((spint)0x1fe00000u) & carry;
  n[8] += ((spint)0xffffffu) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[3] -= (spint)0x200u;
  n[6] -= (spint)0x40000u;
  n[7] -= (spint)0x1fe00000u;
  n[8] -= (spint)0xffffffu;
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
  n[0] += (spint)2u;
  n[3] -= (spint)0x400u;
  n[6] -= (spint)0x80000u;
  n[7] -= (spint)0x3fc00000u;
  n[8] -= (spint)0x1fffffeu;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[3] += ((spint)0x400u) & carry;
  n[6] += ((spint)0x80000u) & carry;
  n[7] += ((spint)0x3fc00000u) & carry;
  n[8] += ((spint)0x1fffffeu) & carry;
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
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[3] += ((spint)0x400u) & carry;
  n[6] += ((spint)0x80000u) & carry;
  n[7] += ((spint)0x3fc00000u) & carry;
  n[8] += ((spint)0x1fffffeu) & carry;
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
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[3] += ((spint)0x400u) & carry;
  n[6] += ((spint)0x80000u) & carry;
  n[7] += ((spint)0x3fc00000u) & carry;
  n[8] += ((spint)0x1fffffeu) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 2890326096838655498
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p7 = 0x1fe00000u;
  spint p8 = 0xffffffu;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  spint v1 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  spint v2 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)(udpint)((udpint)v0 << 9u);
  spint v3 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)(udpint)((udpint)v1 << 9u);
  spint v4 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  t += (dpint)(udpint)((udpint)v2 << 9u);
  spint v5 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  t += (dpint)(udpint)((udpint)v0 << 18u);
  t += (dpint)(udpint)((udpint)v3 << 9u);
  spint v6 = ((spint)t & mask);
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
  t += (dpint)(udpint)((udpint)v1 << 18u);
  t += (dpint)(udpint)((udpint)v4 << 9u);
  spint v7 = ((spint)t & mask);
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
  t += (dpint)(udpint)((udpint)v2 << 18u);
  t += (dpint)(udpint)((udpint)v5 << 9u);
  spint v8 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  t += (dpint)v1 * (dpint)p8;
  t += (dpint)v2 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v3 << 18u);
  t += (dpint)(udpint)((udpint)v6 << 9u);
  c[0] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  t += (dpint)v2 * (dpint)p8;
  t += (dpint)v3 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v4 << 18u);
  t += (dpint)(udpint)((udpint)v7 << 9u);
  c[1] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  t += (dpint)v3 * (dpint)p8;
  t += (dpint)v4 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v5 << 18u);
  t += (dpint)(udpint)((udpint)v8 << 9u);
  c[2] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  t += (dpint)v4 * (dpint)p8;
  t += (dpint)v5 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v6 << 18u);
  c[3] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  t += (dpint)v5 * (dpint)p8;
  t += (dpint)v6 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v7 << 18u);
  c[4] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)(udpint)((udpint)v8 << 18u);
  c[5] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  c[6] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[8] * b[8];
  t += (dpint)v8 * (dpint)p8;
  c[7] = ((spint)t & mask);
  t >>= 29;
  c[8] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p7 = 0x1fe00000u;
  spint p8 = 0xffffffu;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  spint v1 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  spint v2 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 << 9u;
  spint v3 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v1 << 9u;
  spint v4 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v2 << 9u;
  spint v5 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v0 << 18u;
  t += (udpint)v3 << 9u;
  spint v6 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p7;
  t += (udpint)v1 << 18u;
  t += (udpint)v4 << 9u;
  spint v7 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[8];
  tot += (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v0 * p8;
  t += (udpint)v1 * p7;
  t += (udpint)v2 << 18u;
  t += (udpint)v5 << 9u;
  spint v8 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p8;
  t += (udpint)v2 * p7;
  t += (udpint)v3 << 18u;
  t += (udpint)v6 << 9u;
  c[0] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  t += (udpint)v2 * p8;
  t += (udpint)v3 * p7;
  t += (udpint)v4 << 18u;
  t += (udpint)v7 << 9u;
  c[1] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  t += (udpint)v3 * p8;
  t += (udpint)v4 * p7;
  t += (udpint)v5 << 18u;
  t += (udpint)v8 << 9u;
  c[2] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  t += (udpint)v4 * p8;
  t += (udpint)v5 * p7;
  t += (udpint)v6 << 18u;
  c[3] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  t += (udpint)v5 * p8;
  t += (udpint)v6 * p7;
  t += (udpint)v7 << 18u;
  c[4] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  t += (udpint)v6 * p8;
  t += (udpint)v7 * p7;
  t += (udpint)v8 << 18u;
  c[5] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (udpint)v7 * p8;
  t += (udpint)v8 * p7;
  c[6] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[8] * a[8];
  t += tot;
  t += (udpint)v8 * p8;
  c[7] = ((spint)t & mask);
  t >>= 29;
  c[8] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 9; i++) {
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
  spint x[9];
  spint t0[9];
  spint t1[9];
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modsqr(z, z);
  modmul(x, z, z);
  modcpy(z, t0);
  modnsqr(t0, 3);
  modmul(z, t0, t0);
  modcpy(t0, t1);
  modnsqr(t1, 6);
  modmul(t0, t1, t0);
  modnsqr(t0, 3);
  modmul(z, t0, z);
  modsqr(z, t0);
  modmul(x, t0, t0);
  modcpy(t0, t1);
  modnsqr(t1, 16);
  modmul(t0, t1, t0);
  modnsqr(t0, 15);
  modmul(z, t0, z);
  modnsqr(t0, 17);
  modmul(x, t0, t0);
  modnsqr(t0, 143);
  modmul(z, t0, t0);
  modnsqr(t0, 47);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[9];
  spint t[9];
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
  const spint c[9] = {0xc00u,      0x0u,        0x1fff0000u,
                      0x1fdfffffu, 0x1fbfffffu, 0x1fffffffu,
                      0x1fffffffu, 0x1ffffffeu, 0x13u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[9];
  c[0] = 1;
  for (i = 1; i < 9; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// reduce double length input to n-residue
static void modred(const spint *n, spint *b) {
  int i;
  spint t[9];
  for (i = 0; i < 9; i++) {
    b[i] = n[i];
    t[i] = n[i + 9];
  }
  nres(t, t);
  modadd(b, t, b);
  nres(b, b);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[9];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 9; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 29u) &
          (((c0 ^ (spint)1) - (spint)1) >> 29u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[9];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 9; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 29u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 9; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 9; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 9; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[9];
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
  spint c0,c1,s,t;
  spint r=0x5aa5a55a;
  c0=(~d)&(r+1);
  c1=d|r;
  for (i = 0; i < 9; i++) {
      s=g[i]; t=f[i];
      f[i] =c0*t+c1*s;
      f[i]-=r*(t+s);    
  }
}

// conditional swap g and f if d=1
static void modcsw(int d, volatile spint *g, volatile spint *f) {
  int i;
  spint c0,c1,r,s,t,w;
  for (i = 0; i < 9; i++) {
      s=g[i]; t=f[i];
      r=s^t;
      c0=(~d)&(r+1);
      c1=d|r;
      r*=(t+s);
      f[i] =c0*t+c1*s;
      f[i]-=r;  
      g[i] =c0*s+c1*t;
      g[i]-=r;    
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  spint s[9];
  spint y[9];
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
  a[8] = ((a[8] << n)) | (a[7] >> (29u - n));
  for (i = 7; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x1fffffff) | (a[i - 1] >> (29u - n));
  }
  a[0] = (a[0] << n) & (spint)0x1fffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 8; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (29u - n)) & (spint)0x1fffffff);
  }
  a[8] = a[8] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 29u;
  unsigned int m = r % 29u;
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
  spint c[9];
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
  for (i = 0; i < 9; i++) {
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
  spint c[9];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[9], d[9];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 9; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
  }
  return eq;
}

/*** End of automatically generated code ***/

#include "nist256curve.h"

#define BYTES Nbytes
#define LIMBS Nlimbs
#define TOPBIT (8*sizeof(int)-1)

/*** Insert automatically generated curve definition curve.c here ***/

#define COF 1
#define CONSTANT_A -3
static const spint constant_b[9]={0x1897bbfb,0x1cdf6229,0x18486c4,0x1732821,0x1dad59e0,0xabf7212,0x1a06d110,0x17721d20,0x8600c3};
static const spint constant_b3[9]={0x9c733f2,0x169e267d,0x48d944e,0x4597663,0x19080da0,0x3e5638,0xe107331,0x6765762,0x92024b};
static const spint constant_x[9]={0x15228783,0x730d418,0xdb00bcf,0x57f11fb,0xa20eb75,0x12b77622,0x330fdb9,0x1af4dd57,0x120bee};
static const spint constant_y[9]={0x12aac150,0x125357ce,0xf22e6ef,0xe390e86,0x64b1695,0x88dd21f,0x2a97443,0x2962176,0xae3fe3};



/*** End of automatically generated code ***/

// return 1 if b==c, no branching 
static int teq(int b, int c)
{
    int x = b ^ c;
    x -= 1; // if x=0, x now -1
    return ((x >> TOPBIT) & 1);
}

// copy point
void ecn256cpy(point *Q,point *P)
{
    modcpy(Q->x,P->x);
    modcpy(Q->y,P->y);
    modcpy(Q->z,P->z);
}

// negate P
void ecn256neg(point *P)
{
    modneg(P->y,P->y);
}

// add Q to P
// complete formulae from https://eprint.iacr.org/2015/1060
void ecn256add(point *Q,point *P)
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
void ecn256sub(point *Q,point *P)
{
    point W;
    ecn256cpy(Q,&W); ecn256neg(&W);
    ecn256add(&W,P);
}

// double P
// complete formuale from https://eprint.iacr.org/2015/1060
void ecn256dbl(point *P)
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
void ecn256inf(point *P)
{
    modzer(P->x);
    modone(P->y);
    modzer(P->z);
}

// test for infinity
int ecn256isinf(point *P)
{
    return (modis0(P->x) && modis0(P->z));
}

// set to affine
void ecn256affine(point *P)
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
void ecn256cmv(int d,point *Q,point *P)
{
    modcmv(d,Q->x,P->x);
    modcmv(d,Q->y,P->y);
    modcmv(d,Q->z,P->z);
}

// return 1 if equal, else 0
int ecn256cmp(point *P,point *Q)
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
int ecn256get(point *P,char *x,char *y)
{
    spint X[Nlimbs],Y[Nlimbs];
    ecn256affine(P);

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
int ecn256getxyz(point *P,char *x,char *y,char *z)
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
            ecn256inf(P);
        }
        return;
    }
    modpro(V,H);
    if (!modqr(H,V))
    { // point not on curve
        ecn256inf(P);
        return;
    }
    modsqrt(V,H,P->y);
    int d=(modsign(P->y)-s)&1;
    modneg(P->y,T);
    modcmv(d,T,P->y);
    modone(P->z);
}    

// multiply point by small curve cofactor (here assumed to be 1)
void ecn256cof(point *P)
{}

// api visible version, x and y are big endian byte arrays
void ecn256set(int s,const char *x,const char *y,point *P)
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
void ecn256gen(point *P)
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

    ecn256cmv(teq(babs, 0),&W[0],P); // conditional move
    ecn256cmv(teq(babs, 1),&W[1],P); // conditional move
    ecn256cmv(teq(babs, 2),&W[2],P); // conditional move
    ecn256cmv(teq(babs, 3),&W[3],P); // conditional move
    ecn256cmv(teq(babs, 4),&W[4],P); // conditional move
    ecn256cmv(teq(babs, 5),&W[5],P); // conditional move
    ecn256cmv(teq(babs, 6),&W[6],P); // conditional move
    ecn256cmv(teq(babs, 7),&W[7],P); // conditional move
    ecn256cmv(teq(babs, 8),&W[8],P); // conditional move
    
    ecn256cpy(P,&MP);
    ecn256neg(&MP);  // minus P
    ecn256cmv((int)(m & 1),&MP,P);
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
void ecn256mul(const char *e,point *P) 
{
    int i,j;
    point Q,W[9];
    signed char w[2*Nbytes+1];

    ecn256inf(&Q);
    ecn256inf(&W[0]);                         // O
    ecn256cpy(P,&W[1]);                       // P
    ecn256cpy(P,&W[2]); ecn256dbl(&W[2]);        // 2P
    ecn256cpy(&W[2],&W[3]); ecn256add(P,&W[3]);  // 3P
    ecn256cpy(&W[2],&W[4]); ecn256dbl(&W[4]);    // 4P
    ecn256cpy(&W[4],&W[5]); ecn256add(P,&W[5]);  // 5P
    ecn256cpy(&W[3],&W[6]); ecn256dbl(&W[6]);    // 6P
    ecn256cpy(&W[6],&W[7]); ecn256add(P,&W[7]);  // 7P
    ecn256cpy(&W[4],&W[8]); ecn256dbl(&W[8]);    // 8P

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
        ecn256dbl(P);
        ecn256dbl(P);
        ecn256dbl(P);
        ecn256dbl(P);
        ecn256add(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
void ecn256mul2(const char *e,point *P,const char *f,point *Q,point *R)
{
    int i,j;
    point W[5];
    signed char w[8*Nbytes+8];
    ecn256inf(&W[0]);     // O
    ecn256cpy(P,&W[1]);   // P
    ecn256cpy(Q,&W[3]);   // Q
    ecn256cpy(Q,&W[2]); ecn256sub(P,&W[2]);  // Q-P
    ecn256cpy(Q,&W[4]); ecn256add(P,&W[4]);  // Q+P

    dnaf(e,f,(char *)w);

    i=8*Nbytes+7;
    while (w[i]==0) i--; // ignore leading zeros
    ecn256inf(R);
    while (i>=1)
    {
        ecn256dbl(R);
        j=w[i];
        if (j>0) ecn256add(&W[j],R);
        if (j<0) ecn256sub(&W[-j],R);
        i--;
    }
}


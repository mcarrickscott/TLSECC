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

// Command line : python pseudo.py 32 ED25519

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 9
#define Radix 29
#define Nbits 255
#define Nbytes 32

#define MERSENNE
#define MULBYINT
#define ED25519

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
  n[0] -= ((spint)19u) & carry;
  n[8] += ((spint)0x800000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] += (spint)19u;
  n[8] -= (spint)0x800000u;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static void modadd(const spint *a, const spint *b, spint *n) {
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[5] = a[5] + b[5];
  n[6] = a[6] + b[6];
  n[7] = a[7] + b[7];
  n[8] = a[8] + b[8];
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void modsub(const spint *a, const spint *b, spint *n) {
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  n[5] = a[5] - b[5];
  n[6] = a[6] - b[6];
  n[7] = a[7] - b[7];
  n[8] = a[8] - b[8];
  n[0] -= (spint)76u;
  n[8] += (spint)0x2000000u;
  (void)prop(n);
}

// Modular negation
static void modneg(const spint *b, spint *n) {
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  n[5] = (spint)0 - b[5];
  n[6] = (spint)0 - b[6];
  n[7] = (spint)0 - b[7];
  n[8] = (spint)0 - b[8];
  n[0] -= (spint)76u;
  n[8] += (spint)0x2000000u;
  (void)prop(n);
}

// Modular multiplication by an integer, c=a*b mod 2p
static void modmli(const spint *a, int b, spint *c) {
  udpint t = 0;
  spint carry;
  spint s;
  spint mask = ((spint)1 << 29u) - (spint)1;
  t += (udpint)a[0] * (udpint)b;
  spint v0 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[1] * (udpint)b;
  spint v1 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[2] * (udpint)b;
  spint v2 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[3] * (udpint)b;
  spint v3 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[4] * (udpint)b;
  spint v4 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[5] * (udpint)b;
  spint v5 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[6] * (udpint)b;
  spint v6 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[7] * (udpint)b;
  spint v7 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[8] * (udpint)b;
  spint v8 = (spint)t & mask;
  t = t >> 29u;
  // second reduction pass

  udpint ut = (udpint)t;
  ut = (ut << 6) + (spint)(v8 >> 23u);
  v8 &= 0x7fffff;
  ut *= 0x13;
  s = v0 + ((spint)ut & mask);
  c[0] = (spint)(s & mask);
  carry = (s >> 29) + (spint)(ut >> 29);
  c[1] = v1 + carry;
  c[2] = v2;
  c[3] = v3;
  c[4] = v4;
  c[5] = v5;
  c[6] = v6;
  c[7] = v7;
  c[8] = v8;
}

// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  dpint tt;
  spint lo;
  spint hi;
  spint carry;
  spint s;
  spint mask = ((spint)1 << 29u) - (spint)1;
  tt = (dpint)a[1] * (dpint)b[8];
  tt += (dpint)a[2] * (dpint)b[7];
  tt += (dpint)a[3] * (dpint)b[6];
  tt += (dpint)a[4] * (dpint)b[5];
  tt += (dpint)a[5] * (dpint)b[4];
  tt += (dpint)a[6] * (dpint)b[3];
  tt += (dpint)a[7] * (dpint)b[2];
  tt += (dpint)a[8] * (dpint)b[1];
  lo = (spint)tt & mask;
  t += (dpint)lo * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[0];
  spint v0 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[2] * (dpint)b[8];
  tt += (dpint)a[3] * (dpint)b[7];
  tt += (dpint)a[4] * (dpint)b[6];
  tt += (dpint)a[5] * (dpint)b[5];
  tt += (dpint)a[6] * (dpint)b[4];
  tt += (dpint)a[7] * (dpint)b[3];
  tt += (dpint)a[8] * (dpint)b[2];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[1];
  t += (dpint)a[1] * (dpint)b[0];
  spint v1 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[3] * (dpint)b[8];
  tt += (dpint)a[4] * (dpint)b[7];
  tt += (dpint)a[5] * (dpint)b[6];
  tt += (dpint)a[6] * (dpint)b[5];
  tt += (dpint)a[7] * (dpint)b[4];
  tt += (dpint)a[8] * (dpint)b[3];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[2];
  t += (dpint)a[1] * (dpint)b[1];
  t += (dpint)a[2] * (dpint)b[0];
  spint v2 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[4] * (dpint)b[8];
  tt += (dpint)a[5] * (dpint)b[7];
  tt += (dpint)a[6] * (dpint)b[6];
  tt += (dpint)a[7] * (dpint)b[5];
  tt += (dpint)a[8] * (dpint)b[4];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[3];
  t += (dpint)a[1] * (dpint)b[2];
  t += (dpint)a[2] * (dpint)b[1];
  t += (dpint)a[3] * (dpint)b[0];
  spint v3 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[5] * (dpint)b[8];
  tt += (dpint)a[6] * (dpint)b[7];
  tt += (dpint)a[7] * (dpint)b[6];
  tt += (dpint)a[8] * (dpint)b[5];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[4];
  t += (dpint)a[1] * (dpint)b[3];
  t += (dpint)a[2] * (dpint)b[2];
  t += (dpint)a[3] * (dpint)b[1];
  t += (dpint)a[4] * (dpint)b[0];
  spint v4 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[6] * (dpint)b[8];
  tt += (dpint)a[7] * (dpint)b[7];
  tt += (dpint)a[8] * (dpint)b[6];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[5];
  t += (dpint)a[1] * (dpint)b[4];
  t += (dpint)a[2] * (dpint)b[3];
  t += (dpint)a[3] * (dpint)b[2];
  t += (dpint)a[4] * (dpint)b[1];
  t += (dpint)a[5] * (dpint)b[0];
  spint v5 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[7] * (dpint)b[8];
  tt += (dpint)a[8] * (dpint)b[7];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[6];
  t += (dpint)a[1] * (dpint)b[5];
  t += (dpint)a[2] * (dpint)b[4];
  t += (dpint)a[3] * (dpint)b[3];
  t += (dpint)a[4] * (dpint)b[2];
  t += (dpint)a[5] * (dpint)b[1];
  t += (dpint)a[6] * (dpint)b[0];
  spint v6 = (spint)t & mask;
  t = t >> 29u;
  tt = (dpint)a[8] * (dpint)b[8];
  lo = (spint)tt & mask;
  t += (dpint)(spint)(lo + hi) * (dpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += (dpint)a[0] * (dpint)b[7];
  t += (dpint)a[1] * (dpint)b[6];
  t += (dpint)a[2] * (dpint)b[5];
  t += (dpint)a[3] * (dpint)b[4];
  t += (dpint)a[4] * (dpint)b[3];
  t += (dpint)a[5] * (dpint)b[2];
  t += (dpint)a[6] * (dpint)b[1];
  t += (dpint)a[7] * (dpint)b[0];
  spint v7 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[0] * (dpint)b[8];
  t += (dpint)a[1] * (dpint)b[7];
  t += (dpint)a[2] * (dpint)b[6];
  t += (dpint)a[3] * (dpint)b[5];
  t += (dpint)a[4] * (dpint)b[4];
  t += (dpint)a[5] * (dpint)b[3];
  t += (dpint)a[6] * (dpint)b[2];
  t += (dpint)a[7] * (dpint)b[1];
  t += (dpint)a[8] * (dpint)b[0];
  t += (dpint)hi * (dpint)0x4c0;
  spint v8 = (spint)t & mask;
  t = t >> 29u;
  // second reduction pass

  udpint ut = (udpint)t;
  ut = (ut << 6) + (spint)(v8 >> 23u);
  v8 &= 0x7fffff;
  ut *= 0x13;
  s = v0 + ((spint)ut & mask);
  c[0] = (spint)(s & mask);
  carry = (s >> 29) + (spint)(ut >> 29);
  c[1] = v1 + carry;
  c[2] = v2;
  c[3] = v3;
  c[4] = v4;
  c[5] = v5;
  c[6] = v6;
  c[7] = v7;
  c[8] = v8;
}

// Modular squaring, c=a*a mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint t = 0;
  udpint tt;
  udpint t2;
  spint carry;
  spint s;
  spint mask = ((spint)1 << 29u) - (spint)1;
  spint lo;
  spint hi;
  tt = (udpint)a[1] * (udpint)a[8];
  tt += (udpint)a[2] * (udpint)a[7];
  tt += (udpint)a[3] * (udpint)a[6];
  tt += (udpint)a[4] * (udpint)a[5];
  tt *= 2;
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[0];
  t2 += (udpint)lo * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v0 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[2] * (udpint)a[8];
  tt += (udpint)a[3] * (udpint)a[7];
  tt += (udpint)a[4] * (udpint)a[6];
  tt *= 2;
  tt += (udpint)a[5] * (udpint)a[5];
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[1];
  t2 *= 2;
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v1 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[3] * (udpint)a[8];
  tt += (udpint)a[4] * (udpint)a[7];
  tt += (udpint)a[5] * (udpint)a[6];
  tt *= 2;
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[2];
  t2 *= 2;
  t2 += (udpint)a[1] * (udpint)a[1];
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v2 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[4] * (udpint)a[8];
  tt += (udpint)a[5] * (udpint)a[7];
  tt *= 2;
  tt += (udpint)a[6] * (udpint)a[6];
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[3];
  t2 += (udpint)a[1] * (udpint)a[2];
  t2 *= 2;
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v3 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[5] * (udpint)a[8];
  tt += (udpint)a[6] * (udpint)a[7];
  tt *= 2;
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[4];
  t2 += (udpint)a[1] * (udpint)a[3];
  t2 *= 2;
  t2 += (udpint)a[2] * (udpint)a[2];
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v4 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[6] * (udpint)a[8];
  tt *= 2;
  tt += (udpint)a[7] * (udpint)a[7];
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[5];
  t2 += (udpint)a[1] * (udpint)a[4];
  t2 += (udpint)a[2] * (udpint)a[3];
  t2 *= 2;
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v5 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[7] * (udpint)a[8];
  tt *= 2;
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[6];
  t2 += (udpint)a[1] * (udpint)a[5];
  t2 += (udpint)a[2] * (udpint)a[4];
  t2 *= 2;
  t2 += (udpint)a[3] * (udpint)a[3];
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v6 = (spint)t & mask;
  t = t >> 29u;
  tt = (udpint)a[8] * (udpint)a[8];
  lo = (spint)tt & mask;
  t2 = (udpint)a[0] * (udpint)a[7];
  t2 += (udpint)a[1] * (udpint)a[6];
  t2 += (udpint)a[2] * (udpint)a[5];
  t2 += (udpint)a[3] * (udpint)a[4];
  t2 *= 2;
  t2 += (udpint)(spint)(lo + hi) * (udpint)0x4c0;
  hi = (spint)(tt >> 29u);
  t += t2;
  spint v7 = (spint)t & mask;
  t = t >> 29u;
  t2 = (udpint)a[0] * (udpint)a[8];
  t2 += (udpint)a[1] * (udpint)a[7];
  t2 += (udpint)a[2] * (udpint)a[6];
  t2 += (udpint)a[3] * (udpint)a[5];
  t2 *= 2;
  t2 += (udpint)a[4] * (udpint)a[4];
  t += (udpint)hi * (udpint)0x4c0;
  t += t2;
  spint v8 = (spint)t & mask;
  t = t >> 29u;
  // second reduction pass

  udpint ut = (udpint)t;
  ut = (ut << 6) + (spint)(v8 >> 23u);
  v8 &= 0x7fffff;
  ut *= 0x13;
  s = v0 + ((spint)ut & mask);
  c[0] = (spint)(s & mask);
  carry = (s >> 29) + (spint)(ut >> 29);
  c[1] = v1 + carry;
  c[2] = v2;
  c[3] = v3;
  c[4] = v4;
  c[5] = v5;
  c[6] = v6;
  c[7] = v7;
  c[8] = v8;
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

// Calculate progenitor - use optimal addition chain
static void modpro(const spint *w, spint *z) {
  spint x[9];
  spint t0[9];
  spint t1[9];
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modcpy(z, t0);
  modnsqr(t0, 2);
  modmul(z, t0, z);
  modsqr(z, z);
  modmul(x, z, t0);
  modcpy(t0, z);
  modnsqr(z, 5);
  modmul(t0, z, z);
  modcpy(z, t1);
  modnsqr(t1, 5);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 15);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 30);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 60);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 120);
  modmul(t0, t1, t0);
  modnsqr(t0, 10);
  modmul(z, t0, z);
  modnsqr(z, 2);
  modmul(x, z, z);
}

// calculate inverse, provide progenitor h if available, NULL if not
static void modinv(const spint *x, const spint *h, spint *z) {
  int i;
  spint s[9];
  spint t[9];
  if (h == NULL) {
    modpro(x, t);
  } else {
    modcpy(h, t);
  }
  modcpy(x, s);
  for (i = 0; i < 1; i++) {
    modsqr(s, s);
    modmul(s, x, s);
  }
  modnsqr(t, 3);
  modmul(s, t, z);
}

// Convert m to internal form, n=nres(m)
static void nres(const spint *m, spint *n) {
  int i;
  for (i = 0; i < 9; i++) {
    n[i] = m[i];
  }
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  for (i = 0; i < 9; i++) {
    m[i] = n[i];
  }
  (void)modfsb(m);
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

// Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr(const spint *h, const spint *x) {
  spint r[9];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
  modnsqr(r, 1);
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
  for (i = 0; i < 9; i++) {
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
  for (i = 0; i < 9; i++) {
      t=f[i]; s=g[i];
      f[i] =c0*t+c1*s;
      g[i] =c0*s+c1*t;
      f[i]-=r0*t+r1*s;
      g[i]-=r0*s+r1*t;
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  int k;
  spint t[9];
  spint b[9];
  spint v[9];
  spint z[9] = {0xa0ea0b0, 0x770d93a,  0xbf91e31,  0x6300d5a, 0x1d7a72f4,
                0x4c9efd,  0x1c2cad34, 0x1009f83b, 0x2b8324};
  spint s[9];
  spint y[9];
  if (h == NULL) {
    modpro(x, y);
  } else {
    modcpy(h, y);
  }
  modmul(y, x, s);
  modmul(s, y, t);
  nres(z, z);
  for (k = 2; k > 1; k--) {
    modcpy(t, b);
    modnsqr(b, k - 2);
    int d = 1 - modis1(b);
    modmul(s, z, v);
    modcmv(d, v, s);
    modsqr(z, z);
    modmul(t, z, v);
    modcmv(d, v, t);
  }
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

#include "Ed25519curve.h"

#define BYTES Nbytes
#define LIMBS Nlimbs
#define TOPBIT (8*sizeof(int)-1)

/*** Insert automatically generated curve definition curve.c here ***/



#define COF 3
#define CONSTANT_A -1
static const spint constant_b[9]={0x135978a3,0xf5a6e50,0x10762add,0x149a82,0x1e898007,0x3cbbbc,0x19ce331d,0x1dc56dff,0x52036c};
static const spint constant_x[9]={0xf25d51a,0xab16b04,0x969ecb2,0x198ec12a,0xdc5c692,0x1118feeb,0xffb0293,0x1a79adca,0x216936};
static const spint constant_y[9]={0x6666658,0x13333333,0x19999999,0xccccccc,0x6666666,0x13333333,0x19999999,0xccccccc,0x666666};

/*** End of automatically generated code ***/


// return 1 if b==c, no branching 
static int teq(int b, int c)
{
    int x = b ^ c;
    x -= 1; // if x=0, x now -1
    return ((x >> TOPBIT) & 1);
}

// copy point
void ecn25519cpy(point *Q,point *P)
{
    modcpy(Q->x,P->x);
    modcpy(Q->y,P->y);
    modcpy(Q->z,P->z);
}

// negate P
void ecn25519neg(point *P)
{
    modneg(P->x,P->x);
}

// add Q to P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
void ecn25519add(point *Q,point *P)
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
void ecn25519sub(point *Q,point *P)
{
    point W;
    ecn25519cpy(Q,&W); ecn25519neg(&W);
    ecn25519add(&W,P);
}

// double P
// standard projective method from EFD - https://www.hyperelliptic.org/EFD/
void ecn25519dbl(point *P)
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
void ecn25519inf(point *P)
{
    modzer(P->x);
    modone(P->y);
    modone(P->z);
}

// test for infinity
int ecn25519isinf(point *P)
{
    return (modis0(P->x) && modcmp(P->y,P->z));
}

// set to affine
void ecn25519affine(point *P)
{
    spint I[Nlimbs];
    if (modis0(P->z)) {
        ecn25519inf(P);
        return;
    }
    modinv(P->z,NULL,I);
    modone(P->z);
    modmul(P->x,I,P->x);
    modmul(P->y,I,P->y);
}

// move Q to P if d=1
void ecn25519cmv(int d,point *Q,point *P)
{
    modcmv(d,Q->x,P->x);
    modcmv(d,Q->y,P->y);
    modcmv(d,Q->z,P->z);
}

// return 1 if equal, else 0
int ecn25519cmp(point *P,point *Q)
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
int ecn25519get(point *P,char *x,char *y)
{
    spint X[Nlimbs],Y[Nlimbs];
    ecn25519affine(P);
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
            ecn25519inf(P);
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
        ecn25519inf(P);
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
void ecn25519cof(point *P)
{
    int i;
    for (i=0;i<COF;i++)
        ecn25519dbl(P);
}

// Is (x,y) of the right order? Must be checked by calling program!
// api visible version, x and y are big endian byte arrays
void ecn25519set(int s,const char *x,const char *y,point *P)
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
void ecn25519gen(point *P)
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

    ecn25519cmv(teq(babs, 0),&W[0],P); // conditional move
    ecn25519cmv(teq(babs, 1),&W[1],P);
    ecn25519cmv(teq(babs, 2),&W[2],P);    
    ecn25519cmv(teq(babs, 3),&W[3],P);
    ecn25519cmv(teq(babs, 4),&W[4],P);
    ecn25519cmv(teq(babs, 5),&W[5],P);
    ecn25519cmv(teq(babs, 6),&W[6],P);    
    ecn25519cmv(teq(babs, 7),&W[7],P);
    ecn25519cmv(teq(babs, 8),&W[8],P);

    ecn25519cpy(P,&MP);
    ecn25519neg(&MP);  // minus P
    ecn25519cmv((int)(m & 1),&MP,P);
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
void ecn25519mul(const char *e,point *P) 
{
    int i,j;
    point Q,W[9];
    signed char w[2*Nbytes+1];

    ecn25519inf(&Q);
    ecn25519inf(&W[0]);                         // O
    ecn25519cpy(P,&W[1]);                       // P
    ecn25519cpy(P,&W[2]); ecn25519dbl(&W[2]);        // 2P
    ecn25519cpy(&W[2],&W[3]); ecn25519add(P,&W[3]);  // 3P
    ecn25519cpy(&W[2],&W[4]); ecn25519dbl(&W[4]);    // 4P
    ecn25519cpy(&W[4],&W[5]); ecn25519add(P,&W[5]);  // 5P
    ecn25519cpy(&W[3],&W[6]); ecn25519dbl(&W[6]);    // 6P
    ecn25519cpy(&W[6],&W[7]); ecn25519add(P,&W[7]);  // 7P
    ecn25519cpy(&W[4],&W[8]); ecn25519dbl(&W[8]);    // 8P

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
        ecn25519dbl(P);
        ecn25519dbl(P);
        ecn25519dbl(P);
        ecn25519dbl(P);
        ecn25519add(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
void ecn25519mul2(const char *e,point *P,const char *f,point *Q,point *R)
{
    int i,j;
    point W[5];
    signed char w[8*Nbytes+8];
    ecn25519inf(&W[0]);     // O
    ecn25519cpy(P,&W[1]);   // P
    ecn25519cpy(Q,&W[3]);   // Q
    ecn25519cpy(Q,&W[2]); ecn25519sub(P,&W[2]);  // Q-P
    ecn25519cpy(Q,&W[4]); ecn25519add(P,&W[4]);  // Q+P

    dnaf(e,f,w);

    i=8*Nbytes+7;
    while (w[i]==0) i--; // ignore leading zeros
    ecn25519inf(R);
    while (i>=1)
    {
        ecn25519dbl(R);
        j=w[i];
        if (j>0) ecn25519add(&W[j],R);
        if (j<0) ecn25519sub(&W[-j],R);
        i--;
    }
}

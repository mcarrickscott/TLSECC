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


// Command line : python monty.py 64 NIST256

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
  n[0] -= (spint)1u & carry;
  n[1] += ((spint)0x100000000000u) & carry;
  n[3] += ((spint)0x1000000000u) & carry;
  n[4] += ((spint)0xffffffff0000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int inline modfsb(spint *n) {
  n[0] += (spint)1u;
  n[1] -= (spint)0x100000000000u;
  n[3] -= (spint)0x1000000000u;
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
  n[0] += (spint)2u;
  n[1] -= (spint)0x200000000000u;
  n[3] -= (spint)0x2000000000u;
  n[4] -= (spint)0x1fffffffe0000u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[1] += ((spint)0x200000000000u) & carry;
  n[3] += ((spint)0x2000000000u) & carry;
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
  n[0] -= (spint)2u & carry;
  n[1] += ((spint)0x200000000000u) & carry;
  n[3] += ((spint)0x2000000000u) & carry;
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
  n[0] -= (spint)2u & carry;
  n[1] += ((spint)0x200000000000u) & carry;
  n[3] += ((spint)0x2000000000u) & carry;
  n[4] += ((spint)0x1fffffffe0000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 102759236341273337689498043088901
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p4 = 0xffffffff0000u;
  spint q = ((spint)1 << 52u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  t += (dpint)(udpint)((udpint)v0 << 44u);
  spint v1 = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)(udpint)((udpint)v1 << 44u);
  spint v2 = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)(udpint)((udpint)v0 << 36u);
  t += (dpint)(udpint)((udpint)v2 << 44u);
  spint v3 = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)v0 * (dpint)p4;
  t += (dpint)(udpint)((udpint)v1 << 36u);
  t += (dpint)(udpint)((udpint)v3 << 44u);
  spint v4 = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)v1 * (dpint)p4;
  t += (dpint)(udpint)((udpint)v2 << 36u);
  t += (dpint)(udpint)((udpint)v4 << 44u);
  c[0] = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)(udpint)((udpint)v3 << 36u);
  c[1] = ((spint)t & mask);
  t >>= 52;
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)(udpint)((udpint)v4 << 36u);
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
  spint p4 = 0xffffffff0000u;
  spint q = ((spint)1 << 52u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  t += (udpint)v0 << 44u;
  spint v1 = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v1 << 44u;
  spint v2 = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 << 36u;
  t += (udpint)v2 << 44u;
  spint v3 = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v0 * p4;
  t += (udpint)v1 << 36u;
  t += (udpint)v3 << 44u;
  spint v4 = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p4;
  t += (udpint)v2 << 36u;
  t += (udpint)v4 << 44u;
  c[0] = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v2 * p4;
  t += (udpint)v3 << 36u;
  c[1] = ((spint)t & mask);
  t >>= 52;
  tot = (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v3 * p4;
  t += (udpint)v4 << 36u;
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
  spint s[5];
  spint t[5];
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
  const spint c[5] = {0x300u, 0xffffffff00000u, 0xffffefffffffbu,
                      0xfdfffffffffffu, 0x4ffffffu};
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
  spint s[5];
  spint y[5];
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

#include "nist256curve.h"

#define BYTES Nbytes
#define LIMBS Nlimbs
#define TOPBIT (8*sizeof(int)-1)

/*** Insert automatically generated curve definition curve.c here ***/


#define COF 1
#define CONSTANT_A -3
const spint constant_b[5]={0xdf6229c4bddfd,0xca8843090d89c,0x212ed6acf005c,0x83415a220abf7,0xc30061dd4874};
const spint constant_b3[5]={0x9e267d4e399f9,0x5d98c91b289d6,0x638c8406d0116,0x89c20e66203e5,0x49012599d95d};
const spint constant_x[5]={0x30d418a9143c1,0xc4fedb60179e7,0x62251075ba95f,0x5c669fb732b77,0x8905f76b5375};
const spint constant_y[5]={0x5357ce95560a8,0x43a19e45cddf2,0x21f3258b4ab8e,0xd8552e88688dd,0x571ff18a5885};



/*** End of automatically generated code ***/

// return 1 if b==c, no branching 
static int teq(int b, int c)
{
    int x = b ^ c;
    x -= 1; // if x=0, x now -1
    return ((x >> TOPBIT) & 1);
}

// copy point
void ecncpy(point *Q,point *P)
{
    modcpy(Q->x,P->x);
    modcpy(Q->y,P->y);
    modcpy(Q->z,P->z);
}

// negate P
void ecnneg(point *P)
{
    modneg(P->y,P->y);
}

// add Q to P
// complete formulae from https://eprint.iacr.org/2015/1060
void ecnadd(point *Q,point *P)
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
void ecnsub(point *Q,point *P)
{
    point W;
    ecncpy(Q,&W); ecnneg(&W);
    ecnadd(&W,P);
}

// double P
// complete formuale from https://eprint.iacr.org/2015/1060
void ecndbl(point *P)
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
void ecninf(point *P)
{
    modzer(P->x);
    modone(P->y);
    modzer(P->z);
}

// test for infinity
int ecnisinf(point *P)
{
    return (modis0(P->x) && modis0(P->z));
}

// set to affine
void ecnaffine(point *P)
{
    spint I[Nlimbs];
    modinv(P->z,NULL,I);
    modone(P->z);
    modmul(P->x,I,P->x);
    modmul(P->y,I,P->y);
}

// move Q to P if d=1
void ecncmv(int d,point *Q,point *P)
{
    modcmv(d,Q->x,P->x);
    modcmv(d,Q->y,P->y);
    modcmv(d,Q->z,P->z);
}

// return 1 if equal, else 0
int ecncmp(point *P,point *Q)
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
int ecnget(point *P,char *x,char *y)
{
    spint X[Nlimbs],Y[Nlimbs];
    ecnaffine(P);

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
int ecngetxyz(point *P,char *x,char *y,char *z)
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
            ecninf(P);
        }
        return;
    }
    modpro(V,H);
    if (!modqr(H,V))
    { // point not on curve
        ecninf(P);
        return;
    }
    modsqrt(V,H,P->y);
    int d=(modsign(P->y)-s)&1;
    modneg(P->y,T);
    modcmv(d,T,P->y);
    modone(P->z);
}    

// multiply point by small curve cofactor (here assumed to be 1)
void ecncof(point *P)
{}

// api visible version, x and y are big endian byte arrays
void ecnset(int s,const char *x,const char *y,point *P)
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
void ecngen(point *P)
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

    ecncmv(teq(babs, 0),&W[0],P); // conditional move
    ecncmv(teq(babs, 1),&W[1],P); // conditional move
    ecncmv(teq(babs, 2),&W[2],P); // conditional move
    ecncmv(teq(babs, 3),&W[3],P); // conditional move
    ecncmv(teq(babs, 4),&W[4],P); // conditional move
    ecncmv(teq(babs, 5),&W[5],P); // conditional move
    ecncmv(teq(babs, 6),&W[6],P); // conditional move
    ecncmv(teq(babs, 7),&W[7],P); // conditional move
    ecncmv(teq(babs, 8),&W[8],P); // conditional move
    
    ecncpy(P,&MP);
    ecnneg(&MP);  // minus P
    ecncmv((int)(m & 1),&MP,P);
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
void ecnmul(const char *e,point *P) 
{
    int i,j;
    point Q,W[9];
    signed char w[2*Nbytes+1];

    ecninf(&W[0]);                         // O
    ecncpy(P,&W[1]);                       // P
    ecncpy(P,&W[2]); ecndbl(&W[2]);        // 2P
    ecncpy(&W[2],&W[3]); ecnadd(P,&W[3]);  // 3P
    ecncpy(&W[2],&W[4]); ecndbl(&W[4]);    // 4P
    ecncpy(&W[4],&W[5]); ecnadd(P,&W[5]);  // 5P
    ecncpy(&W[3],&W[6]); ecndbl(&W[6]);    // 6P
    ecncpy(&W[6],&W[7]); ecnadd(P,&W[7]);  // 7P
    ecncpy(&W[4],&W[8]); ecndbl(&W[8]);    // 8P

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
        ecndbl(P);
        ecndbl(P);
        ecndbl(P);
        ecndbl(P);
        ecnadd(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
void ecnmul2(const char *e,point *P,const char *f,point *Q,point *R)
{
    int i;
    point T,W[5];
    signed char w[8*Nbytes+8];
    ecninf(&W[0]);     // O
    ecncpy(P,&W[1]);   // P
    ecncpy(Q,&W[3]);   // Q
    ecncpy(Q,&W[2]); ecnsub(P,&W[2]);  // Q-P
    ecncpy(Q,&W[4]); ecnadd(P,&W[4]);  // Q+P

    dnaf(e,f,(char *)w);

    i=8*Nbytes+7;
    while (w[i]==0) i--; // ignore leading zeros
    ecninf(R);
    while (i>=1)
    {
        ecndbl(R);
        if (w[i]!=0) {
            select(w[i],W,&T);
            ecnadd(&T,R);
        }
        i--;
    }
}


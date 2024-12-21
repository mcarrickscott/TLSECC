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
// python curve.py 32/64 NIST521
//

// make sure decoration and generic are both set to False in monty.py or pseudo.py

/*** Insert automatically generated code for modulus field.c here ***/


// Command line : python pseudo.py 32 NIST521

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 18
#define Radix 29
#define Nbits 521
#define Nbytes 66

#define MERSENNE
#define MULBYINT
#define NIST521

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 29u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 29u;
  n[0] &= mask;
  for (i = 1; i < 17; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 29u;
  }
  n[17] += (spint)carry;
  return -((n[17] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static int inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= ((spint)1u) & carry;
  n[17] += ((spint)0x10000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[17] -= (spint)0x10000000u;
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
  n[9] = a[9] + b[9];
  n[10] = a[10] + b[10];
  n[11] = a[11] + b[11];
  n[12] = a[12] + b[12];
  n[13] = a[13] + b[13];
  n[14] = a[14] + b[14];
  n[15] = a[15] + b[15];
  n[16] = a[16] + b[16];
  n[17] = a[17] + b[17];
  n[0] += (spint)2u;
  n[17] -= (spint)0x20000000u;
  carry = prop(n);
  n[0] -= ((spint)2u) & carry;
  n[17] += ((spint)0x20000000u) & carry;
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
  n[9] = a[9] - b[9];
  n[10] = a[10] - b[10];
  n[11] = a[11] - b[11];
  n[12] = a[12] - b[12];
  n[13] = a[13] - b[13];
  n[14] = a[14] - b[14];
  n[15] = a[15] - b[15];
  n[16] = a[16] - b[16];
  n[17] = a[17] - b[17];
  carry = prop(n);
  n[0] -= ((spint)2u) & carry;
  n[17] += ((spint)0x20000000u) & carry;
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
  n[9] = (spint)0 - b[9];
  n[10] = (spint)0 - b[10];
  n[11] = (spint)0 - b[11];
  n[12] = (spint)0 - b[12];
  n[13] = (spint)0 - b[13];
  n[14] = (spint)0 - b[14];
  n[15] = (spint)0 - b[15];
  n[16] = (spint)0 - b[16];
  n[17] = (spint)0 - b[17];
  carry = prop(n);
  n[0] -= ((spint)2u) & carry;
  n[17] += ((spint)0x20000000u) & carry;
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
  t += (udpint)a[9] * (udpint)b;
  spint v9 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[10] * (udpint)b;
  spint v10 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[11] * (udpint)b;
  spint v11 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[12] * (udpint)b;
  spint v12 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[13] * (udpint)b;
  spint v13 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[14] * (udpint)b;
  spint v14 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[15] * (udpint)b;
  spint v15 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[16] * (udpint)b;
  spint v16 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[17] * (udpint)b;
  spint v17 = (spint)t & mask;
  t = t >> 29u;
  // second reduction pass

  udpint ut = (udpint)t;
  ut = (ut << 1) + (spint)(v17 >> 28u);
  v17 &= 0xfffffff;
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
  c[9] = v9;
  c[10] = v10;
  c[11] = v11;
  c[12] = v12;
  c[13] = v13;
  c[14] = v14;
  c[15] = v15;
  c[16] = v16;
  c[17] = v17;
}

// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint ma1 = a[1] * (spint)0x2;
  spint ma2 = a[2] * (spint)0x2;
  spint ma3 = a[3] * (spint)0x2;
  spint ma4 = a[4] * (spint)0x2;
  spint ma5 = a[5] * (spint)0x2;
  spint ma6 = a[6] * (spint)0x2;
  spint ma7 = a[7] * (spint)0x2;
  spint ma8 = a[8] * (spint)0x2;
  spint ma9 = a[9] * (spint)0x2;
  spint ma10 = a[10] * (spint)0x2;
  spint ma11 = a[11] * (spint)0x2;
  spint ma12 = a[12] * (spint)0x2;
  spint ma13 = a[13] * (spint)0x2;
  spint ma14 = a[14] * (spint)0x2;
  spint ma15 = a[15] * (spint)0x2;
  spint ma16 = a[16] * (spint)0x2;
  spint ma17 = a[17] * (spint)0x2;
  spint carry;
  spint s;
  spint mask = ((spint)1 << 29u) - (spint)1;
  t += (dpint)ma1 * (dpint)b[17];
  t += (dpint)ma2 * (dpint)b[16];
  t += (dpint)ma3 * (dpint)b[15];
  t += (dpint)ma4 * (dpint)b[14];
  t += (dpint)ma5 * (dpint)b[13];
  t += (dpint)ma6 * (dpint)b[12];
  t += (dpint)ma7 * (dpint)b[11];
  t += (dpint)ma8 * (dpint)b[10];
  t += (dpint)ma9 * (dpint)b[9];
  t += (dpint)ma10 * (dpint)b[8];
  t += (dpint)ma11 * (dpint)b[7];
  t += (dpint)ma12 * (dpint)b[6];
  t += (dpint)ma13 * (dpint)b[5];
  t += (dpint)ma14 * (dpint)b[4];
  t += (dpint)ma15 * (dpint)b[3];
  t += (dpint)ma16 * (dpint)b[2];
  t += (dpint)ma17 * (dpint)b[1];
  t += (dpint)a[0] * (dpint)b[0];
  spint v0 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma2 * (dpint)b[17];
  t += (dpint)ma3 * (dpint)b[16];
  t += (dpint)ma4 * (dpint)b[15];
  t += (dpint)ma5 * (dpint)b[14];
  t += (dpint)ma6 * (dpint)b[13];
  t += (dpint)ma7 * (dpint)b[12];
  t += (dpint)ma8 * (dpint)b[11];
  t += (dpint)ma9 * (dpint)b[10];
  t += (dpint)ma10 * (dpint)b[9];
  t += (dpint)ma11 * (dpint)b[8];
  t += (dpint)ma12 * (dpint)b[7];
  t += (dpint)ma13 * (dpint)b[6];
  t += (dpint)ma14 * (dpint)b[5];
  t += (dpint)ma15 * (dpint)b[4];
  t += (dpint)ma16 * (dpint)b[3];
  t += (dpint)ma17 * (dpint)b[2];
  t += (dpint)a[0] * (dpint)b[1];
  t += (dpint)a[1] * (dpint)b[0];
  spint v1 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma3 * (dpint)b[17];
  t += (dpint)ma4 * (dpint)b[16];
  t += (dpint)ma5 * (dpint)b[15];
  t += (dpint)ma6 * (dpint)b[14];
  t += (dpint)ma7 * (dpint)b[13];
  t += (dpint)ma8 * (dpint)b[12];
  t += (dpint)ma9 * (dpint)b[11];
  t += (dpint)ma10 * (dpint)b[10];
  t += (dpint)ma11 * (dpint)b[9];
  t += (dpint)ma12 * (dpint)b[8];
  t += (dpint)ma13 * (dpint)b[7];
  t += (dpint)ma14 * (dpint)b[6];
  t += (dpint)ma15 * (dpint)b[5];
  t += (dpint)ma16 * (dpint)b[4];
  t += (dpint)ma17 * (dpint)b[3];
  t += (dpint)a[0] * (dpint)b[2];
  t += (dpint)a[1] * (dpint)b[1];
  t += (dpint)a[2] * (dpint)b[0];
  spint v2 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma4 * (dpint)b[17];
  t += (dpint)ma5 * (dpint)b[16];
  t += (dpint)ma6 * (dpint)b[15];
  t += (dpint)ma7 * (dpint)b[14];
  t += (dpint)ma8 * (dpint)b[13];
  t += (dpint)ma9 * (dpint)b[12];
  t += (dpint)ma10 * (dpint)b[11];
  t += (dpint)ma11 * (dpint)b[10];
  t += (dpint)ma12 * (dpint)b[9];
  t += (dpint)ma13 * (dpint)b[8];
  t += (dpint)ma14 * (dpint)b[7];
  t += (dpint)ma15 * (dpint)b[6];
  t += (dpint)ma16 * (dpint)b[5];
  t += (dpint)ma17 * (dpint)b[4];
  t += (dpint)a[0] * (dpint)b[3];
  t += (dpint)a[1] * (dpint)b[2];
  t += (dpint)a[2] * (dpint)b[1];
  t += (dpint)a[3] * (dpint)b[0];
  spint v3 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma5 * (dpint)b[17];
  t += (dpint)ma6 * (dpint)b[16];
  t += (dpint)ma7 * (dpint)b[15];
  t += (dpint)ma8 * (dpint)b[14];
  t += (dpint)ma9 * (dpint)b[13];
  t += (dpint)ma10 * (dpint)b[12];
  t += (dpint)ma11 * (dpint)b[11];
  t += (dpint)ma12 * (dpint)b[10];
  t += (dpint)ma13 * (dpint)b[9];
  t += (dpint)ma14 * (dpint)b[8];
  t += (dpint)ma15 * (dpint)b[7];
  t += (dpint)ma16 * (dpint)b[6];
  t += (dpint)ma17 * (dpint)b[5];
  t += (dpint)a[0] * (dpint)b[4];
  t += (dpint)a[1] * (dpint)b[3];
  t += (dpint)a[2] * (dpint)b[2];
  t += (dpint)a[3] * (dpint)b[1];
  t += (dpint)a[4] * (dpint)b[0];
  spint v4 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma6 * (dpint)b[17];
  t += (dpint)ma7 * (dpint)b[16];
  t += (dpint)ma8 * (dpint)b[15];
  t += (dpint)ma9 * (dpint)b[14];
  t += (dpint)ma10 * (dpint)b[13];
  t += (dpint)ma11 * (dpint)b[12];
  t += (dpint)ma12 * (dpint)b[11];
  t += (dpint)ma13 * (dpint)b[10];
  t += (dpint)ma14 * (dpint)b[9];
  t += (dpint)ma15 * (dpint)b[8];
  t += (dpint)ma16 * (dpint)b[7];
  t += (dpint)ma17 * (dpint)b[6];
  t += (dpint)a[0] * (dpint)b[5];
  t += (dpint)a[1] * (dpint)b[4];
  t += (dpint)a[2] * (dpint)b[3];
  t += (dpint)a[3] * (dpint)b[2];
  t += (dpint)a[4] * (dpint)b[1];
  t += (dpint)a[5] * (dpint)b[0];
  spint v5 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma7 * (dpint)b[17];
  t += (dpint)ma8 * (dpint)b[16];
  t += (dpint)ma9 * (dpint)b[15];
  t += (dpint)ma10 * (dpint)b[14];
  t += (dpint)ma11 * (dpint)b[13];
  t += (dpint)ma12 * (dpint)b[12];
  t += (dpint)ma13 * (dpint)b[11];
  t += (dpint)ma14 * (dpint)b[10];
  t += (dpint)ma15 * (dpint)b[9];
  t += (dpint)ma16 * (dpint)b[8];
  t += (dpint)ma17 * (dpint)b[7];
  t += (dpint)a[0] * (dpint)b[6];
  t += (dpint)a[1] * (dpint)b[5];
  t += (dpint)a[2] * (dpint)b[4];
  t += (dpint)a[3] * (dpint)b[3];
  t += (dpint)a[4] * (dpint)b[2];
  t += (dpint)a[5] * (dpint)b[1];
  t += (dpint)a[6] * (dpint)b[0];
  spint v6 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma8 * (dpint)b[17];
  t += (dpint)ma9 * (dpint)b[16];
  t += (dpint)ma10 * (dpint)b[15];
  t += (dpint)ma11 * (dpint)b[14];
  t += (dpint)ma12 * (dpint)b[13];
  t += (dpint)ma13 * (dpint)b[12];
  t += (dpint)ma14 * (dpint)b[11];
  t += (dpint)ma15 * (dpint)b[10];
  t += (dpint)ma16 * (dpint)b[9];
  t += (dpint)ma17 * (dpint)b[8];
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
  t += (dpint)ma9 * (dpint)b[17];
  t += (dpint)ma10 * (dpint)b[16];
  t += (dpint)ma11 * (dpint)b[15];
  t += (dpint)ma12 * (dpint)b[14];
  t += (dpint)ma13 * (dpint)b[13];
  t += (dpint)ma14 * (dpint)b[12];
  t += (dpint)ma15 * (dpint)b[11];
  t += (dpint)ma16 * (dpint)b[10];
  t += (dpint)ma17 * (dpint)b[9];
  t += (dpint)a[0] * (dpint)b[8];
  t += (dpint)a[1] * (dpint)b[7];
  t += (dpint)a[2] * (dpint)b[6];
  t += (dpint)a[3] * (dpint)b[5];
  t += (dpint)a[4] * (dpint)b[4];
  t += (dpint)a[5] * (dpint)b[3];
  t += (dpint)a[6] * (dpint)b[2];
  t += (dpint)a[7] * (dpint)b[1];
  t += (dpint)a[8] * (dpint)b[0];
  spint v8 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma10 * (dpint)b[17];
  t += (dpint)ma11 * (dpint)b[16];
  t += (dpint)ma12 * (dpint)b[15];
  t += (dpint)ma13 * (dpint)b[14];
  t += (dpint)ma14 * (dpint)b[13];
  t += (dpint)ma15 * (dpint)b[12];
  t += (dpint)ma16 * (dpint)b[11];
  t += (dpint)ma17 * (dpint)b[10];
  t += (dpint)a[0] * (dpint)b[9];
  t += (dpint)a[1] * (dpint)b[8];
  t += (dpint)a[2] * (dpint)b[7];
  t += (dpint)a[3] * (dpint)b[6];
  t += (dpint)a[4] * (dpint)b[5];
  t += (dpint)a[5] * (dpint)b[4];
  t += (dpint)a[6] * (dpint)b[3];
  t += (dpint)a[7] * (dpint)b[2];
  t += (dpint)a[8] * (dpint)b[1];
  t += (dpint)a[9] * (dpint)b[0];
  spint v9 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma11 * (dpint)b[17];
  t += (dpint)ma12 * (dpint)b[16];
  t += (dpint)ma13 * (dpint)b[15];
  t += (dpint)ma14 * (dpint)b[14];
  t += (dpint)ma15 * (dpint)b[13];
  t += (dpint)ma16 * (dpint)b[12];
  t += (dpint)ma17 * (dpint)b[11];
  t += (dpint)a[0] * (dpint)b[10];
  t += (dpint)a[1] * (dpint)b[9];
  t += (dpint)a[2] * (dpint)b[8];
  t += (dpint)a[3] * (dpint)b[7];
  t += (dpint)a[4] * (dpint)b[6];
  t += (dpint)a[5] * (dpint)b[5];
  t += (dpint)a[6] * (dpint)b[4];
  t += (dpint)a[7] * (dpint)b[3];
  t += (dpint)a[8] * (dpint)b[2];
  t += (dpint)a[9] * (dpint)b[1];
  t += (dpint)a[10] * (dpint)b[0];
  spint v10 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma12 * (dpint)b[17];
  t += (dpint)ma13 * (dpint)b[16];
  t += (dpint)ma14 * (dpint)b[15];
  t += (dpint)ma15 * (dpint)b[14];
  t += (dpint)ma16 * (dpint)b[13];
  t += (dpint)ma17 * (dpint)b[12];
  t += (dpint)a[0] * (dpint)b[11];
  t += (dpint)a[1] * (dpint)b[10];
  t += (dpint)a[2] * (dpint)b[9];
  t += (dpint)a[3] * (dpint)b[8];
  t += (dpint)a[4] * (dpint)b[7];
  t += (dpint)a[5] * (dpint)b[6];
  t += (dpint)a[6] * (dpint)b[5];
  t += (dpint)a[7] * (dpint)b[4];
  t += (dpint)a[8] * (dpint)b[3];
  t += (dpint)a[9] * (dpint)b[2];
  t += (dpint)a[10] * (dpint)b[1];
  t += (dpint)a[11] * (dpint)b[0];
  spint v11 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma13 * (dpint)b[17];
  t += (dpint)ma14 * (dpint)b[16];
  t += (dpint)ma15 * (dpint)b[15];
  t += (dpint)ma16 * (dpint)b[14];
  t += (dpint)ma17 * (dpint)b[13];
  t += (dpint)a[0] * (dpint)b[12];
  t += (dpint)a[1] * (dpint)b[11];
  t += (dpint)a[2] * (dpint)b[10];
  t += (dpint)a[3] * (dpint)b[9];
  t += (dpint)a[4] * (dpint)b[8];
  t += (dpint)a[5] * (dpint)b[7];
  t += (dpint)a[6] * (dpint)b[6];
  t += (dpint)a[7] * (dpint)b[5];
  t += (dpint)a[8] * (dpint)b[4];
  t += (dpint)a[9] * (dpint)b[3];
  t += (dpint)a[10] * (dpint)b[2];
  t += (dpint)a[11] * (dpint)b[1];
  t += (dpint)a[12] * (dpint)b[0];
  spint v12 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma14 * (dpint)b[17];
  t += (dpint)ma15 * (dpint)b[16];
  t += (dpint)ma16 * (dpint)b[15];
  t += (dpint)ma17 * (dpint)b[14];
  t += (dpint)a[0] * (dpint)b[13];
  t += (dpint)a[1] * (dpint)b[12];
  t += (dpint)a[2] * (dpint)b[11];
  t += (dpint)a[3] * (dpint)b[10];
  t += (dpint)a[4] * (dpint)b[9];
  t += (dpint)a[5] * (dpint)b[8];
  t += (dpint)a[6] * (dpint)b[7];
  t += (dpint)a[7] * (dpint)b[6];
  t += (dpint)a[8] * (dpint)b[5];
  t += (dpint)a[9] * (dpint)b[4];
  t += (dpint)a[10] * (dpint)b[3];
  t += (dpint)a[11] * (dpint)b[2];
  t += (dpint)a[12] * (dpint)b[1];
  t += (dpint)a[13] * (dpint)b[0];
  spint v13 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma15 * (dpint)b[17];
  t += (dpint)ma16 * (dpint)b[16];
  t += (dpint)ma17 * (dpint)b[15];
  t += (dpint)a[0] * (dpint)b[14];
  t += (dpint)a[1] * (dpint)b[13];
  t += (dpint)a[2] * (dpint)b[12];
  t += (dpint)a[3] * (dpint)b[11];
  t += (dpint)a[4] * (dpint)b[10];
  t += (dpint)a[5] * (dpint)b[9];
  t += (dpint)a[6] * (dpint)b[8];
  t += (dpint)a[7] * (dpint)b[7];
  t += (dpint)a[8] * (dpint)b[6];
  t += (dpint)a[9] * (dpint)b[5];
  t += (dpint)a[10] * (dpint)b[4];
  t += (dpint)a[11] * (dpint)b[3];
  t += (dpint)a[12] * (dpint)b[2];
  t += (dpint)a[13] * (dpint)b[1];
  t += (dpint)a[14] * (dpint)b[0];
  spint v14 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma16 * (dpint)b[17];
  t += (dpint)ma17 * (dpint)b[16];
  t += (dpint)a[0] * (dpint)b[15];
  t += (dpint)a[1] * (dpint)b[14];
  t += (dpint)a[2] * (dpint)b[13];
  t += (dpint)a[3] * (dpint)b[12];
  t += (dpint)a[4] * (dpint)b[11];
  t += (dpint)a[5] * (dpint)b[10];
  t += (dpint)a[6] * (dpint)b[9];
  t += (dpint)a[7] * (dpint)b[8];
  t += (dpint)a[8] * (dpint)b[7];
  t += (dpint)a[9] * (dpint)b[6];
  t += (dpint)a[10] * (dpint)b[5];
  t += (dpint)a[11] * (dpint)b[4];
  t += (dpint)a[12] * (dpint)b[3];
  t += (dpint)a[13] * (dpint)b[2];
  t += (dpint)a[14] * (dpint)b[1];
  t += (dpint)a[15] * (dpint)b[0];
  spint v15 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)ma17 * (dpint)b[17];
  t += (dpint)a[0] * (dpint)b[16];
  t += (dpint)a[1] * (dpint)b[15];
  t += (dpint)a[2] * (dpint)b[14];
  t += (dpint)a[3] * (dpint)b[13];
  t += (dpint)a[4] * (dpint)b[12];
  t += (dpint)a[5] * (dpint)b[11];
  t += (dpint)a[6] * (dpint)b[10];
  t += (dpint)a[7] * (dpint)b[9];
  t += (dpint)a[8] * (dpint)b[8];
  t += (dpint)a[9] * (dpint)b[7];
  t += (dpint)a[10] * (dpint)b[6];
  t += (dpint)a[11] * (dpint)b[5];
  t += (dpint)a[12] * (dpint)b[4];
  t += (dpint)a[13] * (dpint)b[3];
  t += (dpint)a[14] * (dpint)b[2];
  t += (dpint)a[15] * (dpint)b[1];
  t += (dpint)a[16] * (dpint)b[0];
  spint v16 = (spint)t & mask;
  t = t >> 29u;
  t += (dpint)a[0] * (dpint)b[17];
  t += (dpint)a[1] * (dpint)b[16];
  t += (dpint)a[2] * (dpint)b[15];
  t += (dpint)a[3] * (dpint)b[14];
  t += (dpint)a[4] * (dpint)b[13];
  t += (dpint)a[5] * (dpint)b[12];
  t += (dpint)a[6] * (dpint)b[11];
  t += (dpint)a[7] * (dpint)b[10];
  t += (dpint)a[8] * (dpint)b[9];
  t += (dpint)a[9] * (dpint)b[8];
  t += (dpint)a[10] * (dpint)b[7];
  t += (dpint)a[11] * (dpint)b[6];
  t += (dpint)a[12] * (dpint)b[5];
  t += (dpint)a[13] * (dpint)b[4];
  t += (dpint)a[14] * (dpint)b[3];
  t += (dpint)a[15] * (dpint)b[2];
  t += (dpint)a[16] * (dpint)b[1];
  t += (dpint)a[17] * (dpint)b[0];
  spint v17 = (spint)t & mask;
  t = t >> 29u;
  // second reduction pass

  udpint ut = (udpint)t;
  ut = (ut << 1) + (spint)(v17 >> 28u);
  v17 &= 0xfffffff;
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
  c[9] = v9;
  c[10] = v10;
  c[11] = v11;
  c[12] = v12;
  c[13] = v13;
  c[14] = v14;
  c[15] = v15;
  c[16] = v16;
  c[17] = v17;
}

// Modular squaring, c=a*a mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint t = 0;
  spint ta1 = a[1] * (spint)2;
  spint ta2 = a[2] * (spint)2;
  spint ta3 = a[3] * (spint)2;
  spint ta4 = a[4] * (spint)2;
  spint ta5 = a[5] * (spint)2;
  spint ta6 = a[6] * (spint)2;
  spint ta7 = a[7] * (spint)2;
  spint ta8 = a[8] * (spint)2;
  spint ta9 = a[9] * (spint)2;
  spint ta10 = a[10] * (spint)2;
  spint ta11 = a[11] * (spint)2;
  spint ta12 = a[12] * (spint)2;
  spint ta13 = a[13] * (spint)2;
  spint ta14 = a[14] * (spint)2;
  spint ta15 = a[15] * (spint)2;
  spint ta16 = a[16] * (spint)2;
  spint ta17 = a[17] * (spint)2;
  spint ma1 = a[1] * (spint)0x2;
  spint ma2 = a[2] * (spint)0x2;
  spint ma3 = a[3] * (spint)0x2;
  spint ma4 = a[4] * (spint)0x2;
  spint ma5 = a[5] * (spint)0x2;
  spint ma6 = a[6] * (spint)0x2;
  spint ma7 = a[7] * (spint)0x2;
  spint ma8 = a[8] * (spint)0x2;
  spint ma9 = a[9] * (spint)0x2;
  spint ma10 = a[10] * (spint)0x2;
  spint ma11 = a[11] * (spint)0x2;
  spint ma12 = a[12] * (spint)0x2;
  spint ma13 = a[13] * (spint)0x2;
  spint ma14 = a[14] * (spint)0x2;
  spint ma15 = a[15] * (spint)0x2;
  spint ma16 = a[16] * (spint)0x2;
  spint ma17 = a[17] * (spint)0x2;
  spint carry;
  spint s;
  spint mask = ((spint)1 << 29u) - (spint)1;
  t += (udpint)ma1 * (udpint)ta17;
  t += (udpint)ma2 * (udpint)ta16;
  t += (udpint)ma3 * (udpint)ta15;
  t += (udpint)ma4 * (udpint)ta14;
  t += (udpint)ma5 * (udpint)ta13;
  t += (udpint)ma6 * (udpint)ta12;
  t += (udpint)ma7 * (udpint)ta11;
  t += (udpint)ma8 * (udpint)ta10;
  t += (udpint)ma9 * (udpint)a[9];
  t += (udpint)a[0] * (udpint)a[0];
  spint v0 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma2 * (udpint)ta17;
  t += (udpint)ma3 * (udpint)ta16;
  t += (udpint)ma4 * (udpint)ta15;
  t += (udpint)ma5 * (udpint)ta14;
  t += (udpint)ma6 * (udpint)ta13;
  t += (udpint)ma7 * (udpint)ta12;
  t += (udpint)ma8 * (udpint)ta11;
  t += (udpint)ma9 * (udpint)ta10;
  t += (udpint)a[0] * (udpint)ta1;
  spint v1 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma3 * (udpint)ta17;
  t += (udpint)ma4 * (udpint)ta16;
  t += (udpint)ma5 * (udpint)ta15;
  t += (udpint)ma6 * (udpint)ta14;
  t += (udpint)ma7 * (udpint)ta13;
  t += (udpint)ma8 * (udpint)ta12;
  t += (udpint)ma9 * (udpint)ta11;
  t += (udpint)ma10 * (udpint)a[10];
  t += (udpint)a[0] * (udpint)ta2;
  t += (udpint)a[1] * (udpint)a[1];
  spint v2 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma4 * (udpint)ta17;
  t += (udpint)ma5 * (udpint)ta16;
  t += (udpint)ma6 * (udpint)ta15;
  t += (udpint)ma7 * (udpint)ta14;
  t += (udpint)ma8 * (udpint)ta13;
  t += (udpint)ma9 * (udpint)ta12;
  t += (udpint)ma10 * (udpint)ta11;
  t += (udpint)a[0] * (udpint)ta3;
  t += (udpint)a[1] * (udpint)ta2;
  spint v3 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma5 * (udpint)ta17;
  t += (udpint)ma6 * (udpint)ta16;
  t += (udpint)ma7 * (udpint)ta15;
  t += (udpint)ma8 * (udpint)ta14;
  t += (udpint)ma9 * (udpint)ta13;
  t += (udpint)ma10 * (udpint)ta12;
  t += (udpint)ma11 * (udpint)a[11];
  t += (udpint)a[0] * (udpint)ta4;
  t += (udpint)a[1] * (udpint)ta3;
  t += (udpint)a[2] * (udpint)a[2];
  spint v4 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma6 * (udpint)ta17;
  t += (udpint)ma7 * (udpint)ta16;
  t += (udpint)ma8 * (udpint)ta15;
  t += (udpint)ma9 * (udpint)ta14;
  t += (udpint)ma10 * (udpint)ta13;
  t += (udpint)ma11 * (udpint)ta12;
  t += (udpint)a[0] * (udpint)ta5;
  t += (udpint)a[1] * (udpint)ta4;
  t += (udpint)a[2] * (udpint)ta3;
  spint v5 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma7 * (udpint)ta17;
  t += (udpint)ma8 * (udpint)ta16;
  t += (udpint)ma9 * (udpint)ta15;
  t += (udpint)ma10 * (udpint)ta14;
  t += (udpint)ma11 * (udpint)ta13;
  t += (udpint)ma12 * (udpint)a[12];
  t += (udpint)a[0] * (udpint)ta6;
  t += (udpint)a[1] * (udpint)ta5;
  t += (udpint)a[2] * (udpint)ta4;
  t += (udpint)a[3] * (udpint)a[3];
  spint v6 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma8 * (udpint)ta17;
  t += (udpint)ma9 * (udpint)ta16;
  t += (udpint)ma10 * (udpint)ta15;
  t += (udpint)ma11 * (udpint)ta14;
  t += (udpint)ma12 * (udpint)ta13;
  t += (udpint)a[0] * (udpint)ta7;
  t += (udpint)a[1] * (udpint)ta6;
  t += (udpint)a[2] * (udpint)ta5;
  t += (udpint)a[3] * (udpint)ta4;
  spint v7 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma9 * (udpint)ta17;
  t += (udpint)ma10 * (udpint)ta16;
  t += (udpint)ma11 * (udpint)ta15;
  t += (udpint)ma12 * (udpint)ta14;
  t += (udpint)ma13 * (udpint)a[13];
  t += (udpint)a[0] * (udpint)ta8;
  t += (udpint)a[1] * (udpint)ta7;
  t += (udpint)a[2] * (udpint)ta6;
  t += (udpint)a[3] * (udpint)ta5;
  t += (udpint)a[4] * (udpint)a[4];
  spint v8 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma10 * (udpint)ta17;
  t += (udpint)ma11 * (udpint)ta16;
  t += (udpint)ma12 * (udpint)ta15;
  t += (udpint)ma13 * (udpint)ta14;
  t += (udpint)a[0] * (udpint)ta9;
  t += (udpint)a[1] * (udpint)ta8;
  t += (udpint)a[2] * (udpint)ta7;
  t += (udpint)a[3] * (udpint)ta6;
  t += (udpint)a[4] * (udpint)ta5;
  spint v9 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma11 * (udpint)ta17;
  t += (udpint)ma12 * (udpint)ta16;
  t += (udpint)ma13 * (udpint)ta15;
  t += (udpint)ma14 * (udpint)a[14];
  t += (udpint)a[0] * (udpint)ta10;
  t += (udpint)a[1] * (udpint)ta9;
  t += (udpint)a[2] * (udpint)ta8;
  t += (udpint)a[3] * (udpint)ta7;
  t += (udpint)a[4] * (udpint)ta6;
  t += (udpint)a[5] * (udpint)a[5];
  spint v10 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma12 * (udpint)ta17;
  t += (udpint)ma13 * (udpint)ta16;
  t += (udpint)ma14 * (udpint)ta15;
  t += (udpint)a[0] * (udpint)ta11;
  t += (udpint)a[1] * (udpint)ta10;
  t += (udpint)a[2] * (udpint)ta9;
  t += (udpint)a[3] * (udpint)ta8;
  t += (udpint)a[4] * (udpint)ta7;
  t += (udpint)a[5] * (udpint)ta6;
  spint v11 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma13 * (udpint)ta17;
  t += (udpint)ma14 * (udpint)ta16;
  t += (udpint)ma15 * (udpint)a[15];
  t += (udpint)a[0] * (udpint)ta12;
  t += (udpint)a[1] * (udpint)ta11;
  t += (udpint)a[2] * (udpint)ta10;
  t += (udpint)a[3] * (udpint)ta9;
  t += (udpint)a[4] * (udpint)ta8;
  t += (udpint)a[5] * (udpint)ta7;
  t += (udpint)a[6] * (udpint)a[6];
  spint v12 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma14 * (udpint)ta17;
  t += (udpint)ma15 * (udpint)ta16;
  t += (udpint)a[0] * (udpint)ta13;
  t += (udpint)a[1] * (udpint)ta12;
  t += (udpint)a[2] * (udpint)ta11;
  t += (udpint)a[3] * (udpint)ta10;
  t += (udpint)a[4] * (udpint)ta9;
  t += (udpint)a[5] * (udpint)ta8;
  t += (udpint)a[6] * (udpint)ta7;
  spint v13 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma15 * (udpint)ta17;
  t += (udpint)ma16 * (udpint)a[16];
  t += (udpint)a[0] * (udpint)ta14;
  t += (udpint)a[1] * (udpint)ta13;
  t += (udpint)a[2] * (udpint)ta12;
  t += (udpint)a[3] * (udpint)ta11;
  t += (udpint)a[4] * (udpint)ta10;
  t += (udpint)a[5] * (udpint)ta9;
  t += (udpint)a[6] * (udpint)ta8;
  t += (udpint)a[7] * (udpint)a[7];
  spint v14 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma16 * (udpint)ta17;
  t += (udpint)a[0] * (udpint)ta15;
  t += (udpint)a[1] * (udpint)ta14;
  t += (udpint)a[2] * (udpint)ta13;
  t += (udpint)a[3] * (udpint)ta12;
  t += (udpint)a[4] * (udpint)ta11;
  t += (udpint)a[5] * (udpint)ta10;
  t += (udpint)a[6] * (udpint)ta9;
  t += (udpint)a[7] * (udpint)ta8;
  spint v15 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)ma17 * (udpint)a[17];
  t += (udpint)a[0] * (udpint)ta16;
  t += (udpint)a[1] * (udpint)ta15;
  t += (udpint)a[2] * (udpint)ta14;
  t += (udpint)a[3] * (udpint)ta13;
  t += (udpint)a[4] * (udpint)ta12;
  t += (udpint)a[5] * (udpint)ta11;
  t += (udpint)a[6] * (udpint)ta10;
  t += (udpint)a[7] * (udpint)ta9;
  t += (udpint)a[8] * (udpint)a[8];
  spint v16 = (spint)t & mask;
  t = t >> 29u;
  t += (udpint)a[0] * (udpint)ta17;
  t += (udpint)a[1] * (udpint)ta16;
  t += (udpint)a[2] * (udpint)ta15;
  t += (udpint)a[3] * (udpint)ta14;
  t += (udpint)a[4] * (udpint)ta13;
  t += (udpint)a[5] * (udpint)ta12;
  t += (udpint)a[6] * (udpint)ta11;
  t += (udpint)a[7] * (udpint)ta10;
  t += (udpint)a[8] * (udpint)ta9;
  spint v17 = (spint)t & mask;
  t = t >> 29u;
  // second reduction pass

  udpint ut = (udpint)t;
  ut = (ut << 1) + (spint)(v17 >> 28u);
  v17 &= 0xfffffff;
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
  c[9] = v9;
  c[10] = v10;
  c[11] = v11;
  c[12] = v12;
  c[13] = v13;
  c[14] = v14;
  c[15] = v15;
  c[16] = v16;
  c[17] = v17;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 18; i++) {
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
  spint x[18];
  spint t0[18];
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modcpy(z, t0);
  modnsqr(t0, 2);
  modmul(z, t0, z);
  modcpy(z, t0);
  modnsqr(t0, 4);
  modmul(z, t0, z);
  modcpy(z, t0);
  modnsqr(t0, 8);
  modmul(z, t0, z);
  modcpy(z, t0);
  modnsqr(t0, 16);
  modmul(z, t0, z);
  modcpy(z, t0);
  modnsqr(t0, 32);
  modmul(z, t0, z);
  modsqr(z, t0);
  modmul(x, t0, t0);
  modnsqr(t0, 64);
  modmul(z, t0, z);
  modsqr(z, t0);
  modmul(x, t0, t0);
  modnsqr(t0, 129);
  modmul(z, t0, z);
  modsqr(z, t0);
  modmul(x, t0, t0);
  modnsqr(t0, 259);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available, NULL if not
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[18];
  spint t[18];
  if (h == NULL) {
    modpro(x, t);
  } else {
    modcpy(h, t);
  }
  modcpy(x, s);
  modnsqr(t, 2);
  modmul(s, t, z);
}

// Convert m to internal form, n=nres(m)
static void nres(const spint *m, spint *n) {
  int i;
  for (i = 0; i < 18; i++) {
    n[i] = m[i];
  }
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  for (i = 0; i < 18; i++) {
    m[i] = n[i];
  }
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[18];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 18; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 29u) &
          (((c0 ^ (spint)1) - (spint)1) >> 29u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[18];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 18; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 29u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 18; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 18; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 18; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr(const spint *h, const spint *x) {
  spint r[18];
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
static void modcmv(int d, const spint *g, spint *f) {
  int i;
  spint t;
  spint r0 = f[0] ^ g[1];
  spint r1 = f[1] ^ g[0];
  spint c0 = (1 - (d - r0));
  spint c1 = d+r1;
  for (i = 0; i < 18; i++) {
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
  for (i = 0; i < 18; i++) {
      t=f[i]; s=g[i];
      f[i] =c0*t+c1*s;
      g[i] =c0*s+c1*t;
      f[i]-=r0*t+r1*s;
      g[i]-=r0*s+r1*t;
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  spint s[18];
  spint y[18];
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
  a[17] = ((a[17] << n)) | (a[16] >> (29u - n));
  for (i = 16; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x1fffffff) | (a[i - 1] >> (29u - n));
  }
  a[0] = (a[0] << n) & (spint)0x1fffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 17; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (29u - n)) & (spint)0x1fffffff);
  }
  a[17] = a[17] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 29u;
  unsigned int m = r % 29u;
  modzer(a);
  if (r >= 66 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[18];
  redc(a, c);
  for (i = 65; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 18; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 66; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[18];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[18], d[18];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 18; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
  }
  return eq;
}

/*** End of automatically generated code ***/

#include "nist521curve.h"

#define BYTES Nbytes
#define LIMBS Nlimbs
#define TOPBIT (8*sizeof(int)-1)

/*** Insert automatically generated curve definition curve.c here ***/


#define COF 1
#define CONSTANT_A -3
static const spint constant_b[18]={0xb503f00,0x1a28fea3,0xb0d3c7b,0x7bf107a,0x1bf07357,0x5e9dd8,0xdec594b,0xa3d8fd2,0x1561939,0xc77884f,0xe2d2264,0x13662be7,0xda725b9,0x2a07751,0x88682da,0x1343f253,0x19618e1c,0x28ca9f5};
static const spint constant_b3[18]={0x1f0bd00,0xe7afbea,0x127b573,0x173d316f,0x13d15a05,0x11bd98a,0x9c50be1,0x1eb8af77,0x4024bab,0x56698ed,0xa87672d,0x1a3283b6,0x8f5712c,0x7e165f4,0x1993888e,0x19cbd6f9,0xc24aa55,0x7a5fde1};
static const spint constant_x[18]={0x2e5bd66,0xbf3f18e,0x1a90a6fe,0x1167830a,0x1a8de334,0x93d17f,0x4a3f877,0xefdfceb,0x1aa14b5e,0x35a69ed,0x1e0a2bd8,0xa7f6a43,0x6481390,0xada214e,0x1b2d988e,0x1d39b3c7,0x6b70404,0x6342c70};
static const spint constant_y[18]={0x1fd16650,0x5f4a3b4,0x1cb09022,0x18e10d44,0x10761353,0x1c809fd6,0x19031542,0x132bde84,0xc97ee72,0x1939f331,0x5ebef45,0xf3688d0,0xf544495,0x1e8deccc,0x97ed0b1,0x18008b91,0xa789a3b,0x8c1c94b};


/*** End of automatically generated code ***/

// return 1 if b==c, no branching 
static int teq(int b, int c)
{
    int x = b ^ c;
    x -= 1; // if x=0, x now -1
    return ((x >> TOPBIT) & 1);
}

// copy point
void ecn521cpy(point *Q,point *P)
{
    modcpy(Q->x,P->x);
    modcpy(Q->y,P->y);
    modcpy(Q->z,P->z);
}

// negate P
void ecn521neg(point *P)
{
    modneg(P->y,P->y);
}

// add Q to P
// complete formulae from https://eprint.iacr.org/2015/1060
void ecn521add(point *Q,point *P)
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
void ecn521sub(point *Q,point *P)
{
    point W;
    ecn521cpy(Q,&W); ecn521neg(&W);
    ecn521add(&W,P);
}

// double P
// complete formuale from https://eprint.iacr.org/2015/1060
void ecn521dbl(point *P)
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
void ecn521inf(point *P)
{
    modzer(P->x);
    modone(P->y);
    modzer(P->z);
}

// test for infinity
int ecn521isinf(point *P)
{
    return (modis0(P->x) && modis0(P->z));
}

// set to affine
void ecn521affine(point *P)
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
void ecn521cmv(int d,point *Q,point *P)
{
    modcmv(d,Q->x,P->x);
    modcmv(d,Q->y,P->y);
    modcmv(d,Q->z,P->z);
}

// return 1 if equal, else 0
int ecn521cmp(point *P,point *Q)
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
int ecn521get(point *P,char *x,char *y)
{
    spint X[Nlimbs],Y[Nlimbs];
    ecn521affine(P);

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
int ecn521getxyz(point *P,char *x,char *y,char *z)
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
            ecn521inf(P);
        }
        return;
    }
    modpro(V,H);
    if (!modqr(H,V))
    { // point not on curve
        ecn521inf(P);
        return;
    }
    modsqrt(V,H,P->y);
    int d=(modsign(P->y)-s)&1;
    modneg(P->y,T);
    modcmv(d,T,P->y);
    modone(P->z);
}    

// multiply point by small curve cofactor (here assumed to be 1)
void ecn521cof(point *P)
{}

// api visible version, x and y are big endian byte arrays
void ecn521set(int s,const char *x,const char *y,point *P)
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
void ecn521gen(point *P)
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

    ecn521cmv(teq(babs, 0),&W[0],P); // conditional move
    ecn521cmv(teq(babs, 1),&W[1],P); // conditional move
    ecn521cmv(teq(babs, 2),&W[2],P); // conditional move
    ecn521cmv(teq(babs, 3),&W[3],P); // conditional move
    ecn521cmv(teq(babs, 4),&W[4],P); // conditional move
    ecn521cmv(teq(babs, 5),&W[5],P); // conditional move
    ecn521cmv(teq(babs, 6),&W[6],P); // conditional move
    ecn521cmv(teq(babs, 7),&W[7],P); // conditional move
    ecn521cmv(teq(babs, 8),&W[8],P); // conditional move
    
    ecn521cpy(P,&MP);
    ecn521neg(&MP);  // minus P
    ecn521cmv((int)(m & 1),&MP,P);
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
void ecn521mul(const char *e,point *P) 
{
    int i,j;
    point Q,W[9];
    signed char w[2*Nbytes+1];

    ecn521inf(&Q); 
    ecn521inf(&W[0]);                         // O
    ecn521cpy(P,&W[1]);                       // P
    ecn521cpy(P,&W[2]); ecn521dbl(&W[2]);        // 2P
    ecn521cpy(&W[2],&W[3]); ecn521add(P,&W[3]);  // 3P
    ecn521cpy(&W[2],&W[4]); ecn521dbl(&W[4]);    // 4P
    ecn521cpy(&W[4],&W[5]); ecn521add(P,&W[5]);  // 5P
    ecn521cpy(&W[3],&W[6]); ecn521dbl(&W[6]);    // 6P
    ecn521cpy(&W[6],&W[7]); ecn521add(P,&W[7]);  // 7P
    ecn521cpy(&W[4],&W[8]); ecn521dbl(&W[8]);    // 8P

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
        ecn521dbl(P);
        ecn521dbl(P);
        ecn521dbl(P);
        ecn521dbl(P);
        ecn521add(&Q,P);
    }
}

// double point multiplication R=eP+fQ
// not constant time
void ecn521mul2(const char *e,point *P,const char *f,point *Q,point *R)
{
    int i,j;
    point W[5];
    signed char w[8*Nbytes+8];
    ecn521inf(&W[0]);     // O
    ecn521cpy(P,&W[1]);   // P
    ecn521cpy(Q,&W[3]);   // Q
    ecn521cpy(Q,&W[2]); ecn521sub(P,&W[2]);  // Q-P
    ecn521cpy(Q,&W[4]); ecn521add(P,&W[4]);  // Q+P

    dnaf(e,f,w);

    i=8*Nbytes+7;
    while (w[i]==0) i--; // ignore leading zeros
    ecn521inf(R);
    while (i>=1)
    {
        ecn521dbl(R);
        j=w[i];
        if (j>0) ecn521add(&W[j],R);
        if (j<0) ecn521sub(&W[-j],R);
        i--;
    }
}
